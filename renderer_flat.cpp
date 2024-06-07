#include <iostream>
#include "quickcg.h"
#include <cmath>
#include <vector>
#include <string>

using namespace QuickCG;


#define mapHeight 24
#define mapWidth 24
#define texWidth 64
#define texHeight 64
#define screenHeight 480
#define screenWidth 640

int worldMap[mapHeight][mapWidth] = {
	{2,2,4,4,2,2,2,2,2,2,2,2,2,2,2,2,2,2,4,2,2,2,2,2},
	{2,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
	{2,0,2,2,2,4,2,0,2,0,2,0,5,5,5,5,5,5,5,5,5,0,0,2},
	{2,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,5,0,0,2},
	{2,2,2,2,2,2,2,2,2,0,2,0,5,5,5,5,5,5,5,0,5,0,0,2},
	{2,0,0,0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,5,0,0,4},
	{2,0,2,2,2,2,2,2,2,2,2,2,5,0,5,5,5,5,5,5,5,0,0,4},
	{2,0,0,0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,1,5,0,0,2},
	{2,2,2,2,2,2,2,2,2,4,2,0,5,5,5,5,5,5,5,5,2,1,1,2},
	{2,0,0,0,2,0,0,0,0,0,0,0,2,2,2,0,0,0,0,0,2,2,2,2},
	{2,0,2,0,4,0,5,0,5,0,5,0,0,0,2,0,0,5,0,0,0,0,0,2},
	{2,0,2,0,2,0,5,0,5,0,5,0,5,0,2,0,0,0,0,0,2,0,0,2},
	{2,0,2,0,2,0,0,0,0,0,0,0,0,0,2,0,2,0,2,0,2,0,0,2},
	{2,0,4,0,2,0,5,0,5,0,5,0,5,0,2,0,0,0,0,0,4,0,0,2},
	{2,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,2},
	{2,0,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,2,0,0,2},
	{2,0,2,1,2,0,0,0,2,1,2,0,0,0,0,0,0,0,0,0,4,0,0,2},
	{2,0,2,0,2,0,2,0,2,0,2,0,5,0,5,0,2,0,4,0,2,0,0,2},
	{2,0,2,0,2,0,2,0,2,0,2,0,5,0,5,0,0,0,0,0,2,0,0,2},
	{2,1,2,0,2,0,2,0,2,0,4,0,5,0,5,0,0,5,0,0,2,0,0,2},
	{2,2,2,0,2,0,2,0,2,0,2,0,5,0,5,0,0,0,0,0,2,0,0,2},
	{3,0,2,0,2,0,2,0,2,0,2,0,5,0,5,4,2,2,2,4,2,0,0,2},
	{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
	{2,2,2,2,2,2,2,4,4,4,4,2,2,2,2,2,2,2,2,2,2,2,2,2},
};

Uint32 buffer[screenHeight][screenWidth];

int main(int argc, char **argv)
{
	float posX = 1, posY = 18;
	float dirX = -1, dirY = 0;
	float planeX = 0, planeY = 0.66;

	float time = 0, oldTime = 0;

	std::vector<Uint32> texture[5];
	for(int i = 0; i < 5; i++)
		texture[i].resize(texWidth*texHeight);

	screen(screenWidth, screenHeight, 0, "Raycaster");

	unsigned long tw, th;
	loadImage(texture[0], tw, th, "pics/DEAD.png");
	loadImage(texture[1], tw, th, "pics/bricks.png");
	loadImage(texture[2], tw, th, "pics/portal.png");
	loadImage(texture[3], tw, th, "pics/ruined_bricks.png");
	loadImage(texture[4], tw, th, "pics/wood1.png");

	while(!done())
	{
		for(int x = 0; x < w; x++)
		{
			float cameraX = 2 * x / (float)w - 1;
			float rayDirX = dirX + planeX * cameraX;
			float rayDirY = dirY + planeY * cameraX;

			int mapX = (int)posX;
			int mapY = (int)posY;

			float sideDistX;
			float sideDistY;

			float deltaDistX = (rayDirX == 0) ? 1e30 : std::abs(1 / rayDirX);
			float deltaDistY = (rayDirY == 0) ? 1e30 : std::abs(1 / rayDirY);
			float perpWallDist;

			int stepX;
			int stepY;

			int hit = 0;
			int side;

			if(rayDirX < 0)
			{
				stepX = -1;
				sideDistX = (posX - mapX) * deltaDistX;
			}
			else
			{
				stepX = 1;
				sideDistX = (mapX + 1.0 - posX) * deltaDistX;
			}
			if(rayDirY < 0)
			{
				stepY = -1;
				sideDistY = (posY - mapY) * deltaDistY;
			}
			else
			{
				stepY = 1;
				sideDistY = (mapY + 1.0 - posY) * deltaDistY;
			}

			//DDA
			while(hit == 0)
			{
				if(sideDistX < sideDistY)
				{
					sideDistX += deltaDistX;
					mapX += stepX;
					side = 0;
				}
				else
				{
					sideDistY += deltaDistY;
					mapY += stepY;
					side = 1;
				}

				if(worldMap[mapX][mapY] > 0)
					hit = 1;
			}

			if(side == 0)
				perpWallDist = (sideDistX - deltaDistX);
			else
				perpWallDist = (sideDistY - deltaDistY);

			int lineHeight = (int)(h / perpWallDist);

			int pitch = 100;

			int drawStart = -lineHeight/2 + h/2 + pitch;
			if(drawStart < 0)
				drawStart = 0;
			int drawEnd = lineHeight/2 + h/2 + pitch;
			if(drawEnd >= h)
				drawEnd = h - 1;

			int texNum = worldMap[mapX][mapY] -1;// This needs to be changed

			float wallX;
			if(side == 0)
				wallX = posY + perpWallDist * rayDirY;
			else
				wallX = posX + perpWallDist * rayDirX;
			wallX -= floor(wallX);

			int texX = int(wallX * float(texWidth));
			if(side == 0 && rayDirX > 0)
				texX = texWidth - texX -1;
			if(side == 1 && rayDirY < 0)
				texX = texWidth - texX -1;

			float step = 1.0 * texHeight / lineHeight;
			float texPos = (drawStart - pitch - h/2 + lineHeight/2) * step;

			for(int y=drawStart; y<drawEnd; y++)
			{
				int texY = (int)texPos & (texHeight - 1);
				texPos += step;
				Uint32 color = texture[texNum][texHeight * texY + texX];
				if(side == 1)
					color = (color >> 1) & 8355711;
				buffer[y][x]=color;
			}
		}

		drawBuffer(buffer[0]);
		for(int y=0; y<h; y++)
			for(int x=0; x<w; x++)
				buffer[y][x]=0;

		oldTime = time;
		time = getTicks();
		float frameTime = (time - oldTime)/1000.0;
		print(1.0/frameTime);
		redraw();
		//speed modifiers
    		double moveSpeed = frameTime * 5.0; //the constant value is in squares/second
    		double rotSpeed = frameTime * 3.0; //the constant value is in radians/second

    		readKeys();
    		//move forward if no wall in front of you
    		if(keyDown(SDLK_UP))
    		{
      			if(worldMap[int(posX + dirX * moveSpeed)][int(posY)] == false) 
				posX += dirX * moveSpeed;
      			if(worldMap[int(posX)][int(posY + dirY * moveSpeed)] == false) 
				posY += dirY * moveSpeed;
    		}
    		//move backwards if no wall behind you
    		if(keyDown(SDLK_DOWN))
    		{
      			if(worldMap[int(posX - dirX * moveSpeed)][int(posY)] == false) 
				posX -= dirX * moveSpeed;
      			if(worldMap[int(posX)][int(posY - dirY * moveSpeed)] == false) 
				posY -= dirY * moveSpeed;
    		}
	        //rotate to the right
	        if(keyDown(SDLK_RIGHT))
	        {
	      		//both camera direction and camera plane must be rotated
	      		double oldDirX = dirX;
	      		dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
	      		dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
	      		double oldPlaneX = planeX;
	      		planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
	      		planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
	        }
	   	 //rotate to the left
	        if(keyDown(SDLK_LEFT))
	        {
	      		//both camera direction and camera plane must be rotated
	      		double oldDirX = dirX;
	      		dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
	      		dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
	      		double oldPlaneX = planeX;
	      		planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
	      		planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
	    	}
	    	if(keyDown(SDLK_ESCAPE))
	    	{
	      		break;
	    	}	
	}

	return 0;
}
