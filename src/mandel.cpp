#include <SFML/Graphics.hpp>
#include <complex.h>
#include <iostream>
#include <cmath>

#define ITERATIONS 10
#define XLOW ((double)-2)
#define XHIGH ((double)0.5)
#define YLOW ((double)-1.25)
#define YHIGH ((double)1.25)

#define WIDTH 1920
#define HEIGHT 1080

bool check_diverged_point(double real, double imag) {
    std::complex<double> c(real,imag);
    std::complex<double> current = c;
    for(int i = 0; i < ITERATIONS; i++) {
        current = current * current + c;
        if(std::abs(current) > 2) {
            return true;
        }
    }
    return false;
}

//if not divering, we can fill rectangle within borders
void check_diverging_borders(sf::Vector2<double> topl, sf::Vector2<double> lowr,sf::Uint8 *image, double xlow, double xhigh, double ylow, double yhigh) {
    bool diverged = false;
    bool converged = false;
    for(double x = topl.x; x <= lowr.x; x++) {
        if(check_diverged_point((x/WIDTH)*(xhigh-xlow) + xlow,(topl.y/HEIGHT)*(yhigh-ylow) + ylow)) {
            diverged = true;
        } else
        {
            converged = true;
        }
        if(check_diverged_point((x/WIDTH)*(xhigh-xlow) + xlow,(lowr.y/HEIGHT)*(yhigh-ylow) + ylow)) {
            diverged = true;
        } else
        {
            converged = true;
        }
        if(diverged&&converged) {
            break;
        }
    }
    for(double y = topl.y + 1; y <= lowr.y - 1; y++) {
        if(check_diverged_point((topl.x/WIDTH)*(xhigh-xlow) + xlow,(y/HEIGHT)*(yhigh-ylow) + ylow)) {
            diverged = true;
        } else
        {
            converged = true;
        }
        if(check_diverged_point((lowr.x/WIDTH)*(xhigh-xlow) + xlow,(y/HEIGHT)*(yhigh-ylow) + ylow)) {
            diverged = true;
        } else
        {
            converged = true;
        }
        if(diverged&&converged) {
            break;
        }
    }

    if(!converged) {
        //fill with black (not in set)
        for(int x = topl.x; x <= lowr.x; x++) {
            for(int y = topl.y; y <= lowr.y; y++) {
                *(image + x*4 + y*WIDTH*4)     = 0;
                *(image + x*4 + y*WIDTH*4 + 1) = 120;
                *(image + x*4 + y*WIDTH*4 + 2) = 80;
                *(image + x*4 + y*WIDTH*4 + 3) = 255;
            }
        }/*
        for(int x = topl.x; x < lowr.x; x++) {
            *(image + x*4 + (int)lowr.y*WIDTH*4    ) = 125;
            *(image + x*4 + (int)lowr.y*WIDTH*4 + 1) = 125;
            *(image + x*4 + (int)lowr.y*WIDTH*4 + 2) = 125;
            *(image + x*4 + (int)lowr.y*WIDTH*4 + 3) = 255;
        }
        for(int y = topl.y; y < lowr.y; y++) {
            *(image + (int)lowr.x*4 + y*WIDTH*4    ) = 255;
            *(image + (int)lowr.x*4 + y*WIDTH*4 + 1) = 255;
            *(image + (int)lowr.x*4 + y*WIDTH*4 + 2) = 255;
            *(image + (int)lowr.x*4 + y*WIDTH*4 + 3) = 255;
        }*/
        return;
    }
    if(!diverged) {
        //fill with white (in set)
        for(int x = topl.x; x <= lowr.x; x++) {
            for(int y = topl.y; y <= lowr.y; y++) {
                *(image + x*4 + y*WIDTH*4 )    = 255;
                *(image + x*4 + y*WIDTH*4 + 1) = 255;
                *(image + x*4 + y*WIDTH*4 + 2) = 255;
                *(image + x*4 + y*WIDTH*4 + 3) = 255;
            }
        }
        //std::cout << "vit\n";
        //std::cout << "(" << topl.x << ", " << topl.y << ")\n(" << lowr.x << ", " << lowr.y << ")\n\n";
        return;
    }




    sf::Vector2<double> mid_top( (int)(topl.x + ((lowr.x - topl.x)/2)) , topl.y);
    sf::Vector2<double> mid_bot( (int)(topl.x + ((lowr.x - topl.x)/2)) , lowr.y);
    sf::Vector2<double> mid_rgt( lowr.x                                , (int)(topl.y + ((lowr.y - topl.y)/2)));
    sf::Vector2<double> mid_lft( topl.x                                , (int)(topl.y + ((lowr.y - topl.y)/2)));
    sf::Vector2<double> mid(     (int)(topl.x + ((lowr.x - topl.x)/2)) , (int)(topl.y + ((lowr.y - topl.y)/2)));
    check_diverging_borders(topl,mid,image,xlow,xhigh,ylow,yhigh);
    if(topl.x < lowr.x) {
        check_diverging_borders(sf::Vector2<double>(mid_top.x + 1, mid_top.y),mid_rgt,image,xlow,xhigh,ylow,yhigh);
    }
    if(topl.y < lowr.y) {
        check_diverging_borders(sf::Vector2<double>(mid_lft.x, mid_lft.y + 1),mid_bot,image,xlow,xhigh,ylow,yhigh);
    }
    if(topl.x < lowr.x && topl.y < lowr.y) {
        check_diverging_borders(sf::Vector2<double>(mid.x + 1, mid.y + 1),lowr,image,xlow,xhigh,ylow,yhigh);
    }
}


