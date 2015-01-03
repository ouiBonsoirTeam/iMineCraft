#include <GL/glew.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <unistd.h>

#include <stdlib.h>
#include <time.h>

#include <glimac/SDLWindowManager.hpp>
#include <SDL2/SDL_mixer.h>
#include <glimac/Program.hpp>
#include <glimac/Image.hpp>
#include <glimac/glm.hpp>
#include <glimac/Sphere.hpp>
#include <glimac/CustomProgram.hpp>
#include <glimac/FreeFlyCamera.hpp>
#include <glimac/Torch.hpp>
#include <glimac/Geometry.hpp>
#include <glimac/Sound.hpp>


#include "voxel_engine/Chunk.hpp"
#include "voxel_engine/ChunkManager.hpp"
#include "physics/Event_manager.hpp"
#include "voxel_engine/Block.hpp"
#include "Skybox.hpp"

#include "../glimac/src/tiny_obj_loader.h"

using namespace glimac;



int main(int argc, char** argv) {
	// Initialize SDL and open a window
	SDLWindowManager windowManager("iMineCraft Oui Bonsoir", 0);

	// const unsigned int windowWidth = windowManager.getWindow()->width;
	// const unsigned int windowHeight = windowManager.getWindow()->height;
	// const unsigned int windowWidth = 800;
 //    const unsigned int windowHeight = 600;

	glewExperimental = GL_TRUE;
	// Initialize glew for OpenGL3+ support
	GLenum glewInitError = glewInit();
	if(GLEW_OK != glewInitError) {
		std::cerr << glewGetErrorString(glewInitError) << std::endl;
		return EXIT_FAILURE;
	}

	std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLEW Version : " << glewGetString(GLEW_VERSION) << std::endl;

	/*********************************
	 * HERE SHOULD COME THE INITIALIZATION CODE
	 *********************************/



	Mix_Music *music = nullptr;
	std::vector<Mix_Chunk*> mix_chunk = initsound(music);

	glEnable(GL_DEPTH_TEST);

	//Chargement des shaders
    FilePath applicationPath(argv[0]);


	GeneralProgram gProgram(applicationPath);
	pointLightProgram lProgram(applicationPath);
	SkyboxProgram skyProgram(applicationPath);
	GeometryProgram geoProgram(applicationPath);

	
	//Load texture
	std::unique_ptr<Image> texturePointer;
	texturePointer = loadImage("../iMineCraft/assets/textures/glass_1024.png");
	if(texturePointer == NULL)
	{
		std::cerr << "Error while charging texture." << std::endl;
	}

	GLuint idTexture;
	glGenTextures(1, &idTexture);
	glBindTexture(GL_TEXTURE_2D,  idTexture);
	std::cout << "Bison" << std::endl;
	glTexImage2D(GL_TEXTURE_2D,  0,  GL_RGBA,  texturePointer->getWidth(),  
					texturePointer->getHeight(),  0,  GL_RGBA,  GL_FLOAT,  texturePointer->getPixels());
	std::cout << "Rat" << std::endl;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D,  0);

	

	// Initialisation chunkmanager
	ChunkManager chunkmanager;
	chunkmanager.initialize("bin/assets/saves");


	//Initialisation camera freefly
	FreeFlyCamera ffCam;
	chunkmanager.update(ffCam.getPosition(), ffCam.getFrontVector());
	
	ffCam.setPosition(glm::vec3(0,chunkmanager.getNoiseValue(0,0)+5,0));


	//initialisation angle
	float angleX = 0;
	float angleY = 0;
	float angleYfinal = 0;

	int crouch = 0;

	const float CAMERA_ROT_FACTOR = 0.05f;

	int max_fps = 60;
	float lastTime = windowManager.getTime();
	float lastTime2 = windowManager.getTime();
	int nbFrames = 0;
	
	//make me a skybox
	Skybox skybox;

	skybox.init(skyProgram);

	Geometry veget1;
	Geometry lander;
	Geometry crowbar;
	Geometry engineblock;
	Geometry screws;
	veget1.init(geoProgram, veget1, "Hyophorbe_lagenicaulis.obj", true, "Hyophorbe_lagenicaulis_dif.jpg");
	lander.init(geoProgram, lander, "Lander.obj", true, "Lander.png");
	crowbar.init(geoProgram, crowbar, "crowbar.obj", true, "metal01.jpg");
	engineblock.init(geoProgram, engineblock, "Engine_Block.obj", true, "Engine_Block.tga");
	screws.init(geoProgram, screws, "screw.obj", true, "screws.jpg");


	srand(time(NULL));


	BlockType currentBlockType = BlockType_Earth;
	Inventory invent;


	// Application loop:
	bool done = false;
	while(!done) {
	
		chunkmanager.update(ffCam.getPosition(), ffCam.getFrontVector());
		
		// Event loop:
		event_manager(windowManager,ffCam,angleX,angleY,angleYfinal,CAMERA_ROT_FACTOR,done,chunkmanager, invent, crouch, currentBlockType, 
						mix_chunk);

		// Measure speed
		float currentTime = windowManager.getTime();
		nbFrames++;
		if ( currentTime - lastTime >= 1.0 )
		{ 
		    std::cout << "fps : " << nbFrames << std::endl;
		    std::cout << "invent : " << std::endl;
		    invent.affiche();
		    nbFrames = 0;
		    lastTime += 1.0;
		}

		//std::cout<<"currentTime - lastTime2 : "<< (currentTime - lastTime2) << std::endl;
		//std::cout<<"1/max_fps : "<< (1.f/max_fps) << std::endl;
		//std::cout<<"diff : "<< (1.f/max_fps) - (currentTime - lastTime2) << std::endl;
		if (currentTime - lastTime2 < (1.f/max_fps) && currentTime - lastTime2 > 0)
		{

			usleep( (unsigned int)(((1.f/max_fps) - (currentTime - lastTime2))*2000000) ) ;
		}

		lastTime2 = currentTime;

		/*********************************
		 * HERE SHOULD COME THE RENDERING CODE
		 *********************************/
		
		// glClear(GL_COLOR_BUFFER_BIT);

		glm::mat4 viewMatrix = ffCam.getViewMatrix();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		skyProgram.m_Program.use();
			skybox.draw(skyProgram, viewMatrix);





		geoProgram.m_Program.use();
			veget1.draw(geoProgram, veget1, viewMatrix, glm::vec3(-4,glm::round(chunkmanager.getNoiseValue(-4,-4))+0.5,-4), glm::vec3(0.01, 0.01, 0.01), 0, glm::vec3(1.0, 1.0, 1.0));
			
			// for (int i = 0; i < 20; ++i)
			// {
			// 	veget1.draw(geoProgram, veget1, viewMatrix, glm::vec3((std::rand() % 100) + 1, 0, (std::rand() % 100) + 1), glm::vec3(0.01, 0.01, 0.01));
			// }
			lander.draw(geoProgram, lander, viewMatrix, glm::vec3(4,glm::round(chunkmanager.getNoiseValue(4,8))+0.5,8), glm::vec3(0.5, 0.5, 0.5), 0, glm::vec3(1.0, 1.0, 1.0));
			crowbar.drawCrowbar(geoProgram, crowbar, ffCam);

			//engineblock.draw(geoProgram, engineblock, viewMatrix, glm::vec3(50, 0, 50), glm::vec3(1, 1, 1));
			//screws.draw(geoProgram, screws, viewMatrix, glm::vec3(15, 5, 5), glm::vec3(0.001, 0.001, 0.001), windowManager.getTime(), glm::4ec3(0.0, 1.0, 0.0));

		gProgram.m_Program.use();
		chunkmanager.render(gProgram, ffCam.getViewMatrix());



		// Update the display
		windowManager.swapBuffers();

	}

	skybox.destruct();

	crowbar.destruct();


	deletesound(mix_chunk, music);


	return EXIT_SUCCESS;
}
