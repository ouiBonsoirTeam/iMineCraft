#pragma once

#include <vector>
#include <cmath>
#include <glimac/SDLWindowManager.hpp>
#include <glimac/FreeFlyCamera.hpp>
#include <glimac/Torch.hpp>
#include "../voxel_engine/Chunk.hpp"
#include "../voxel_engine/ChunkManager.hpp"
#include "../voxel_engine/Block.hpp"
#include "../inventory.hpp"


Block* getBlockFromChunk(ChunkManager& chunkmanager, glm::vec3 position, glm::vec3 direction)
{
	int chunkX = glm::round(position.x + direction.x) / Chunk::CHUNK_SIZE;
		if (glm::round(position.x + direction.x) < 0) chunkX += -1;

	int chunkY = glm::round(position.y + direction.y) / Chunk::CHUNK_SIZE;
		if (glm::round(position.y + direction.y) < 0) chunkY += -1;

	int chunkZ = glm::round(position.z + direction.z) / Chunk::CHUNK_SIZE;
		if (glm::round(position.z + direction.z) < 0) chunkZ += -1;


	int blockX =  (glm::round(position.x + direction.x) - Chunk::CHUNK_SIZE * chunkX);
		if (blockX == Chunk::CHUNK_SIZE) blockX = Chunk::CHUNK_SIZE -1;

	int blockY =  (glm::round(position.y + direction.y) - Chunk::CHUNK_SIZE * chunkY);
		if (blockY == Chunk::CHUNK_SIZE) blockY = Chunk::CHUNK_SIZE -1;

	int blockZ =  (glm::round(position.z + direction.z) - Chunk::CHUNK_SIZE * chunkZ);
		if (blockZ == Chunk::CHUNK_SIZE) blockZ = Chunk::CHUNK_SIZE -1;




	return &chunkmanager.getChunk(chunkX, chunkY, chunkZ)->getBlocks()[blockX][blockY][blockZ];
}


