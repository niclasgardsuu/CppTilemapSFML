#ifndef CLASS_TILEMAP
#define CLASS_TILEMAP

#include <SFML/Graphics.hpp>
#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>

#define DIMX 100
#define DIMY 100
#define OUTLINE_THICKNESS 1 //pixels
#define AIR_TILE 0
#define WALL_TILE 1
#define AIR_COLOR sf::Color::Black
#define WALL_COLOR sf::Color::White

class Tilemap {
    private:
        int min_i(int a, int b) {
            return a < b ? a : b;
        }
    protected:
    
    public:
        int map[DIMY][DIMX];

        Tilemap() {
            //fill map with empty tiles
            for(int y = 0; y < DIMY; y++) {
                for(int x = 0; x < DIMX; x++) {
                    map[y][x] = AIR_TILE;
                }
            }

            //TODO: remove this, this is just for debugging
            for(int i = 0; i < min_i(DIMX,DIMY); i++) {
                map[i][i] = 1;
            }
        }

        //overload constructor with inputs
        Tilemap(int **input) {
            //fill map with tiles from input array
            for(int y = 0; y < DIMY; y++) {
                for(int x = 0; x < DIMX; x++) {
                    map[y][x] = input[y][x];
                }
            }
        }

        // sets all tiles to empty
        void reset() {
            for(int y = 0; y < DIMY; y++) {
                for(int x = 0; x < DIMX; x++) {
                    map[y][x] = AIR_TILE;
                }
            }
        }

        void setTile(int x, int y, int value) {
            map[y][x] = value;
        }

        //gets value of tile
        int getTile(int x, int y) {
            if(x < 0 || x >= DIMX) {
                return -1;
            }
            if(y < 0 || y >= DIMY) {
                return -1;
            }
            return map[y][x];
        }

        //this one doesn't check valid input, since it is most likely going to be used privately
        int getTile(sf::Vector2i pos) {
            return map[pos.y][pos.x];
        }

        //precondition: One unit in float space is the same as 1 index in tilemap
        void render(sf::RenderWindow &window, sf::Vector2f origin, int zoom) {
            //TODO: make this depend on zooming variable
            int tiles_per_width = zoom;
            int tiles_per_height = ((float)window.getSize().y / (float)window.getSize().x) * tiles_per_width;
            float tile_width = (float)window.getSize().x/(float)tiles_per_width;
            float offset_x = origin.x - (int)origin.x;
            float offset_y = origin.y - (int)origin.y;


            sf::Vector2i origin_tile = (sf::Vector2i)origin;
            sf::RectangleShape tile;
            tile.setSize(sf::Vector2f(tile_width-OUTLINE_THICKNESS*1, tile_width-OUTLINE_THICKNESS*1));
            tile.setOutlineThickness(OUTLINE_THICKNESS);
            tile.setOutlineColor(sf::Color(50,50,50,255));
            tile.setOrigin(sf::Vector2f(-OUTLINE_THICKNESS, -OUTLINE_THICKNESS));

            int start_x = floor((float)-tiles_per_width / 2.f) + origin_tile.x;
            int end_x = (tiles_per_width / 2) + origin_tile.x + 1; //+1 because there were some rounding errors
            int start_y = floor((float)-tiles_per_height / 2.f) + origin_tile.y;
            int end_y = (tiles_per_height / 2) + origin_tile.y + 1; //+1 same reason
            if(start_x < 0)
                start_x = 0;
            if(end_x > DIMX)
                end_x = DIMX;
            if(start_y < 0)
                start_y = 0;
            if(end_y > DIMY)
                end_y = DIMY;

            for(int y = start_y; y <= end_y; y++) {
                for(int x = start_x; x <= end_x; x++) {
                    tile.setPosition(sf::Vector2f((x - origin_tile.x)*tile_width - offset_x*tile_width + window.getSize().x/2, (y - origin_tile.y)*tile_width - offset_y*tile_width + window.getSize().y/2));
                    if(x >= DIMX || x < 0 || y >= DIMY || y < 0)
                        continue;
                    int tile_value = getTile(sf::Vector2i(x,y));
                    switch (tile_value)
                    {
                    case 0:
                        tile.setFillColor(sf::Color::Black);
                        window.draw(tile);  
                        break;
                    case 1:
                        tile.setFillColor(sf::Color::White);
                        window.draw(tile);  
                        break;
                    case 2: //target
                        tile.setFillColor(sf::Color(120,150,150));
                        window.draw(tile);
                        break;
                    default: //Error case
                        tile.setFillColor(sf::Color::Green);
                        window.draw(tile);  
                        break;
                    }
                }
            }
        }

        ~Tilemap() {
            
        }
};

#endif