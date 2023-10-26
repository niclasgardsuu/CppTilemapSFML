MANDEL := mandel.cpp
MANDELO := mandel.o
MMO := mmo.cpp Tilemap.cpp ClickableTilemap.cpp GameOfLife.cpp
MMOO := mmo.o Tilemap.o ClickableTilemap.o GameOfLife.o

all:
	echo 'jarmo'

mandel: $(MANDELO)
	g++ $^ -o $@ -Lsrc/lib -lsfml-graphics -lsfml-window -lsfml-system
	del *.o

gol: $(MMOO)
	g++ $^ -o $@ -Lsrc/lib -lsfml-graphics -lsfml-window -lsfml-system
	del *.o

%.o: src/%.cpp
	g++ -std=c++11 -Isrc/include -c $^ -g -Wall -Wextra

clean:
	del *.o