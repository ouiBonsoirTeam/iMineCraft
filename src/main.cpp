#include <GL/glew.h>
#include <iostream>
#include <unistd.h>
#include <glimac/SDLWindowManager.hpp>
#include <glimac/Program.hpp>
#include <glimac/Image.hpp>
#include <glimac/glm.hpp>
#include <glimac/Sphere.hpp>
#include <glimac/CustomProgram.hpp>
#include <glimac/FreeFlyCamera.hpp>
#include <glimac/Torch.hpp>

#include <glimac/Sound.hpp>
#include "../third-party/api/MAC/include/fmod.h"
#include "../third-party/api/LINUX/include/fmod.h"

#include "voxel_engine/Chunk.hpp"
#include "voxel_engine/ChunkManager.hpp"
#include "physics/Event_manager.hpp"
#include "Skybox.hpp"

using namespace glimac;



int main(int argc, char** argv) {
	// Initialize SDL and open a window
	SDLWindowManager windowManager("iMineCraft Oui Bonsoir", 0);

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
	// Musique du jeu
    FMOD_SYSTEM *music;
    FMOD_System_Create(&music);
    FMOD_System_Init(music, 2, FMOD_INIT_NORMAL, NULL);
    int playing = 0;

	glEnable(GL_DEPTH_TEST);

	//Chargement des shaders
    FilePath applicationPath(argv[0]);

	//comme P ne change jamais on peut la declarer a l'initialisation
	glm::mat4 matrixP = glm::perspective(glm::radians(70.f), 800.f/600.f, 0.1f, 100.f);

	GeneralProgram gProgram(applicationPath);
	pointLightProgram lProgram(applicationPath);
	SkyboxProgram skyProg(applicationPath);

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

	//Initialisation camera freefly
	FreeFlyCamera ffCam;

	// // TEST
	std::string savesFolder = "bin/assets/saves/";
	ChunkManager * chunkmanager = new ChunkManager;
	chunkmanager->initialize(savesFolder);

	//initialisation angle
	float angleX = 0;
	float angleY = 0;
	float angleYfinal = 0;

	const float CAMERA_ROT_FACTOR = 0.05f;

	int max_fps = 60;
	float lastTime = windowManager.getTime();
	float lastTime2 = windowManager.getTime();
	int nbFrames = 0;
	
	//make me a skybox
	Skybox skybox;
	skybox.init(skyProg);

	// // make me a torch
	// Torch torch;

	// Application loop:
	bool done = false;
	while(!done) {
		// Play the music
        if(playing == 0)
        {
            playMusic(music, NULL, "./bin/assets/sound/SOR_TFM_Atmos_65.mp3");
            playing = 1;
        }

		chunkmanager->update(ffCam.getPosition(), ffCam.getFrontVector());
		
		// Event loop:
		event_manager(windowManager,ffCam,angleX,angleY,angleYfinal,CAMERA_ROT_FACTOR,done,*chunkmanager->getChunk(0,0,0));			

		// Measure speed
		float currentTime = windowManager.getTime();
		nbFrames++;
		if ( currentTime - lastTime >= 1.0 )
		{ 
		    std::cout << "fps : " << nbFrames << std::endl;
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
		
		// glClear(GL_COLOR_BUFFER_BIT);

		glm::mat4 viewMatrix = ffCam.getViewMatrix();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		skyProg.m_Program.use();
			skybox.draw(skyProg, viewMatrix);

		gProgram.m_Program.use();
			// torch.draw(lProgram, viewMatrix);

		
		chunkmanager->render(gProgram, ffCam.getViewMatrix());

		// Update the display
		windowManager.swapBuffers();

	}

	skybox.destruct();
	delete chunkmanager;
	glDeleteTextures(1, &idTexture);

	return EXIT_SUCCESS;
}
