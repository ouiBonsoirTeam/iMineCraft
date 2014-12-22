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
				   float PLAYER_SPEED,
				   bool& done,
				   Chunk& chunk){



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
		ffCam.moveFront(PLAYER_SPEED);
		if(windowManager.isKeyPressed(SDLK_LSHIFT)) 
		{
			ffCam.moveFront(PLAYER_SPEED);
		}
	}
	
	else if(windowManager.isKeyPressed(SDLK_s)) 
	{
		ffCam.moveFront(-PLAYER_SPEED);
	}
	if(windowManager.isKeyPressed(SDLK_q)) 
	{
		ffCam.moveLeft(PLAYER_SPEED);
	}
	else if(windowManager.isKeyPressed(SDLK_d)) 
	{
		ffCam.moveLeft(-PLAYER_SPEED);
	}
	if(windowManager.isKeyPressed(SDLK_SPACE)) 
	{
		ffCam.moveUp(PLAYER_SPEED);
	}
	else if(windowManager.isKeyPressed(SDLK_b)) 
	{
		ffCam.moveUp(-PLAYER_SPEED);
	}


	// PHYSICS

	

	// INERTIA

	const float INERTIA_FACTOR = 1.005;

	if(windowManager.isKeyPressed(SDLK_z)) 
		{
			ffCam.setInertia(ffCam.getFrontVector()*PLAYER_SPEED);
		}
	if(windowManager.isKeyPressed(SDLK_s)) 
		{
			ffCam.setInertia(ffCam.getFrontVector()*(-PLAYER_SPEED));
		}
	if(windowManager.isKeyPressed(SDLK_q)) 
		{
			ffCam.setInertia(ffCam.getLeftVector()*PLAYER_SPEED);
		}
	if(windowManager.isKeyPressed(SDLK_d)) 
		{
			ffCam.setInertia(ffCam.getLeftVector()*(-PLAYER_SPEED));
		}
	



	if (sqrt(ffCam.getInertia().x*ffCam.getInertia().x + ffCam.getInertia().z*ffCam.getInertia().z) > 0.005
		&& !windowManager.isKeyPressed(SDLK_z)
		&& !windowManager.isKeyPressed(SDLK_s)
		&& !windowManager.isKeyPressed(SDLK_q)
		&& !windowManager.isKeyPressed(SDLK_d))
	{
		ffCam.divideInertia(INERTIA_FACTOR);

		ffCam.slide(ffCam.getInertia());
	}



	// GRAVITY

	float gravityFactor = 0.005f;

	

	// TEST COLLISION
	/*
	Block*** blocks = chunk.getBlocks();
	if(blocks[(int)ffCam.getPosition().x][(int)ffCam.getPosition().y-1][(int)ffCam.getPosition().z].isActive())
		{
			gravityFactor = 0.00f;
			std::cerr<<"zizi"<<std::endl;
		}

	// GRAVITY
*/
	ffCam.moveUp(-gravityFactor);

};