void event_manager(SDLWindowManager& windowManager,
				   FreeFlyCamera& ffCam,
				   float& angleYCurrent,
				   float CAMERA_ROT_FACTOR,
				   bool& done,
				   ChunkManager& chunkmanager,
				   Inventory& inventory,
				   int& crouch,
				   BlockType& currentBlockType,
				   std::vector<Mix_Chunk*> mix_chunk,
				   float &breackCube,
				   Torch& torch){

	// INIT


	const float INERTIA_FACTOR = 1.004;
	const float INERTIA_JUMP_FACTOR = 1.02;


	float gravityFactor = 0.05f;
	float playerSpeed = 0.05f;


	glm::vec3 velocity=glm::vec3(0,0,0);

	bool leftClick = false;
	bool rightClick = false;

	float angleX = 0;
	float angleY = 0;


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

			if (e.key.keysym.sym == SDLK_e)
			{
				torch.setPosition(ffCam.getPosition()+ffCam.getFrontVector()*2.f);
			}


		}
		if (e.type == SDL_KEYUP)
		{
			if (e.key.keysym.sym == SDLK_AMPERSAND)
			{
				if(Mix_Playing(0) == 0)
				{
					Mix_PlayChannelTimed(17,mix_chunk[6],0, 450);
				}
				currentBlockType=BlockType_Wood;
			}
			if (e.key.keysym.sym == 233) //é
			{
				if(Mix_Playing(0) == 0)
				{
					Mix_PlayChannelTimed(1,mix_chunk[6],0, 450);
				}
				currentBlockType=BlockType_Rock;
			}
			if (e.key.keysym.sym == SDLK_QUOTEDBL)
			{
				if(Mix_Playing(0) == 0)
				{
					Mix_PlayChannelTimed(2,mix_chunk[6],0, 450);
				}
				currentBlockType=BlockType_Earth;
			}
			if (e.key.keysym.sym == SDLK_QUOTE)
			{
				if(Mix_Playing(0) == 0)
				{
					Mix_PlayChannelTimed(3,mix_chunk[6],0, 450);
				}
				currentBlockType=BlockType_Grass;
			}
			if (e.key.keysym.sym == SDLK_LEFTPAREN)
			{
				if(Mix_Playing(0) == 0)
				{
					Mix_PlayChannelTimed(4,mix_chunk[6],0, 450);
				}
				currentBlockType=BlockType_1st_Snow;
			}
			if (e.key.keysym.sym == SDLK_MINUS || e.key.keysym.sym == 167)
			{
				if(Mix_Playing(0) == 0)
				{
					Mix_PlayChannelTimed(5,mix_chunk[6],0, 450);
				}
				currentBlockType=BlockType_Snow;
			}
			if (e.key.keysym.sym == 232) //è
			{
				if(Mix_Playing(0) == 0)
				{
					Mix_PlayChannelTimed(6,mix_chunk[6],0, 450);
				}
				currentBlockType=BlockType_Ice;
			}	

			if (e.key.keysym.sym == SDLK_LCTRL) 
			{
				if(!(getBlockFromChunk(chunkmanager, ffCam.getPosition(), glm::vec3(0,1.5,0))->isActive()) && crouch == 1)
				{
					ffCam.slide(glm::vec3(0,+1,0));
					crouch = 0;	
				}
			}
		}

		//souris
		if (e.type == SDL_MOUSEMOTION)
		{
			angleX -= e.motion.xrel * CAMERA_ROT_FACTOR;
			angleY -= e.motion.yrel * CAMERA_ROT_FACTOR;
			angleYCurrent -= e.motion.yrel * CAMERA_ROT_FACTOR;
			angleYCurrent = std::min(87.0f, std::max(-87.0f, angleYCurrent)); //pour pas passer sa tête entre ses jambes
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
	if (angleYCurrent < 87 && angleYCurrent >-87)
		ffCam.rotateUp(angleY);
	if (angleYCurrent == 87)
		ffCam.setTheta(87);
	if (angleYCurrent == -87)
		ffCam.setTheta(-87);
	
			
	if(windowManager.isKeyPressed(SDLK_z)) 
	{
		ffCam.setInertia(ffCam.getFrontVector()*playerSpeed);
		if(windowManager.isKeyPressed(SDLK_LSHIFT)  && crouch ==0) 
		{
			//run
			if(Mix_Playing(0) == 0 && getBlockFromChunk(chunkmanager, ffCam.getPosition(), glm::vec3(0, velocity.y - 1.5, 0))->isActive())
			{
				Mix_PlayChannelTimed(7,mix_chunk[0],0, 450);
			}
			//jetpack avec shift
			else if(Mix_Playing(0) == 0)
			{
				Mix_PlayChannelTimed(18,mix_chunk[5],0, 450);
			}
			velocity+=glm::vec3(ffCam.getFrontVector().x*playerSpeed*3.f,0,ffCam.getFrontVector().z*playerSpeed*3.f);

		}
		if(crouch ==1) 
		{
			velocity+=glm::vec3(ffCam.getFrontVector().x*playerSpeed*0.3f,0,ffCam.getFrontVector().z*playerSpeed*0.3f);
		}
		else velocity+=glm::vec3(ffCam.getFrontVector().x*playerSpeed,0,ffCam.getFrontVector().z*playerSpeed);
	}
	
	else if(windowManager.isKeyPressed(SDLK_s)) 
	{
		if(crouch ==1) 
		{
			velocity+=glm::vec3(ffCam.getFrontVector().x*(-playerSpeed*0.2f),0,ffCam.getFrontVector().z*(-playerSpeed*0.2f));
		}
		else 
		{
		velocity+=glm::vec3(ffCam.getFrontVector().x*(-playerSpeed*0.5f),0,ffCam.getFrontVector().z*(-playerSpeed*0.5f));
		ffCam.setInertia(ffCam.getFrontVector()*(-playerSpeed));
		}
	}


	if(windowManager.isKeyPressed(SDLK_q)) 
	{
		if(crouch ==1) 
		{
			velocity+=ffCam.getLeftVector()*playerSpeed*0.3f;
		}
		else 
		{
			velocity+=ffCam.getLeftVector()*playerSpeed;
			ffCam.setInertia(ffCam.getLeftVector()*playerSpeed);
		}
	}

	else if(windowManager.isKeyPressed(SDLK_d)) 
	{
		if(crouch ==1) 
		{
			velocity+=ffCam.getLeftVector()*(-playerSpeed*0.3f);
		}
		else
		{
		velocity+=ffCam.getLeftVector()*(-playerSpeed);
		ffCam.setInertia(ffCam.getLeftVector()*(-playerSpeed));
		}
	}


	if(windowManager.isKeyPressed(SDLK_SPACE)  && crouch ==0) 
	{
		//jetpack
		if(Mix_Playing(0) == 0)
		{
			Mix_PlayChannelTimed(8,mix_chunk[5],0, 450);
		}
		velocity+=glm::vec3(0,1,0)*(2.f*playerSpeed);
		ffCam.setJumpInertia(glm::vec3(0,1,0)*(2.f*playerSpeed));
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
	if((getBlockFromChunk(chunkmanager, ffCam.getPosition(), glm::vec3(velocity.x + 0.1, -1.45, 0))->isActive() && crouch == 0)
		|| getBlockFromChunk(chunkmanager, ffCam.getPosition(), glm::vec3(velocity.x + 0.1, -0.45, 0))->isActive() )
	{
		countCollision += 1;
		if(windowManager.isKeyPressed(SDLK_z))
		{
			velocity=glm::vec3(0,velocity.y,ffCam.getFrontVector().z*playerSpeed);
		}
		else velocity=glm::vec3(0,velocity.y,0);

		if(getBlockFromChunk(chunkmanager, ffCam.getPosition(), glm::vec3(velocity.x + 0.1, -1, 0))->isActive())
			ffCam.setInertia(glm::vec3(0,0,0));

		if ( ffCam.getPosition().x-floor(ffCam.getPosition().x) > 0.4  && ffCam.getPosition().x-floor(ffCam.getPosition().x) < 0.5)
			ffCam.setPosition(glm::vec3(floor(ffCam.getPosition().x)+0.4,ffCam.getPosition().y,ffCam.getPosition().z));
	}


	// -x
	if((getBlockFromChunk(chunkmanager, ffCam.getPosition(), glm::vec3(velocity.x - 0.1, -1.45, 0))->isActive() && crouch == 0)
		|| getBlockFromChunk(chunkmanager, ffCam.getPosition(), glm::vec3(velocity.x - 0.1, -0.45, 0))->isActive() )
	{
		countCollision += 1;
		if(windowManager.isKeyPressed(SDLK_z))
		{
			velocity=glm::vec3(0,velocity.y,ffCam.getFrontVector().z*playerSpeed);
		}
		else velocity=glm::vec3(0,velocity.y,0);

		if(getBlockFromChunk(chunkmanager, ffCam.getPosition(), glm::vec3(velocity.x - 0.1, -1, 0))->isActive())
			ffCam.setInertia(glm::vec3(0,0,0));

		if ( ffCam.getPosition().x-floor(ffCam.getPosition().x) < 0.6  && ffCam.getPosition().x-floor(ffCam.getPosition().x) > 0.5)
			ffCam.setPosition(glm::vec3(floor(ffCam.getPosition().x)+0.6,ffCam.getPosition().y,ffCam.getPosition().z));
	}

	// +z
	if((getBlockFromChunk(chunkmanager, ffCam.getPosition(), glm::vec3(0, -1.45, velocity.z + 0.1))->isActive() && crouch == 0)
		|| getBlockFromChunk(chunkmanager, ffCam.getPosition(), glm::vec3(0, -0.45, velocity.z + 0.1))->isActive())
	{
		countCollision += 1;
		if(windowManager.isKeyPressed(SDLK_z))
		{
			velocity=glm::vec3(ffCam.getFrontVector().x*playerSpeed,velocity.y,0);
		}
		else velocity=glm::vec3(0,velocity.y,0);
		
		if(getBlockFromChunk(chunkmanager, ffCam.getPosition(), glm::vec3(0, -1, velocity.z + 0.1))->isActive())
			ffCam.setInertia(glm::vec3(0,0,0));

		if ( ffCam.getPosition().z-floor(ffCam.getPosition().z) > 0.4  && ffCam.getPosition().z-floor(ffCam.getPosition().z) < 0.5)
			ffCam.setPosition(glm::vec3(ffCam.getPosition().x,ffCam.getPosition().y,floor(ffCam.getPosition().z)+0.4));
	}

	// -z
	if((getBlockFromChunk(chunkmanager, ffCam.getPosition(), glm::vec3(0, -1.45, velocity.z - 0.1))->isActive() && crouch == 0)
		|| getBlockFromChunk(chunkmanager, ffCam.getPosition(), glm::vec3(0, -0.45, velocity.z - 0.1))->isActive())
	{
		countCollision += 1;
		if(windowManager.isKeyPressed(SDLK_z))
		{
			velocity=glm::vec3(ffCam.getFrontVector().x*playerSpeed,velocity.y,0);
		}
		else velocity=glm::vec3(0,velocity.y,0);

		if(getBlockFromChunk(chunkmanager, ffCam.getPosition(), glm::vec3(0, -1, velocity.z - 0.1))->isActive())
			ffCam.setInertia(glm::vec3(0,0,0));

		if ( ffCam.getPosition().z-floor(ffCam.getPosition().z) < 0.6  && ffCam.getPosition().z-floor(ffCam.getPosition().z) > 0.5)
			ffCam.setPosition(glm::vec3(ffCam.getPosition().x,ffCam.getPosition().y,floor(ffCam.getPosition().z)+0.6));
	}

	if(countCollision > 1) velocity=glm::vec3(0,velocity.y,0);


	// sol
	if((getBlockFromChunk(chunkmanager, ffCam.getPosition(), glm::vec3(0, velocity.y - 1.5, 0))->isActive() && crouch == 0)
		|| (getBlockFromChunk(chunkmanager, ffCam.getPosition(), glm::vec3(0, velocity.y - 0.5, 0))->isActive() && crouch == 1))
	{
		
		gravityFactor = 0.00f;
		velocity.y=0;
		ffCam.setJumpInertia(glm::vec3(0,0,0));
		if(!(getBlockFromChunk(chunkmanager, ffCam.getPosition(), glm::vec3(0, velocity.y - 1.5, 0))->getType() == BlockType_Ice))
			ffCam.setInertia(glm::vec3(0,0,0));
		
		
		if(Mix_Playing(0) == 0)
		{
			//grass
			if ( ( windowManager.isKeyPressed(SDLK_z) || windowManager.isKeyPressed(SDLK_s) || windowManager.isKeyPressed(SDLK_q) || windowManager.isKeyPressed(SDLK_d) ) && getBlockFromChunk(chunkmanager, ffCam.getPosition(), glm::vec3(0, velocity.y - 1.5, 0))->getType() == BlockType_Grass)
			{
				Mix_PlayChannelTimed(9,mix_chunk[1],0, 450);
			}
			//earth
			if ( ( windowManager.isKeyPressed(SDLK_z) || windowManager.isKeyPressed(SDLK_s) || windowManager.isKeyPressed(SDLK_q) || windowManager.isKeyPressed(SDLK_d) ) && getBlockFromChunk(chunkmanager, ffCam.getPosition(), glm::vec3(0, velocity.y - 1.5, 0))->getType() == BlockType_Earth)
			{
				Mix_PlayChannelTimed(10,mix_chunk[1],0, 450);
			}
			//first snow
			if ( ( windowManager.isKeyPressed(SDLK_z) || windowManager.isKeyPressed(SDLK_s) || windowManager.isKeyPressed(SDLK_q) || windowManager.isKeyPressed(SDLK_d) ) && getBlockFromChunk(chunkmanager, ffCam.getPosition(), glm::vec3(0, velocity.y - 1.5, 0))->getType() == BlockType_1st_Snow)
			{
				Mix_PlayChannelTimed(11,mix_chunk[2],0, 450);
			}
			//snow
			if ( ( windowManager.isKeyPressed(SDLK_z) || windowManager.isKeyPressed(SDLK_s) || windowManager.isKeyPressed(SDLK_q) || windowManager.isKeyPressed(SDLK_d) ) && getBlockFromChunk(chunkmanager, ffCam.getPosition(), glm::vec3(0, velocity.y - 1.5, 0))->getType() == BlockType_Snow)
			{
				Mix_PlayChannelTimed(12,mix_chunk[2],0, 450);
			}
			//stone
			if ( ( windowManager.isKeyPressed(SDLK_z) || windowManager.isKeyPressed(SDLK_s) || windowManager.isKeyPressed(SDLK_q) || windowManager.isKeyPressed(SDLK_d) ) && getBlockFromChunk(chunkmanager, ffCam.getPosition(), glm::vec3(0, velocity.y - 1.5, 0))->getType() == BlockType_Rock)
			{
				Mix_PlayChannelTimed(13,mix_chunk[3],0, 450);
			}
			//lava
			if (getBlockFromChunk(chunkmanager, ffCam.getPosition(), glm::vec3(0, velocity.y - 1.5, 0))->getType() == BlockType_Lava)
			{
				Mix_PlayChannelTimed(14,mix_chunk[4],0, 450);
			}
			//lava
			if ( ( windowManager.isKeyPressed(SDLK_z) || windowManager.isKeyPressed(SDLK_s) || windowManager.isKeyPressed(SDLK_q) || windowManager.isKeyPressed(SDLK_d) ) && getBlockFromChunk(chunkmanager, ffCam.getPosition(), glm::vec3(0, velocity.y - 1.5, 0))->getType() == BlockType_Ice)
			{
				Mix_PlayChannelTimed(19,mix_chunk[2],0, 450);
			}
		}
	}

	if (!(getBlockFromChunk(chunkmanager, ffCam.getPosition(), glm::vec3(0.1,1.5,0))->isActive()) && crouch == 1 && !windowManager.isKeyPressed(SDLK_LCTRL))
		{
			ffCam.slide(glm::vec3(0,+1,0));
			crouch = 0;
		}



	// MOUVEMENT

	// mouvements
	ffCam.slide(velocity);
	// gravity
	ffCam.moveUp(-gravityFactor);
	// y inertia
	if (ffCam.getJumpInertia().y > 0.005
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


	//std::cout<<"ffCam.getPosition.y : "<<ffCam.getPosition().y-floor(ffCam.getPosition().y)<<std::endl;

	int chunkX = (int) glm::round(ffCam.getPosition().x + ffCam.getFrontVector().x) / Chunk::CHUNK_SIZE;
		if (glm::round(ffCam.getPosition().x) < 0) chunkX += -1;

	int chunkY = (int) glm::round(ffCam.getPosition().y + ffCam.getFrontVector().y) / Chunk::CHUNK_SIZE;
		if (glm::round(ffCam.getPosition().y) < 0) chunkY += -1;

	int chunkZ = (int) glm::round(ffCam.getPosition().z + ffCam.getFrontVector().z) / Chunk::CHUNK_SIZE;
		if (glm::round(ffCam.getPosition().z) < 0) chunkZ += -1;


	int blockX = (int) glm::round(ffCam.getPosition().x + ffCam.getFrontVector().x) - Chunk::CHUNK_SIZE * chunkX;
		if (blockX == Chunk::CHUNK_SIZE) blockX = Chunk::CHUNK_SIZE -1;

	int blockY = (int)glm::round(ffCam.getPosition().y + ffCam.getFrontVector().y) - Chunk::CHUNK_SIZE * chunkY;
		if (blockY == Chunk::CHUNK_SIZE) blockY = Chunk::CHUNK_SIZE -1;

	int blockZ = (int)glm::round(ffCam.getPosition().z + ffCam.getFrontVector().z) - Chunk::CHUNK_SIZE * chunkZ;
		if (blockZ == Chunk::CHUNK_SIZE) blockZ = Chunk::CHUNK_SIZE -1;





	// DESTRUCT CUBE
	if (leftClick)
	{
		breackCube = -2.f;

		chunkX = (int) glm::round(ffCam.getPosition().x + ffCam.getFrontVector().x) / Chunk::CHUNK_SIZE;
			if (glm::round(ffCam.getPosition().x + ffCam.getFrontVector().x) < 0) chunkX += -1;

		chunkY = (int) glm::round(ffCam.getPosition().y + ffCam.getFrontVector().y) / Chunk::CHUNK_SIZE;
			if (glm::round(ffCam.getPosition().y + ffCam.getFrontVector().y) < 0) chunkY += -1;

		chunkZ = (int) glm::round(ffCam.getPosition().z + ffCam.getFrontVector().z) / Chunk::CHUNK_SIZE;
			if (glm::round(ffCam.getPosition().z + ffCam.getFrontVector().z) < 0) chunkZ += -1;


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

				// Test if the deleted cube is in a chunk fronter, then add the other chunk to the rebuild list
				// X
				if(blockX == 0)
					chunkmanager.addChunkToRebuildList(chunkmanager.getChunk(chunkX - 1,chunkY,chunkZ));
				else if(blockX == Chunk::CHUNK_SIZE -1)
					chunkmanager.addChunkToRebuildList(chunkmanager.getChunk(chunkX + 1,chunkY,chunkZ));

				// Y
				if(blockY == 0)
					chunkmanager.addChunkToRebuildList(chunkmanager.getChunk(chunkX,chunkY - 1,chunkZ));
				else if(blockY == Chunk::CHUNK_SIZE -1)
					chunkmanager.addChunkToRebuildList(chunkmanager.getChunk(chunkX,chunkY + 1,chunkZ));

				// Z
				if(blockZ == 0)
					chunkmanager.addChunkToRebuildList(chunkmanager.getChunk(chunkX,chunkY,chunkZ - 1));
				else if(blockZ == Chunk::CHUNK_SIZE -1)
					chunkmanager.addChunkToRebuildList(chunkmanager.getChunk(chunkX,chunkY,chunkZ + 1));


				//delete cube
				if(Mix_Playing(0) == 0)
				{
					Mix_PlayChannelTimed(15,mix_chunk[8],0, 450);
				}
			}
		}
	}


	// CREATE CUBE
	if (rightClick)
	{
		chunkX = (int) glm::round(ffCam.getPosition().x + ffCam.getFrontVector().x) / Chunk::CHUNK_SIZE;
			if (glm::round(ffCam.getPosition().x + ffCam.getFrontVector().x) < 0) chunkX += -1;

		chunkY = (int) glm::round(ffCam.getPosition().y + ffCam.getFrontVector().y) / Chunk::CHUNK_SIZE;
			if (glm::round(ffCam.getPosition().y + ffCam.getFrontVector().y) < 0) chunkY += -1;

		chunkZ = (int) glm::round(ffCam.getPosition().z + ffCam.getFrontVector().z) / Chunk::CHUNK_SIZE;
			if (glm::round(ffCam.getPosition().z + ffCam.getFrontVector().z) < 0) chunkZ += -1;


		blockX = (int) glm::round(ffCam.getPosition().x + ffCam.getFrontVector().x) - Chunk::CHUNK_SIZE * chunkX;
			if (blockX == Chunk::CHUNK_SIZE) blockX = Chunk::CHUNK_SIZE -1;

		blockY = (int)glm::round(ffCam.getPosition().y + ffCam.getFrontVector().y) - Chunk::CHUNK_SIZE * chunkY;
			if (blockY == Chunk::CHUNK_SIZE) blockY = Chunk::CHUNK_SIZE -1;

		blockZ = (int)glm::round(ffCam.getPosition().z + ffCam.getFrontVector().z) - Chunk::CHUNK_SIZE * chunkZ;
			if (blockZ == Chunk::CHUNK_SIZE) blockZ = Chunk::CHUNK_SIZE -1;



		BlockType bt = currentBlockType;

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
					//add cube
					if(Mix_Playing(0) == 0)
					{
						Mix_PlayChannelTimed(16,mix_chunk[7],0, 450);
					}
					inventory.deleteBlock(bt);
					chunkmanager.addChunkToRebuildList(chunkmanager.getChunk(chunkX,chunkY,chunkZ));
				}
			}
		}
	}

};