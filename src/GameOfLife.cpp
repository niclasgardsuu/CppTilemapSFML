#ifndef CLASS_GAMEOFLIFE
#define CLASS_GAMEOFLIFE

#include <SFML/Graphics.hpp>
#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>

#include "Tilemap.cpp"
#include "ClickableTilemap.cpp"

#define DIMX 100
#define DIMY 100
#define AIR_TILE 0
#define WALL_TILE 1

class GameOfLife : public ClickableTilemap {
    private:
        bool placed, paused;
        float timer, tick_time;
    public:
        GameOfLife() : ClickableTilemap() {
            placed = true;
            timer = 0.f;
            tick_time = 0.5f;
            reset();
        }

        void click(float x, float y) {
            ClickableTilemap::click(x,y);
            placed = false;
        }

        void pause() {
            timer = 0;
            paused = !paused;
        }

        void incTick(float amount) {
            tick_time *= amount;
        }

        void decTick(float amount) {
            tick_time /= amount;
        }

        void setTick(float value) {
            tick_time = value;
        }

        void tick(float dt) {
            if(!placed) {
                std::cout << "PLACE";
                if(getTile(target.x,target.y))
                    setTile(target.x,target.y,0);
                else 
                    //REMOVE ME: Only have this to play around :)
                    setTile(target.x,target.y,1);
                    setTile(target.x+1,target.y,1);
                    setTile(target.x,target.y+1,1);
                    setTile(target.x+1,target.y+1,1);

                    setTile(target.x+10,target.y,1);
                    setTile(target.x+10,target.y+1,1);
                    setTile(target.x+10,target.y+2,1);
                    setTile(target.x+11,target.y-1,1);
                    setTile(target.x+11,target.y+3,1);
                    setTile(target.x+12,target.y-2,1);
                    setTile(target.x+12,target.y+4,1);
                    setTile(target.x+13,target.y-2,1);
                    setTile(target.x+13,target.y+4,1);
                    setTile(target.x+14,target.y+1,1);
                    setTile(target.x+15,target.y-1,1);
                    setTile(target.x+15,target.y+3,1);
                    setTile(target.x+16,target.y,1);
                    setTile(target.x+16,target.y+1,1);
                    setTile(target.x+16,target.y+2,1);
                    setTile(target.x+17,target.y+1,1);
                    
                    setTile(target.x+20,target.y-2,1);
                    setTile(target.x+20,target.y-1,1);
                    setTile(target.x+20,target.y,1);
                    setTile(target.x+21,target.y-2,1);
                    setTile(target.x+21,target.y-1,1);
                    setTile(target.x+21,target.y,1);
                    setTile(target.x+22,target.y-3,1);
                    setTile(target.x+22,target.y+1,1);
                    setTile(target.x+24,target.y-4,1);
                    setTile(target.x+24,target.y-3,1);
                    setTile(target.x+24,target.y+1,1);
                    setTile(target.x+24,target.y+2,1);

                    setTile(target.x+34,target.y-2,1);
                    setTile(target.x+34,target.y-1,1);
                    setTile(target.x+35,target.y-2,1);
                    setTile(target.x+35,target.y-1,1);
                    //setTile(target.x,target.y,1);
                placed = true;
            }
            if(!paused) {
                timer += dt;
                while(timer >= tick_time) {
                    game_tick();
                    timer -= tick_time;
                }
            } else {

            }
        }

        void game_tick() {
            int new_map[DIMY][DIMX];
            for(int y = 1; y < DIMY - 1; y++) {
                for(int x = 1; x < DIMX - 1; x++) {
                    int neighbors = getTile(x-1,y+1) + getTile(x,y+1) + getTile(x+1,y+1) + 
                                    getTile(x-1,y)   +                + getTile(x+1,y)   + 
                                    getTile(x-1,y-1) + getTile(x,y-1) + getTile(x+1,y-1);
                    if(neighbors < 2) {
                        new_map[y][x] = 0;
                    } else if(neighbors == 2) {
                        new_map[y][x] = getTile(x,y);
                    } else if(neighbors == 3) {
                        new_map[y][x] = 1;
                    } else if(neighbors > 3) {
                        new_map[y][x] = 0;
                    }
                }
            }
        
            for(int y = 1; y < DIMY - 1; y++)
                for(int x = 1; x < DIMX - 1; x++)
                    setTile(x,y,new_map[y][x]);
        }

        void render(sf::RenderWindow &window, sf::Vector2f origin,int zoom) {
            ClickableTilemap::render(window,origin,zoom);
        }

        ~GameOfLife() {
            
        }

};

#endif