# Labyrinth of Shadows
Step into the dark corridors of the ancient Labyrinth of Shadows, where reality bends and secrets lie hidden behind every stone wall. Inspired by the classic raycasting style of Wolfenstein 3D, this maze game plunges you into a retro-futuristic world.

## Controls
W - Forward  
S - Backward  
D - Rotate clockwise  
A - Rotate anticlockwise  

## Dependencies  
This project requires the QuickCG library a port of the SDL Library.  
To install it on a Debian based distro:
```
sudo apt install -libsdl-dev
```
To install it on a Arch based distro:
```
sudo pacman -S
```
After this follow the procedure mentioned in the execute section while ensuring you have the quickcg.h and quickcg.cpp files in the same directory as your labyrinth.cpp file.

## How to execute  
I have created the Makefile to compile all the reuired files into an executable called Labyrinth.  
Simply run these in order,  
```
make
./Labyrinth
```
Or you can just simply run the executable provided above as well.
```
./Labyrinth
```
