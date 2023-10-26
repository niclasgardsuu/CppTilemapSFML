#ifndef CLASS_CTILEMAP
#define CLASS_CTILEMAP

#include <SFML/Graphics.hpp>
#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>

#include "Tilemap.cpp"

#define DIMX 100
#define DIMY 100
#define AIR_TILE 0
#define WALL_TILE 1

class ClickableTilemap : public Tilemap {
    protected:
        sf::Vector2f target_position;
        sf::Vector2i target;

    public:
        ClickableTilemap() : Tilemap() {
            target = sf::Vector2i(-1,-1);    
        }

        ClickableTilemap(int **input) {
            //fill map with tiles from input array
            for(int y = 0; y < DIMY; y++) {
                for(int x = 0; x < DIMX; x++) {
                    map[y][x] = input[y][x];
                }
            }
        }

        void reset() {
            Tilemap::reset();
        }

        sf::Vector2i click(float x, float y) {
            target_position = sf::Vector2f(x,y);
            target = sf::Vector2i((int)x,(int)y);
        }

        // void setTile(int x, int y, int value) {
        //     Tilemap::setTile(x,y,value);
        // }

        void render(sf::RenderWindow &window, sf::Vector2f origin, int zoom) {
            Tilemap::render(window, origin, zoom); //superclass method of rendering

            int tiles_per_width = zoom;
            int tiles_per_height = ((float)window.getSize().y / (float)window.getSize().x) * tiles_per_width;
            float tile_width = (float)window.getSize().x/(float)tiles_per_width;
            float offset_x = origin.x - (int)origin.x;
            float offset_y = origin.y - (int)origin.y;

            sf::RectangleShape targ;
            targ.setSize(sf::Vector2f(tile_width, tile_width));
            targ.setFillColor(sf::Color(200,5,5,100));

            float x_render_position = (target.x - (int)origin.x - offset_x)*tile_width + window.getSize().x/2;
            float y_render_position = (target.y - (int)origin.y - offset_y)*tile_width + window.getSize().y/2;

            if(x_render_position > 0 && x_render_position < window.getSize().x && y_render_position > 0 && y_render_position < window.getSize().y) {
                targ.setPosition(sf::Vector2f(x_render_position, y_render_position));
                window.draw(targ);
            }
        }

        ~ClickableTilemap() {
            
        }
};

#endif