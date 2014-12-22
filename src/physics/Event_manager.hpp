#pragma once

#include <vector>
#include <cmath>
#include <glimac/SDLWindowManager.hpp>
#include <glimac/FreeFlyCamera.hpp>
#include "../voxel_engine/Chunk.hpp"

void event_manager(SDLWindowManager& windowManager,
				   FreeFlyCamera& ffCam,
				   float& angleX,float& angleY,float& angleYfinal,
				   float CAMERA_ROT_FACTOR,
				   bool& done,
				   Chunk& chunk){


	Block*** blocks = chunk.getBlocks();
	const float INERTIA_FACTOR = 1.0005;
	float gravityFactor = 0.005f;
	float playerSpeed = 0.01f;



	// EVENTS
	SDL_Event e;
	while(windowManager.pollEvent(e)) {
		if(e.type == SDL_QUIT) {
			done = true; // Leave the loop after this iteration
		}
		if (e.type == SDL_KEYDOWN)
		{
			if (e.key.keysym.sym == SDLK_ESCAPE)
			{
				done = true; // Leave the loop after this iteration
			}
		}

		//souris
		if (e.type == SDL_MOUSEMOTION)
		{
			angleX -= e.motion.xrel * CAMERA_ROT_FACTOR;
			angleY -= e.motion.yrel * CAMERA_ROT_FACTOR;
			angleYfinal -= e.motion.yrel * CAMERA_ROT_FACTOR;
			angleYfinal = std::min(90.0f, std::max(-90.0f, angleYfinal)); //pour pas passer sa tête entre ses jambes
		}
	}
	ffCam.rotateLeft(angleX);
	if (angleYfinal != 90 && angleYfinal !=-90) ffCam.rotateUp(angleY);
	angleY = 0;
	angleX = 0;
	
	
	
			
	if(windowManager.isKeyPressed(SDLK_z)) 
	{
		if(!blocks[(int)(ffCam.getPosition().x+0.5)][(int)ffCam.getPosition().y][(int)ffCam.getPosition().z].isActive())
		{
			ffCam.moveFront(playerSpeed);
			ffCam.setInertia(ffCam.getFrontVector()*playerSpeed);

			if(windowManager.isKeyPressed(SDLK_LSHIFT)) 
			{
				ffCam.moveFront(playerSpeed);
			}
		}
		else ffCam.setInertia(glm::vec3(0,0,0));
	}
	
	else if(windowManager.isKeyPressed(SDLK_s)) 
	{
		ffCam.moveFront(-playerSpeed);
		ffCam.setInertia(ffCam.getFrontVector()*(-playerSpeed));
	}
	if(windowManager.isKeyPressed(SDLK_q)) 
	{
		ffCam.moveLeft(playerSpeed);
		ffCam.setInertia(ffCam.getLeftVector()*playerSpeed);
	}
	else if(windowManager.isKeyPressed(SDLK_d)) 
	{
		ffCam.moveLeft(-playerSpeed);
		ffCam.setInertia(ffCam.getLeftVector()*(-playerSpeed));
	}
	if(windowManager.isKeyPressed(SDLK_SPACE)) 
	{
		ffCam.moveUp(playerSpeed);
	}
	else if(windowManager.isKeyPressed(SDLK_b)) 
	{
		ffCam.moveUp(-playerSpeed);
	}


	// PHYSICS

	

	// INERTIA


	if (sqrt(ffCam.getInertia().x*ffCam.getInertia().x + ffCam.getInertia().z*ffCam.getInertia().z) > 0.001
		&& !windowManager.isKeyPressed(SDLK_z)
		&& !windowManager.isKeyPressed(SDLK_s)
		&& !windowManager.isKeyPressed(SDLK_q)
		&& !windowManager.isKeyPressed(SDLK_d))
	{
		ffCam.divideInertia(INERTIA_FACTOR);

		ffCam.slide(ffCam.getInertia());
	}

	

	// COLLISION
	
	if(blocks[(int)ffCam.getPosition().x][(int)ffCam.getPosition().y-1][(int)ffCam.getPosition().z].isActive())
		{
			gravityFactor = 0.00f;
		}










	// GRAVITY

	ffCam.moveUp(-gravityFactor);

};