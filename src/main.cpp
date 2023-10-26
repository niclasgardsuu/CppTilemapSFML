#include <SFML/Graphics.hpp>
#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>

#include "Tilemap.cpp"
#include "ClickableTilemap.cpp"
#include "GameOfLife.cpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(1920,1080), "Mandelbrot", sf::Style::Fullscreen);
    sf::Vector2f origin(0,0), offset(0,0);
    // ClickableTilemap map;
    GameOfLife map;
    map.setTick(0.1);
    
    sf::Vector2i position(0,0), prev_position(0,0);
    int zoom = 50;
    float fps;
    sf::Clock clock;
    sf::Time previousTime = clock.getElapsedTime();
    sf::Time currentTime;

    bool holding_map = false;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if(event.type == sf::Event::KeyPressed) {
                std::cout << "key\n";
            } else if(event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();
                else if (event.key.code == sf::Keyboard::W)
                    origin.y -= 0.03;
                else if (event.key.code == sf::Keyboard::A)
                    origin.x -= 0.03;
                else if (event.key.code == sf::Keyboard::S)
                    origin.y += 0.03;
                else if (event.key.code == sf::Keyboard::D)
                    origin.x += 0.03;
                else if (event.key.code == sf::Keyboard::T)
                    map.game_tick();
                else if (event.key.code == sf::Keyboard::Space)
                    map.pause();
                else if (event.key.code == sf::Keyboard::Up)
                    map.decTick(1.1f);
                else if (event.key.code == sf::Keyboard::Down)
                    map.incTick(1.1f);
            } else if(event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    holding_map = true;
                    prev_position = sf::Mouse::getPosition(window);
                }
            } else if(event.type == sf::Event::MouseWheelScrolled) {
                zoom -= event.mouseWheelScroll.delta;
                if(zoom < 1) zoom = 1;
                if(zoom > 200) zoom = 200;
            } else if(event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    holding_map = false;
                    origin -= sf::Vector2f((offset.x*zoom/window.getSize().x),(offset.y*zoom/window.getSize().x));
                    offset = sf::Vector2f(0,0);
                }   
                if (event.mouseButton.button == sf::Mouse::Right) {
                    sf::Vector2f pos = (sf::Vector2f)sf::Mouse::getPosition(window);
                    sf::Vector2u window_size = window.getSize();

                    // Divide by window_size.x because tiles have same height as width
                    map.click(origin.x + (pos.x - window_size.x/2)*(float)zoom/(float)window_size.x, 
                              origin.y + (pos.y - window_size.y/2)*(float)zoom/(float)window_size.x);
                    std::cout << origin.x + (pos.x - window_size.x/2)*zoom/window_size.x << " - " << 
                                 origin.y + (pos.y - window_size.y/2)*zoom/window_size.x << std::endl;
                }
            }
        }

        if(holding_map) {
            position = sf::Mouse::getPosition(window);
            offset = (sf::Vector2f)position - (sf::Vector2f)prev_position;
        }


        window.clear();
        //render
        map.render(window, sf::Vector2f(origin.x - (offset.x*zoom/window.getSize().x), origin.y - (offset.y*zoom/window.getSize().x)), zoom);

        window.display();

        currentTime = clock.getElapsedTime();
        fps = 1.0f / (currentTime.asSeconds() - previousTime.asSeconds()); // the asSeconds returns a float
        map.tick((currentTime.asSeconds() - previousTime.asSeconds()));
        //std::cout << "fps =" << floor(fps) << std::endl; // flooring it will make the frame rate a rounded number
        previousTime = currentTime;
    }
    return 0;
}