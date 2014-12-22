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

	// INIT

	Block*** blocks = chunk.getBlocks();
	const float INERTIA_FACTOR = 1.0001;
	float gravityFactor = 0.008f;
	float playerSpeed = 0.005f;

	glm::vec3 velocity=glm::vec3(0,0,0);

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
		velocity+=ffCam.getFrontVector()*playerSpeed;
		ffCam.setInertia(ffCam.getFrontVector()*playerSpeed);

		if(windowManager.isKeyPressed(SDLK_LSHIFT)) 
		{
			velocity+=ffCam.getFrontVector()*playerSpeed;
		}
	}
	
	else if(windowManager.isKeyPressed(SDLK_s)) 
	{
		velocity+=ffCam.getFrontVector()*(-playerSpeed);
		ffCam.setInertia(ffCam.getFrontVector()*(-playerSpeed));
	}


	if(windowManager.isKeyPressed(SDLK_q)) 
	{
		velocity+=ffCam.getLeftVector()*playerSpeed;
		ffCam.setInertia(ffCam.getLeftVector()*playerSpeed);
	}

	else if(windowManager.isKeyPressed(SDLK_d)) 
	{
		velocity+=ffCam.getLeftVector()*(-playerSpeed);
		ffCam.setInertia(ffCam.getLeftVector()*(-playerSpeed));
	}


	if(windowManager.isKeyPressed(SDLK_SPACE)) 
	{
		velocity+=glm::vec3(0,1,0)*(2*playerSpeed);
	}
	else if(windowManager.isKeyPressed(SDLK_b)) 
	{
		velocity+=glm::vec3(0,1,0)*(-2*playerSpeed);
	}



	// PHYSICS


	// COLLISION
	
	if(blocks[(int)glm::round(ffCam.getPosition().x)]
		     [(int)glm::round(ffCam.getPosition().y-1.5)]
		     [(int)glm::round(ffCam.getPosition().z)]
		     .isActive())
	{
		gravityFactor = 0.00f;
	}

	if(blocks[(int)glm::round((ffCam.getPosition()+velocity).x+0.2)]
			 [(int)glm::round((ffCam.getPosition()+velocity).y-1)]
			 [(int)glm::round((ffCam.getPosition()+velocity).z)]
			 .isActive())
	{
		if(windowManager.isKeyPressed(SDLK_z))
		{
			velocity=glm::vec3(0,0,ffCam.getFrontVector().z*playerSpeed);
		}
		else velocity=glm::vec3(0,0,0);
		ffCam.setInertia(glm::vec3(0,0,0));

	}

	if(blocks[(int)glm::round((ffCam.getPosition()+velocity).x-0.2)]
			 [(int)glm::round((ffCam.getPosition()+velocity).y-1)]
			 [(int)glm::round((ffCam.getPosition()+velocity).z)]
			 .isActive())
	{
		if(windowManager.isKeyPressed(SDLK_z))
		{
			velocity=glm::vec3(0,0,ffCam.getFrontVector().z*playerSpeed);
		}
		else velocity=glm::vec3(0,0,0);
		ffCam.setInertia(glm::vec3(0,0,0));

	}

	if(blocks[(int)glm::round((ffCam.getPosition()+velocity).x)]
			 [(int)glm::round((ffCam.getPosition()+velocity).y-1)]
			 [(int)glm::round((ffCam.getPosition()+velocity).z+0.2)]
			 .isActive())
	{
		if(windowManager.isKeyPressed(SDLK_z))
		{
			velocity=glm::vec3(ffCam.getFrontVector().x*playerSpeed,0,0);
		}
		else velocity=glm::vec3(0,0,0);
		ffCam.setInertia(glm::vec3(0,0,0));

	}

	if(blocks[(int)glm::round((ffCam.getPosition()+velocity).x)]
			 [(int)glm::round((ffCam.getPosition()+velocity).y-1)]
			 [(int)glm::round((ffCam.getPosition()+velocity).z-0.2)]
			 .isActive())
	{
		if(windowManager.isKeyPressed(SDLK_z))
		{
			velocity=glm::vec3(ffCam.getFrontVector().x*playerSpeed,0,0);
		}
		else velocity=glm::vec3(0,0,0);
		ffCam.setInertia(glm::vec3(0,0,0));

	}















	// MOUVEMENT

	ffCam.slide(velocity);
	ffCam.moveUp(-gravityFactor);
	if (sqrt(ffCam.getInertia().x*ffCam.getInertia().x + ffCam.getInertia().z*ffCam.getInertia().z) > 0.001
		&& !windowManager.isKeyPressed(SDLK_z)
		&& !windowManager.isKeyPressed(SDLK_s)
		&& !windowManager.isKeyPressed(SDLK_q)
		&& !windowManager.isKeyPressed(SDLK_d))
	{
		ffCam.divideInertia(INERTIA_FACTOR);

		ffCam.slide(ffCam.getInertia());
	}

};