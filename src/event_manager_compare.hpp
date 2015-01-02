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
				   ChunkManager& chunkmanager){

	// INIT

	const float INERTIA_FACTOR = 1.01;
	const float INERTIA_JUMP_FACTOR = 1.05;

	float gravityFactor = 0.03f;
	float playerSpeed = 0.05f;

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
		velocity+=glm::vec3(0,1,0)*(1.5f*playerSpeed);
		ffCam.setJumpInertia(glm::vec3(0,1,0)*(1.5f*playerSpeed));
	}
	else if(windowManager.isKeyPressed(SDLK_b)) 
	{
		velocity+=glm::vec3(0,1,0)*(-playerSpeed);
	}



	// PHYSICS
	static const int CHUNK_SIZE = Chunk::CHUNK_SIZE;

	/*Block*** blocks = chunkmanager.getChunk((int)ffCam.getPosition().x/CHUNK_SIZE,
											(int)ffCam.getPosition().y/CHUNK_SIZE,
											(int)ffCam.getPosition().z/CHUNK_SIZE)->getBlocks();
*/
	

	// COLLISION
	
	int chunkX = (int)glm::round(ffCam.getPosition().x)/CHUNK_SIZE;
		if (ffCam.getPosition().x < 0) chunkX += -1;
	int chunkY = (int)glm::round(ffCam.getPosition().y)/CHUNK_SIZE;
		if (ffCam.getPosition().y < 0) chunkY += -1;
	int chunkZ = (int)glm::round(ffCam.getPosition().z)/CHUNK_SIZE;
		if (ffCam.getPosition().z < 0) chunkZ += -1;

	int blockX = (int)glm::round(ffCam.getPosition().x)-CHUNK_SIZE*chunkX;
		if (blockX == CHUNK_SIZE) blockX = CHUNK_SIZE -1;
	int blockY = (int)glm::round(ffCam.getPosition().y)-CHUNK_SIZE*chunkY;
		if (blockY == CHUNK_SIZE) blockY = CHUNK_SIZE -1;
	int blockZ = (int)glm::round(ffCam.getPosition().z)-CHUNK_SIZE*chunkZ;
		if (blockZ == CHUNK_SIZE) blockZ = CHUNK_SIZE -1;

	std::cout << "chunk : " << chunkX << "," << chunkY << "," << chunkZ << std::endl;
	std::cout << "chuck.block : " << blockX << "," << blockY << "," << blockZ << std::endl;


	// sol
	chunkY = (int)glm::round(ffCam.getPosition().y+velocity.y-1.5)/CHUNK_SIZE;
		if (ffCam.getPosition().y+velocity.y-1.5 < 0) chunkY += -1;
	blockY = (int)glm::round(ffCam.getPosition().y+velocity.y-1.5)-CHUNK_SIZE*chunkY;
		if (blockY == CHUNK_SIZE) blockY = CHUNK_SIZE -1;

	if(chunkmanager.getChunk(chunkX,chunkY,chunkZ)->getBlocks()
	   [blockX][blockY][blockZ].isActive())
	{
		gravityFactor = 0.00f;
		ffCam.setInertia(glm::vec3(0,0,0));
	}


	// plafon
	chunkY = (int)glm::round(ffCam.getPosition().y+velocity.y+0.5)/CHUNK_SIZE;
		if (ffCam.getPosition().y+velocity.y+0.5 < 0) chunkY += -1;
	blockY = (int)glm::round(ffCam.getPosition().y+velocity.y+0.5)-CHUNK_SIZE*chunkY;
		if (blockY == CHUNK_SIZE) blockY = CHUNK_SIZE -1;
	
	if(chunkmanager.getChunk(chunkX,chunkY,chunkZ)->getBlocks()
	   [blockX][blockY][blockZ].isActive())
	{
		velocity=glm::vec3(velocity.x,0,velocity.z);
		ffCam.setJumpInertia(glm::vec3(0,0,0));
	}

	int countCollision =0;

	// +x genou
	chunkX = (int)glm::round(ffCam.getPosition().x+velocity.x+0.1)/CHUNK_SIZE;
		if (ffCam.getPosition().x+velocity.x+0.1 < 0) chunkX += -1;
	blockX = (int)glm::round(ffCam.getPosition().x+velocity.x+0.1)-CHUNK_SIZE*chunkX;
		if (blockX == CHUNK_SIZE) blockX = CHUNK_SIZE -1;
	chunkY = (int)glm::round(ffCam.getPosition().y-1)/CHUNK_SIZE;
		if (ffCam.getPosition().y-1 < 0) chunkY += -1;
	blockY = (int)glm::round(ffCam.getPosition().y-1)-CHUNK_SIZE*chunkY;
		if (blockY == CHUNK_SIZE) blockY = CHUNK_SIZE -1;

	if(chunkmanager.getChunk(chunkX,chunkY,chunkZ)->getBlocks()
	   [blockX][blockY][blockZ].isActive())
	{
		countCollision += 1;
		if(windowManager.isKeyPressed(SDLK_z))
		{
			velocity=glm::vec3(0,velocity.y,ffCam.getFrontVector().z*playerSpeed);
		}
		else velocity=glm::vec3(0,velocity.y,0);
		ffCam.setInertia(glm::vec3(0,0,0));
	}


	// +x tete
	chunkY = (int)glm::round(ffCam.getPosition().y)/CHUNK_SIZE;
		if (ffCam.getPosition().y < 0) chunkY += -1;
	blockY = (int)glm::round(ffCam.getPosition().y)-CHUNK_SIZE*chunkY;
		if (blockY == CHUNK_SIZE) blockY = CHUNK_SIZE -1;

	if(chunkmanager.getChunk(chunkX,chunkY,chunkZ)->getBlocks()
	   [blockX][blockY][blockZ].isActive())
	{
		countCollision += 1;
		if(windowManager.isKeyPressed(SDLK_z))
		{
			velocity=glm::vec3(0,velocity.y,ffCam.getFrontVector().z*playerSpeed);
		}
		else velocity=glm::vec3(0,velocity.y,0);
		ffCam.setInertia(glm::vec3(0,0,0));
	}


	// -x genou
	chunkX = (int)glm::round(ffCam.getPosition().x+velocity.x-0.1)/CHUNK_SIZE;
		if (ffCam.getPosition().x+velocity.x-0.1 < 0) chunkX += -1;
	blockX = (int)glm::round(ffCam.getPosition().x+velocity.x-0.1)-CHUNK_SIZE*chunkX;
		if (blockX == CHUNK_SIZE) blockX = CHUNK_SIZE -1;
	chunkY = (int)glm::round(ffCam.getPosition().y-1)/CHUNK_SIZE;
		if (ffCam.getPosition().y-1 < 0) chunkY += -1;
	blockY = (int)glm::round(ffCam.getPosition().y-1)-CHUNK_SIZE*chunkY;
		if (blockY == CHUNK_SIZE) blockY = CHUNK_SIZE -1;

	if(chunkmanager.getChunk(chunkX,chunkY,chunkZ)->getBlocks()
	   [blockX][blockY][blockZ].isActive())
	{
		countCollision += 1;
		if(windowManager.isKeyPressed(SDLK_z))
		{
			velocity=glm::vec3(0,velocity.y,ffCam.getFrontVector().z*playerSpeed);
		}
		else velocity=glm::vec3(0,velocity.y,0);
		ffCam.setInertia(glm::vec3(0,0,0));
	}


	// -x tete
	chunkY = (int)glm::round(ffCam.getPosition().y)/CHUNK_SIZE;
		if (ffCam.getPosition().y < 0) chunkY += -1;
	blockY = (int)glm::round(ffCam.getPosition().y)-CHUNK_SIZE*chunkY;
		if (blockY == CHUNK_SIZE) blockY = CHUNK_SIZE -1;

	if(chunkmanager.getChunk(chunkX,chunkY,chunkZ)->getBlocks()
	   [blockX][blockY][blockZ].isActive())
	{
		countCollision += 1;
		if(windowManager.isKeyPressed(SDLK_z))
		{
			velocity=glm::vec3(0,velocity.y,ffCam.getFrontVector().z*playerSpeed);
		}
		else velocity=glm::vec3(0,velocity.y,0);
		ffCam.setInertia(glm::vec3(0,0,0));
	}


	chunkX = (int)glm::round(ffCam.getPosition().x)/CHUNK_SIZE;
		if (ffCam.getPosition().x < 0) chunkX += -1;
	blockX = (int)glm::round(ffCam.getPosition().x)-CHUNK_SIZE*chunkX;
		if (blockX == CHUNK_SIZE) blockX = CHUNK_SIZE -1;

		// +z genou
	chunkZ = (int)glm::round(ffCam.getPosition().z+velocity.z+0.1)/CHUNK_SIZE;
		if (ffCam.getPosition().z+velocity.z+0.1 < 0) chunkZ += -1;
	blockZ = (int)glm::round(ffCam.getPosition().z+velocity.z+0.1)-CHUNK_SIZE*chunkZ;
		if (blockZ == CHUNK_SIZE) blockZ = CHUNK_SIZE -1;
	chunkY = (int)glm::round(ffCam.getPosition().y+-1)/CHUNK_SIZE;
		if (ffCam.getPosition().y-1 < 0) chunkY += -1;
	blockY = (int)glm::round(ffCam.getPosition().y-1)-CHUNK_SIZE*chunkY;
		if (blockY == CHUNK_SIZE) blockY = CHUNK_SIZE -1;

	if(chunkmanager.getChunk(chunkX,chunkY,chunkZ)->getBlocks()
	   [blockX][blockY][blockZ].isActive())
	{
		countCollision += 1;
		if(windowManager.isKeyPressed(SDLK_z))
		{
			velocity=glm::vec3(ffCam.getFrontVector().x*playerSpeed,velocity.y,0);
		}
		else velocity=glm::vec3(0,velocity.y,0);
		ffCam.setInertia(glm::vec3(0,0,0));
	}


	// +z tete
	chunkY = (int)glm::round(ffCam.getPosition().y)/CHUNK_SIZE;
		if (ffCam.getPosition().y < 0) chunkY += -1;
	blockY = (int)glm::round(ffCam.getPosition().y)-CHUNK_SIZE*chunkY;
		if (blockY == CHUNK_SIZE) blockY = CHUNK_SIZE -1;

	if(chunkmanager.getChunk(chunkX,chunkY,chunkZ)->getBlocks()
	   [blockX][blockY][blockZ].isActive())
	{
		countCollision += 1;
		if(windowManager.isKeyPressed(SDLK_z))
		{
			velocity=glm::vec3(ffCam.getFrontVector().x*playerSpeed,velocity.y,0);
		}
		else velocity=glm::vec3(0,velocity.y,0);
		ffCam.setInertia(glm::vec3(0,0,0));
	}


	// -z genou
	chunkZ = (int)glm::round(ffCam.getPosition().z+velocity.z-0.1)/CHUNK_SIZE;
		if (ffCam.getPosition().z+velocity.z-0.1 < 0) chunkZ += -1;
	blockZ = (int)glm::round(ffCam.getPosition().z+velocity.z-0.1)-CHUNK_SIZE*chunkZ;
		if (blockZ == CHUNK_SIZE) blockZ = CHUNK_SIZE -1;
	chunkY = (int)glm::round(ffCam.getPosition().y-1)/CHUNK_SIZE;
		if (ffCam.getPosition().y-1 < 0) chunkY += -1;
	blockY = (int)glm::round(ffCam.getPosition().y-1)-CHUNK_SIZE*chunkY;
		if (blockY == CHUNK_SIZE) blockY = CHUNK_SIZE -1;

	if(chunkmanager.getChunk(chunkX,chunkY,chunkZ)->getBlocks()
	   [blockX][blockY][blockZ].isActive())
	{
		countCollision += 1;
		if(windowManager.isKeyPressed(SDLK_z))
		{
			velocity=glm::vec3(ffCam.getFrontVector().x*playerSpeed,velocity.y,0);
		}
		else velocity=glm::vec3(0,velocity.y,0);
		ffCam.setInertia(glm::vec3(0,0,0));
	}


	// -z tete
	chunkY = (int)glm::round(ffCam.getPosition().y)/CHUNK_SIZE;
		if (ffCam.getPosition().y < 0) chunkY += -1;
	blockY = (int)glm::round(ffCam.getPosition().y)-CHUNK_SIZE*chunkY;
		if (blockY == CHUNK_SIZE) blockY = CHUNK_SIZE -1;

	if(chunkmanager.getChunk(chunkX,chunkY,chunkZ)->getBlocks()
	   [blockX][blockY][blockZ].isActive())
	{
		countCollision += 1;
		if(windowManager.isKeyPressed(SDLK_z))
		{
			velocity=glm::vec3(ffCam.getFrontVector().x*playerSpeed,velocity.y,0);
		}
		else velocity=glm::vec3(0,velocity.y,0);
		ffCam.setInertia(glm::vec3(0,0,0));
	}

	if(countCollision > 1) velocity=glm::vec3(0,velocity.y,0);














	// MOUVEMENT

	// mouvements
	ffCam.slide(velocity);
	// gravity
	ffCam.moveUp(-gravityFactor);
	// y inertia
	if (ffCam.getJumpInertia().y > 0.001
		&& !windowManager.isKeyPressed(SDLK_SPACE)
		)
	{
		ffCam.divideJumpInertia(INERTIA_JUMP_FACTOR);

		ffCam.moveUp(ffCam.getJumpInertia().y);
	}
	// x/z inertia
	if (sqrt(ffCam.getInertia().x*ffCam.getInertia().x + ffCam.getInertia().z*ffCam.getInertia().z) > 0.0005
		&& !windowManager.isKeyPressed(SDLK_z)
		&& !windowManager.isKeyPressed(SDLK_s)
		&& !windowManager.isKeyPressed(SDLK_q)
		&& !windowManager.isKeyPressed(SDLK_d))
	{
		ffCam.divideInertia(INERTIA_FACTOR);

		ffCam.slide(ffCam.getInertia());
	}

};