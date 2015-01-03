#include <GL/glew.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <unistd.h>
#include <glimac/SDLWindowManager.hpp>
#include <SDL2/SDL_mixer.h>
#include <glimac/Program.hpp>
#include <glimac/Image.hpp>
#include <glimac/glm.hpp>
#include <glimac/Sphere.hpp>
#include <glimac/CustomProgram.hpp>
#include <glimac/FreeFlyCamera.hpp>
#include <glimac/Torch.hpp>
#include <glimac/Sound.hpp>

#include "voxel_engine/Chunk.hpp"
#include "voxel_engine/ChunkManager.hpp"
#include "physics/Event_manager.hpp"
#include "voxel_engine/Block.hpp"
#include "Skybox.hpp"

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

	// SDL_Window* pWindow = NULL;
	// SDL_Surface* pImage = NULL;
	// int imgFlags = IMG_INIT_JPG|IMG_INIT_PNG|IMG_INIT_TIF;
	// const char* imagePath = "./bin/assets/textures/caisse.jpg";

	// SDL_Rect blitDestination;
	// TTF_Font* pFont = NULL;
 //    const char* fontPath = "./bin/assets/font/callme.ttf";
 //    SDL_Color fontColor = {99, 140, 222};
 //    SDL_Surface* pFontSurface = NULL;
 //    SDL_Rect texteDestination;
 //    IMG_Init(imgFlags);
 //    TTF_Init();

 //    pImage = IMG_Load(imagePath);
 //    if ( pImage == NULL )
 //        {
 //            fprintf(stderr,"Erreur de chargement de l'image %s : %s\n",imagePath,IMG_GetError());
 //        }
 //    pFont = TTF_OpenFont(fontPath,32);
 //    if ( pFont == NULL )
 //        {
 //            fprintf(stderr,"Erreur de chargement de la police %s : %s\n",fontPath,TTF_GetError());
 //        }
 //    pFontSurface = TTF_RenderText_Solid(pFont,"LOL",fontColor);
 //    if ( !pFontSurface )
 //        {
 //            fprintf(stderr,"Erreur pour generer le texte '%s'\n",TTF_GetError());
 //        }
 //    // Une fois l'image chargée, nous pouvons calculer une position relative à celle-ci
 //    // Nous centrons l'image dans la fenêtre
 //    blitDestination.x = windowWidth/2 - pImage->w/2;
 //    blitDestination.y = windowHeight/2 - pImage->h/2;
 //    blitDestination.w = pImage->w;
 //    blitDestination.h = pImage->h;
 //    std::cout << pImage->w << std::endl;
 //    std::cout << pImage->h << std::endl;
 //    // Nous avons notre surface pour le texte donc nous calculons la position relative
 //    // Le texte est à un quart de la hauteur de l'ecran
 //    texteDestination.x = windowWidth/2 - pFontSurface->w/2;
 //    texteDestination.y = windowHeight/4;
 //    texteDestination.w = pFontSurface->w;
 //    texteDestination.h = pFontSurface->h;
 //    std::cout << pFontSurface->w << std::endl;
 //    std::cout << pFontSurface->h << std::endl;



	Mix_Music *music = nullptr;
	std::vector<Mix_Chunk*> mix_chunk = initsound(music);

	glEnable(GL_DEPTH_TEST);

	//Chargement des shaders
    FilePath applicationPath(argv[0]);

	//comme P ne change jamais on peut la declarer a l'initialisation
	//glm::mat4 matrixP = glm::perspective(glm::radians(70.f), 800.f/600.f, 0.1f, 100.f);

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
	std::cout << "Bison" << std::endl;
	glTexImage2D(GL_TEXTURE_2D,  0,  GL_RGBA,  texturePointer->getWidth(),  
					texturePointer->getHeight(),  0,  GL_RGBA,  GL_FLOAT,  texturePointer->getPixels());
	std::cout << "Rat" << std::endl;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D,  0);

	std::string savesFolder = "bin/assets/saves/";
	ChunkManager chunkmanager;
	chunkmanager.initialize(savesFolder);

	//Initialisation camera freefly
	FreeFlyCamera ffCam;
	chunkmanager.update(ffCam.getPosition(), ffCam.getFrontVector());
	
	ffCam.setPosition(glm::vec3(5,chunkmanager.getNoiseValue(5,5)+5,5));

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
	skybox.init(skyProg);

	// // make me a torch
	// Torch torch;
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

		
		chunkmanager.render(gProgram, ffCam.getViewMatrix());

		// SDL_BlitSurface(pImage,NULL,SDL_GetWindowSurface(pWindow),NULL);
		// SDL_BlitSurface(pFontSurface,NULL,SDL_GetWindowSurface(pWindow),NULL);
		// SDL_UpdateWindowSurface(pWindow);

		// Update the display
		windowManager.swapBuffers();

	}

	skybox.destruct();
	glDeleteTextures(1, &idTexture);
	deletesound(mix_chunk, music);
	// SDL_FreeSurface(pFontSurface);
	// TTF_CloseFont(pFont);
	// SDL_FreeSurface(pImage);

	return EXIT_SUCCESS;
}
