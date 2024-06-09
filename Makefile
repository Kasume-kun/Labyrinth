Comp = g++
CPPFlags = -lSDL -O3 -W -Wall -ansi -pedantic

output: labyrinth.cpp quickcg.cpp
	$(Comp) *.cpp $(CPPFlags) -o Labyrinth

clean:
	rm Labyrinth