void math(sf::Uint8 *image) {
    for(int y = 0; y < HEIGHT; y++) { 
        for(int x = 0; x < WIDTH; x++) {
            double rx = x / 1000;
            double ry = y / 1000;
            if(tan(rx*rx * sin(ry*ry)) - tan(ry*ry * sin(rx*rx)) < 0.1) {
                *(image + x*4 + y*WIDTH*4 )    = 255;
                *(image + x*4 + y*WIDTH*4 + 1) = 255;
                *(image + x*4 + y*WIDTH*4 + 2) = 255;
                *(image + x*4 + y*WIDTH*4 + 3) = 255;
            } else {
                *(image + x*4 + y*WIDTH*4 )    = 0;
                *(image + x*4 + y*WIDTH*4 + 1) = 0;
                *(image + x*4 + y*WIDTH*4 + 2) = 0;
                *(image + x*4 + y*WIDTH*4 + 3) = 255;
            }
        }
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(WIDTH,HEIGHT), "Mandelbrot");
    window.setFramerateLimit(60);
    sf::Uint8 *pixels = new sf::Uint8[WIDTH*HEIGHT*4];
    std::cout << "COCK";
    double xlow = -2;
    double xhigh = 0.5;
    double ylow = -1.25;
    double yhigh = 1.25;

    //check_diverging_borders(sf::Vector2<double>(0,0),sf::Vector2<double>(HEIGHT - 1,WIDTH - 1),pixels,xlow,xhigh,ylow,yhigh);
    
    sf::Image mandelbrot;
    sf::Texture mandelbrot_texture;
    sf::Sprite background;

    float fps;
    sf::Clock clock;
    sf::Time previousTime = clock.getElapsedTime();
    sf::Time currentTime;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if(event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up)
                    window.close();
            } else if(event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();
            } else if(event.type == sf::Event::MouseButtonReleased) {
                if(event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i pos = sf::Mouse::getPosition();
                    sf::Vector2i winpos = window.getPosition();
                    sf::Vector2<double> relpos((((double)pos.x-winpos.x)/ WIDTH)*(xhigh - xlow) + xlow, (((double)pos.y - winpos.y)/ HEIGHT)*(yhigh - ylow) + ylow);
                    std::cout << "(" << relpos.x << ", " << relpos.y <<")\n";
                    xlow = xlow - ((xlow - relpos.x) * 0.1);
                    xhigh = xhigh - ((xhigh - relpos.x) * 0.1);
                    ylow = ylow - ((ylow - relpos.y) * 0.1);
                    yhigh = yhigh - ((yhigh - relpos.y) * 0.1);
                }
            }
        }

        // check_diverging_borders(sf::Vector2<double>(0,0),sf::Vector2<double>(HEIGHT - 1,WIDTH - 1),pixels,xlow,xhigh,ylow,yhigh);
        math(pixels);
        mandelbrot.create((unsigned int)WIDTH,(unsigned int)HEIGHT,pixels);
        mandelbrot_texture.loadFromImage(mandelbrot);
        background.setTexture(mandelbrot_texture);

        window.clear();
        window.draw(background);
        window.display();

    currentTime = clock.getElapsedTime();
    fps = 1.0f / (currentTime.asSeconds() - previousTime.asSeconds()); // the asSeconds returns a float
    std::cout << "fps =" << floor(fps) << std::endl; // flooring it will make the frame rate a rounded number
    previousTime = currentTime;
    }
    return 0;
}
/*
no_points = 50000;
check_diverge_steps = 500;
xlow = -2;
xhigh = 0.5;
ylow = -1.25;
yhigh = 1.25;
frames = 10000;
step = 0.05;
x = -0.1674;
y = 1.0412;
divergence = 2;

for k = 1:frames
    points = zeros(1, 2*no_points);
    count = 1;
    for i = xlow:(xhigh-xlow)/200:xhigh
        for j = ylow:(yhigh-ylow)/200:yhigh
            next = i + j*1i;
            fst = next;
            for l = 1:check_diverge_steps
                next = next^2 + fst;
                if(imag(next) > divergence || real(next) > divergence)
                    break;
                end
                if l == check_diverge_steps
                    points(count) = fst;
                    count = count + 1;
                end
            end
        end
    end
    plot(points, '.');
    xlim([xlow xhigh]);
    ylim([ylow yhigh]);
    %x = -0.1674
    %y = 1.0412
    xlow = xlow - (xlow - x) * step;
    xhigh = xhigh -(xhigh - x) * step;
    ylow = ylow -(ylow - y) * step;
    yhigh = yhigh - (yhigh - y) * step;
    pause(.001);
end
*/