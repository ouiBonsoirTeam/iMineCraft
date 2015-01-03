#pragma once

#include <vector>
#include <cmath>
#include <glimac/SDLWindowManager.hpp>
#include <glimac/FreeFlyCamera.hpp>
#include "../voxel_engine/Chunk.hpp"
#include "../voxel_engine/ChunkManager.hpp"
#include "../voxel_engine/Block.hpp"
#include "../inventory.hpp"


Block* getBlockFromChunk(ChunkManager& chunkmanager, glm::vec3 position, glm::vec3 direction)
{
	int chunkX = (int) glm::round(position.x + direction.x) / Chunk::CHUNK_SIZE;
		if (position.x + direction.x < 0) chunkX += -1;

	int chunkY = (int) glm::round(position.y + direction.y) / Chunk::CHUNK_SIZE;
		if (position.y + direction.y < 0) chunkY += -1;

	int chunkZ = (int) glm::round(position.z + direction.z) / Chunk::CHUNK_SIZE;
		if (position.z + direction.z < 0) chunkZ += -1;


	int blockX = (int) glm::round(position.x + direction.x) - Chunk::CHUNK_SIZE * chunkX;
		if (blockX == Chunk::CHUNK_SIZE) blockX = Chunk::CHUNK_SIZE -1;

	int blockY = (int)glm::round(position.y + direction.y) - Chunk::CHUNK_SIZE * chunkY;
		if (blockY == Chunk::CHUNK_SIZE) blockY = Chunk::CHUNK_SIZE -1;

	int blockZ = (int)glm::round(position.z + direction.z) - Chunk::CHUNK_SIZE * chunkZ;
		if (blockZ == Chunk::CHUNK_SIZE) blockZ = Chunk::CHUNK_SIZE -1;



	return &chunkmanager.getChunk(chunkX, chunkY, chunkZ)->getBlocks()[blockX][blockY][blockZ];
}


