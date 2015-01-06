#include <GL/glew.h>
#include <iostream>
#include <unistd.h>

#include <stdlib.h>
#include <time.h>

#include <glimac/SDLWindowManager.hpp>
#include <SDL2/SDL_mixer.h>
#include <glimac/Program.hpp>
#include <glimac/Image.hpp>
#include <glimac/glm.hpp>
#include <glimac/CustomProgram.hpp>
#include <glimac/FreeFlyCamera.hpp>
#include <glimac/Torch.hpp>
#include <glimac/Geometry.hpp>
#include <glimac/Sound.hpp>
#include <glimac/tiny_obj_loader.h>

#include "voxel_engine/Chunk.hpp"
#include "voxel_engine/ChunkManager.hpp"
#include "physics/Event_manager.hpp"
#include "voxel_engine/Block.hpp"
#include "Skybox.hpp"
#include "Light.hpp"
#include "Helmet.hpp"

using namespace glimac;

int main(int argc, char** argv) 
{
	// Initialize SDL and open a window
	SDLWindowManager windowManager("iMineCraft Oui Bonsoir", 0);

	glewExperimental = GL_TRUE;
	// Initialize glew for OpenGL3+ support
	GLenum glewInitError = glewInit();
	if(GLEW_OK != glewInitError) 
	{
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
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//For transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Chargement des shaders
    FilePath applicationPath(argv[0]);

	GeneralProgram gProgram(applicationPath);
	LightsProgram lightsProg(applicationPath);
	HelmetProgram hellProg(applicationPath);
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

	glTexImage2D(GL_TEXTURE_2D,  0,  GL_RGBA,  texturePointer->getWidth(),  
					texturePointer->getHeight(),  0,  GL_RGBA,  GL_FLOAT,  texturePointer->getPixels());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D,  0);

	std::string savesFolder = "bin/assets/saves/";
	ChunkManager chunkmanager;
	chunkmanager.initialize(savesFolder);

	//Initialisation camera freefly
	FreeFlyCamera ffCam;
	chunkmanager.update(ffCam.getPosition(), ffCam.getFrontVector());
	
	ffCam.setPosition(glm::vec3(0,chunkmanager.getNoiseValue(0,0)+5,0));


	float angleYCurrent = 0;

	int crouch = 0;
	float breakCube = 0;

	const float CAMERA_ROT_FACTOR = 0.05f;

	int max_fps = 60;
	float lastTime = windowManager.getTime();
	float lastTime2 = windowManager.getTime();
	int nbFrames = 0;
	
	Skybox skybox;
		skybox.init(skyProgram);

	Geometry lander;
		lander.init(geoProgram, lander, "Lander.obj", true, "Lander.png");

	Geometry crowbar;
		crowbar.init(geoProgram, crowbar, "crowbar.obj", true, "metal01.jpg");

	srand(time(NULL));

	Light sun = Light(glm::vec3(1,1,1), glm::vec3(-0.5,0.5,-0.5));

	Torch torch(glm::vec3(3,glm::round(chunkmanager.getNoiseValue(3,4))+5,4));

	// define current BlockType
	BlockType currentBlockType = BlockType_Earth;

	Inventory invent;

	Helmet helmet;

	// Application loop:
	bool done = false;
	while(!done) 
	{
	
		chunkmanager.update(ffCam.getPosition(), ffCam.getFrontVector());
		
		// Event loop:
		event_manager(windowManager,ffCam,angleYCurrent,CAMERA_ROT_FACTOR,done,chunkmanager, invent, crouch, currentBlockType, 
						mix_chunk, breakCube);

		// Measure speed
		float currentTime = windowManager.getTime();
		nbFrames++;
		if ( currentTime - lastTime >= 1.0 )
		{ 
		    //std::cout << "fps : " << nbFrames << std::endl;
		    nbFrames = 0;
		    lastTime += 1.0;
		}

		if (currentTime - lastTime2 < (1.f/max_fps) && currentTime - lastTime2 > 0)
		{
			usleep( (unsigned int)(((1.f/max_fps) - (currentTime - lastTime2))*2000000) ) ;
		}

		lastTime2 = currentTime;

		/*********************************
		 * HERE SHOULD COME THE RENDERING CODE
		 *********************************/

		glm::mat4 viewMatrix = ffCam.getViewMatrix();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		skyProgram.m_Program.use();
			skybox.draw(skyProgram, viewMatrix);

		geoProgram.m_Program.use();
			lander.draw(geoProgram, lander, viewMatrix, glm::vec3(4,glm::round(chunkmanager.getNoiseValue(4,8))+0.5,8), glm::vec3(0.5, 0.5, 0.5), 0, glm::vec3(1.0, 1.0, 1.0));
			crowbar.drawCrowbar(geoProgram, crowbar, ffCam, breakCube);

		lightsProg.m_Program.use();
			torch.translatePos(glm::sin(windowManager.getTime()) * glm::vec3(0,0.02,0));
			torch.computeLight(lightsProg, ffCam);
			sun.initMaterial(glm::vec3(1,1,1), glm::vec3(1,1,1), 2.f);
			sun.computeLight(lightsProg, ffCam.getViewMatrix());

			chunkmanager.render(lightsProg, ffCam.getViewMatrix());

		gProgram.m_Program.use();
			torch.drawBillboard(gProgram, ffCam);

		hellProg.m_Program.use();
			helmet.setPosition(ffCam.getPosition() + glm::vec3(ffCam.getFrontVector().x * 0.15, ffCam.getFrontVector().y * 0.15, ffCam.getFrontVector().z * 0.15 ));
			helmet.drawBillboard(hellProg, ffCam);

		// Update the display
		windowManager.swapBuffers();
	}

	// Unload and save the displayed map
	chunkmanager.unloadWorld();
	
	skybox.destruct();
	crowbar.destruct();

	glDeleteTextures(1, &idTexture);
	deletesound(mix_chunk, music);

	return EXIT_SUCCESS;
}