void event_manager(SDLWindowManager& windowManager,
				   FreeFlyCamera& ffCam,
				   float& angleX,float& angleY,float& angleYfinal,
				   float CAMERA_ROT_FACTOR,
				   bool& done,
				   ChunkManager& chunkmanager,
				   Inventory& inventory,
				   int& crouch,
				   BlockType& currentBlockType){

	// INIT

	const float INERTIA_FACTOR = 1.01;
	const float INERTIA_JUMP_FACTOR = 1.05;

	float gravityFactor = 0.03f;
	float playerSpeed = 0.05f;

	glm::vec3 velocity=glm::vec3(0,0,0);

	bool leftClick = false;
	bool rightClick = false;


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

			if (e.key.keysym.sym == SDLK_LCTRL)
			{
				if(getBlockFromChunk(chunkmanager, ffCam.getPosition(), glm::vec3(0,-1.5,0))->isActive() && crouch == 0)
				{
					ffCam.slide(glm::vec3(0,-1,0));
					crouch = 1;	
				}
			}

		}
		if (e.type == SDL_KEYUP)
		{
			if (e.key.keysym.sym == SDLK_AMPERSAND)
			{
				currentBlockType=BlockType_Lava;
			}
			if (e.key.keysym.sym == 233) //é
			{
				currentBlockType=BlockType_Rock;
			}
			if (e.key.keysym.sym == SDLK_QUOTEDBL)
			{
				currentBlockType=BlockType_Earth;
			}
			if (e.key.keysym.sym == SDLK_QUOTE)
			{
				currentBlockType=BlockType_Grass;
			}
			if (e.key.keysym.sym == SDLK_LEFTPAREN)
			{
				currentBlockType=BlockType_1st_Snow;
			}
			if (e.key.keysym.sym == SDLK_MINUS)
			{
				currentBlockType=BlockType_Snow;
			}
			if (e.key.keysym.sym == 232) //è
			{
				currentBlockType=BlockType_Ice;
			}	

			if (e.key.keysym.sym == SDLK_LCTRL && crouch == 1) 
			{
				ffCam.slide(glm::vec3(0,+1,0));
				crouch = 0;		
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

		if (e.type == SDL_MOUSEBUTTONDOWN)
		{
			if (e.button.button == SDL_BUTTON_LEFT)
			{
				leftClick = true;
			}
			if (e.button.button == SDL_BUTTON_RIGHT)
			{
				rightClick = true;
			}
		}
	}
	ffCam.rotateLeft(angleX);
	if (angleYfinal != 90 && angleYfinal !=-90) ffCam.rotateUp(angleY);
	angleY = 0;
	angleX = 0;
	
	
	
			
	if(windowManager.isKeyPressed(SDLK_z)) 
	{
		ffCam.setInertia(ffCam.getFrontVector()*playerSpeed);
		if(windowManager.isKeyPressed(SDLK_LSHIFT)) 
		{
			velocity+=glm::vec3(ffCam.getFrontVector().x*playerSpeed*3.f,0,ffCam.getFrontVector().z*playerSpeed*3.f);
		}
		if(windowManager.isKeyPressed(SDLK_LCTRL)) 
		{
			velocity+=glm::vec3(ffCam.getFrontVector().x*playerSpeed*0.3f,0,ffCam.getFrontVector().z*playerSpeed*0.3f);
		}
		else velocity+=glm::vec3(ffCam.getFrontVector().x*playerSpeed,0,ffCam.getFrontVector().z*playerSpeed);
	}
	
	else if(windowManager.isKeyPressed(SDLK_s)) 
	{
		velocity+=glm::vec3(ffCam.getFrontVector().x*(-playerSpeed),0,ffCam.getFrontVector().z*(-playerSpeed));
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




	// PHYSICS

	// COLLISION


	// plafond
	if(getBlockFromChunk(chunkmanager, ffCam.getPosition(), glm::vec3(0,0.5,0))->isActive())
	{
		velocity=glm::vec3(velocity.x,0,velocity.z);
		ffCam.setJumpInertia(glm::vec3(0,0,0));
	}

	int countCollision =0;

	// +x
	if((getBlockFromChunk(chunkmanager, ffCam.getPosition(), glm::vec3(velocity.x + 0.1, -1.0, 0))->isActive() && crouch == 0)
		|| getBlockFromChunk(chunkmanager, ffCam.getPosition(), glm::vec3(velocity.x + 0.1, 0, 0))->isActive() )
	{
		countCollision += 1;
		if(windowManager.isKeyPressed(SDLK_z))
		{
			velocity=glm::vec3(0,velocity.y,ffCam.getFrontVector().z*playerSpeed);
		}
		else velocity=glm::vec3(0,velocity.y,0);
		ffCam.setInertia(glm::vec3(0,0,0));
	}


	// -x
	if((getBlockFromChunk(chunkmanager, ffCam.getPosition(), glm::vec3(velocity.x - 0.1, -1.0, 0))->isActive() && crouch == 0)
		|| getBlockFromChunk(chunkmanager, ffCam.getPosition(), glm::vec3(velocity.x - 0.1, 0, 0))->isActive() )
	{
		countCollision += 1;
		if(windowManager.isKeyPressed(SDLK_z))
		{
			velocity=glm::vec3(0,velocity.y,ffCam.getFrontVector().z*playerSpeed);
		}
		else velocity=glm::vec3(0,velocity.y,0);
		ffCam.setInertia(glm::vec3(0,0,0));
	}

	// +z

	if((getBlockFromChunk(chunkmanager, ffCam.getPosition(), glm::vec3(0, -1.0, velocity.z + 0.1))->isActive() && crouch == 0)
		|| getBlockFromChunk(chunkmanager, ffCam.getPosition(), glm::vec3(0, 0, velocity.z + 0.1))->isActive())
	{
		countCollision += 1;
		if(windowManager.isKeyPressed(SDLK_z))
		{
			velocity=glm::vec3(ffCam.getFrontVector().x*playerSpeed,velocity.y,0);
		}
		else velocity=glm::vec3(0,velocity.y,0);
		ffCam.setInertia(glm::vec3(0,0,0));
	}

	// -z

	if((getBlockFromChunk(chunkmanager, ffCam.getPosition(), glm::vec3(0, -1.0, velocity.z - 0.1))->isActive() && crouch == 0)
		|| getBlockFromChunk(chunkmanager, ffCam.getPosition(), glm::vec3(0, 0, velocity.z - 0.1))->isActive())
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


	// sol

	
	if((getBlockFromChunk(chunkmanager, ffCam.getPosition(), glm::vec3(0, velocity.y - 1.5, 0))->isActive() && crouch == 0)
		|| (getBlockFromChunk(chunkmanager, ffCam.getPosition(), glm::vec3(0, velocity.y - 0.5, 0))->isActive() && crouch == 1))
	{
		
		gravityFactor = 0.00f;
		velocity.y=0;
		ffCam.setInertia(glm::vec3(0,0,0));
	}






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

	int chunkX = (int) glm::round(ffCam.getPosition().x) / Chunk::CHUNK_SIZE;
			if (ffCam.getPosition().x < 0) chunkX += -1;

	int chunkY = (int) glm::round(ffCam.getPosition().y) / Chunk::CHUNK_SIZE;
		if (ffCam.getPosition().y < 0) chunkY += -1;

	int chunkZ = (int) glm::round(ffCam.getPosition().z) / Chunk::CHUNK_SIZE;
		if (ffCam.getPosition().z < 0) chunkZ += -1;


	int blockX = (int) glm::round(ffCam.getPosition().x ) - Chunk::CHUNK_SIZE * chunkX;
		if (blockX == Chunk::CHUNK_SIZE) blockX = Chunk::CHUNK_SIZE -1;

	int blockY = (int)glm::round(ffCam.getPosition().y) - Chunk::CHUNK_SIZE * chunkY;
		if (blockY == Chunk::CHUNK_SIZE) blockY = Chunk::CHUNK_SIZE -1;

	int blockZ = (int)glm::round(ffCam.getPosition().z) - Chunk::CHUNK_SIZE * chunkZ;
		if (blockZ == Chunk::CHUNK_SIZE) blockZ = Chunk::CHUNK_SIZE -1;

	// std::cout << "chunk : " << chunkX << "," << chunkY << "," << chunkZ << std::endl;
	// std::cout << "chuck.block : " << blockX << "," << blockY << "," << blockZ << std::endl;



	// DESTRUCT CUBE
	if (leftClick)
	{
		//Block* lookCube = getBlockFromChunk(chunkmanager, ffCam.getPosition(), ffCam.getFrontVector());

		chunkX = (int) glm::round(ffCam.getPosition().x + ffCam.getFrontVector().x) / Chunk::CHUNK_SIZE;
			if (ffCam.getPosition().x + ffCam.getFrontVector().x < 0) chunkX += -1;

		chunkY = (int) glm::round(ffCam.getPosition().y + ffCam.getFrontVector().y) / Chunk::CHUNK_SIZE;
			if (ffCam.getPosition().y + ffCam.getFrontVector().y < 0) chunkY += -1;

		chunkZ = (int) glm::round(ffCam.getPosition().z + ffCam.getFrontVector().z) / Chunk::CHUNK_SIZE;
			if (ffCam.getPosition().z + ffCam.getFrontVector().z < 0) chunkZ += -1;


		blockX = (int) glm::round(ffCam.getPosition().x + ffCam.getFrontVector().x) - Chunk::CHUNK_SIZE * chunkX;
			if (blockX == Chunk::CHUNK_SIZE) blockX = Chunk::CHUNK_SIZE -1;

		blockY = (int)glm::round(ffCam.getPosition().y + ffCam.getFrontVector().y) - Chunk::CHUNK_SIZE * chunkY;
			if (blockY == Chunk::CHUNK_SIZE) blockY = Chunk::CHUNK_SIZE -1;

		blockZ = (int)glm::round(ffCam.getPosition().z + ffCam.getFrontVector().z) - Chunk::CHUNK_SIZE * chunkZ;
			if (blockZ == Chunk::CHUNK_SIZE) blockZ = Chunk::CHUNK_SIZE -1;



		BlockType bt;
		if (!(getBlockFromChunk(chunkmanager, ffCam.getPosition(), ffCam.getFrontVector())->getType() == BlockType_Lava))
		{
			if (chunkmanager.getChunk(chunkX,chunkY,chunkZ)->destructBlock(blockX,blockY,blockZ, bt) )
			{
				inventory.addBlock(bt);
				chunkmanager.addChunkToRebuildList(chunkmanager.getChunk(chunkX,chunkY,chunkZ));
			}
		}		
	}

		std::cout << "currentBlockType : " << currentBlockType << std::endl;

	// CREATE CUBE
	if (rightClick)
	{
		chunkX = (int) glm::round(ffCam.getPosition().x + ffCam.getFrontVector().x) / Chunk::CHUNK_SIZE;
			if (ffCam.getPosition().x + ffCam.getFrontVector().x < 0) chunkX += -1;

		chunkY = (int) glm::round(ffCam.getPosition().y + ffCam.getFrontVector().y) / Chunk::CHUNK_SIZE;
			if (ffCam.getPosition().y + ffCam.getFrontVector().y < 0) chunkY += -1;

		chunkZ = (int) glm::round(ffCam.getPosition().z + ffCam.getFrontVector().z) / Chunk::CHUNK_SIZE;
			if (ffCam.getPosition().z + ffCam.getFrontVector().z < 0) chunkZ += -1;


		blockX = (int) glm::round(ffCam.getPosition().x + ffCam.getFrontVector().x) - Chunk::CHUNK_SIZE * chunkX;
			if (blockX == Chunk::CHUNK_SIZE) blockX = Chunk::CHUNK_SIZE -1;

		blockY = (int)glm::round(ffCam.getPosition().y + ffCam.getFrontVector().y) - Chunk::CHUNK_SIZE * chunkY;
			if (blockY == Chunk::CHUNK_SIZE) blockY = Chunk::CHUNK_SIZE -1;

		blockZ = (int)glm::round(ffCam.getPosition().z + ffCam.getFrontVector().z) - Chunk::CHUNK_SIZE * chunkZ;
			if (blockZ == Chunk::CHUNK_SIZE) blockZ = Chunk::CHUNK_SIZE -1;


		//Faire une fonction qui récupère le choix du bloc type
		BlockType bt = currentBlockType;
		std::cout << "currentBlockType : " << currentBlockType << std::endl;

		if (inventory.getNbBlock(bt) > 0)
		{
			if (!getBlockFromChunk(chunkmanager, ffCam.getPosition(), ffCam.getFrontVector())->isActive() && 
				(	getBlockFromChunk(chunkmanager, ffCam.getPosition(), ffCam.getFrontVector() + glm::vec3(1,0,0))->isActive()
				 ||	getBlockFromChunk(chunkmanager, ffCam.getPosition(), ffCam.getFrontVector() + glm::vec3(-1,0,0))->isActive()
				 ||	getBlockFromChunk(chunkmanager, ffCam.getPosition(), ffCam.getFrontVector() + glm::vec3(0,1,0))->isActive()
				 ||	getBlockFromChunk(chunkmanager, ffCam.getPosition(), ffCam.getFrontVector() + glm::vec3(0,-1,0))->isActive()
				 ||	getBlockFromChunk(chunkmanager, ffCam.getPosition(), ffCam.getFrontVector() + glm::vec3(0,0,1))->isActive()
				 ||	getBlockFromChunk(chunkmanager, ffCam.getPosition(), ffCam.getFrontVector() + glm::vec3(0,0,-1))->isActive()
				 ))
			{
				if (chunkmanager.getChunk(chunkX,chunkY,chunkZ)->constructBlock(blockX,blockY,blockZ, bt) )
				{
					inventory.deleteBlock(bt);
					chunkmanager.addChunkToRebuildList(chunkmanager.getChunk(chunkX,chunkY,chunkZ));
				}
			}
		}
	}

};