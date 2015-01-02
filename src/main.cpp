#include <GL/glew.h>
#include <iostream>
#include <unistd.h>

#include <stdlib.h>
#include <time.h>

#include <glimac/SDLWindowManager.hpp>
#include <glimac/Program.hpp>
#include <glimac/Image.hpp>
#include <glimac/glm.hpp>
#include <glimac/Sphere.hpp>
#include <glimac/CustomProgram.hpp>
#include <glimac/FreeFlyCamera.hpp>
#include <glimac/Torch.hpp>
#include <glimac/Geometry.hpp>

#include "voxel_engine/Chunk.hpp"
#include "physics/Event_manager.hpp"
#include "Skybox.hpp"

#include "../glimac/src/tiny_obj_loader.h"

using namespace glimac;

/*********************************
 *
 * Ce fichier est un exemple d'application minimale utilisant shaders et textures
 * Le but est pour vous de comprendre quel chemin de fichier utiliser pour charger vos shaders et assets
 *
 * Au moment de la compilation, tous les shaders (.glsl) du repertoire du même nom sont copiés dans le repertoire
 * "shaders" à coté de l'executable. Ainsi pour obtenir le chemin vers le shader "tex2D.vs.glsl", on utilise
 * le chemin vers notre executable, contenu dans argv[0]:
 *
 * FilePath applicationPath(argv[0]);
 *
 * Le chemin du shader à charger est alors: applicationPath.dirPath() + "/shaders/tex2D.vs.glsl"
 *
 * De la même manière, tous les fichiers (sans contrainte d'extension) du repertoire assets sont copiés dans
 * le repertoire "assets" à coté de l'executable. Pour obtenir le chemin vers la texture "textures/triforce.png" on fait:
 *
 * applicationPath.dirPath() + "/assets/textures/triforce.png"
 *
 * easy peasy.
 *
 *********************************/

struct Vertex {
	glm::vec2 position;
	glm::vec2 texCoords;
};

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

	glEnable(GL_DEPTH_TEST);

	/*********************************
	 * HERE SHOULD COME THE INITIALIZATION CODE
	 *********************************/

	//Chargement des shaders
    FilePath applicationPath(argv[0]);

	//comme P ne change jamais on peut la declarer a l'initialisation
	glm::mat4 matrixP = glm::perspective(glm::radians(70.f), 800.f/600.f, 0.1f, 100.f);

	GeneralProgram gProgram(applicationPath);
	pointLightProgram lProgram(applicationPath);
	SkyboxProgram skyProgram(applicationPath);
	GeometryProgram geoProgram(applicationPath);

	// gProgram.m_Program.use();
	// lProgram.m_Program.use();
	// skyProgram.m_Program.use();

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

	Chunk chunk;
	chunk.init();

	chunk.createMesh();

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

	// // make me a torch
	// Torch torch;
	srand(time(NULL));
	// Application loop:
	bool done = false;
	while(!done) {
		// Event loop:
		event_manager(windowManager,ffCam,angleX,angleY,angleYfinal,CAMERA_ROT_FACTOR,done,chunk);

				

		// Measure speed
		float currentTime = windowManager.getTime();
		nbFrames++;
		if ( currentTime - lastTime >= 1.0 )
		{ 
		    std::cout << "fps : " << nbFrames << std::endl;
		    nbFrames = 0;
		    lastTime += 1.0;
		}

		//std::cout<<"currentTime - lastTime2 : "<< (currentTime - lastTime2) << std::endl;
		//std::cout<<"1/max_fps : "<< (1.f/max_fps) << std::endl;
		//std::cout<<"diff : "<< (1.f/max_fps) - (currentTime - lastTime2) << std::endl;
		if (currentTime - lastTime2 < (1.f/max_fps) && currentTime - lastTime2 > 0)
		{
			//usleep( (unsigned int)(((1.f/max_fps) - (currentTime - lastTime2))*2000000) ) ;
			//std::cout<<"zizi"<<std::endl;
		}
		lastTime2 = currentTime;


		/*********************************
		 * HERE SHOULD COME THE RENDERING CODE
		 *********************************/

		glm::mat4 viewMatrix = ffCam.getViewMatrix();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		skyProgram.m_Program.use();
			skybox.draw(skyProgram, viewMatrix);

		// lProgram.m_Program.use();
			// torch.draw(lProgram, viewMatrix);

		geoProgram.m_Program.use();
			//veget1.draw(geoProgram, veget1, viewMatrix, glm::vec3(10, 0, 5), glm::vec3(0.01, 0.01, 0.01), 0, glm::vec3(1.0, 1.0, 1.0));
			// for (int i = 0; i < 20; ++i)
			// {
			// 	veget1.draw(geoProgram, veget1, viewMatrix, glm::vec3((std::rand() % 100) + 1, 0, (std::rand() % 100) + 1), glm::vec3(0.01, 0.01, 0.01));
			// }
			//lander.draw(geoProgram, lander, viewMatrix, glm::vec3(20, 0, 20), glm::vec3(1, 1, 1));
			

			// crowbar.draw(geoProgram, crowbar, viewMatrix, ffCam.getPosition(), glm::vec3(-0.3, -0.3, 0.5), glm::vec3(0.06, 0.06, 0.06), 44.4, glm::vec3(0.2, 1.0, 0.0));
			// crowbar.draw(geoProgram, crowbar, viewMatrix, glm::vec3(ffCam.getPosition()+glm::vec3(-0.3, -0.3, 0.5)), glm::vec3(0.06, 0.06, 0.06), 1, glm::vec3(ffCam.getFrontVector()+glm::vec3(0.2, 1.0, 0.0)));
			crowbar.draw(geoProgram, crowbar, viewMatrix, glm::vec3(ffCam.getPosition()), glm::vec3(-1, -1, 5), glm::vec3(0.06, 0.06, 0.06), 1, glm::vec3(ffCam.getFrontVector()));
			//crowbar.draw(geoProgram, crowbar, viewMatrix, glm::vec3(ffCam.getPosition()), glm::vec3(-1, -1, 5), glm::vec3(0.06, 0.06, 0.06), ffCam.getFrontVector().x, glm::vec3(ffCam.getFrontVector().x, ffCam.getLeftVector().y, ffCam.getUpVector().z));
			// crowbar.draw(geoProgram, crowbar, viewMatrix, ffCam.getFrontVector(), glm::vec3(0, 0, 1), glm::vec3(0.06, 0.06, 0.06), 44.4, glm::vec3(0.2, 1.0, 0.0));
			

			// engineblock.draw(geoProgram, engineblock, viewMatrix, glm::vec3(50, 0, 50), glm::vec3(1, 1, 1));
			//screws.draw(geoProgram, screws, viewMatrix, glm::vec3(5, 5, 5), glm::vec3(0.001, 0.001, 0.001), windowManager.getTime(), glm::vec3(0.0, 1.0, 0.0));


		gProgram.m_Program.use();
			chunk.render(gProgram, viewMatrix, idTexture);

		// Update the display
		windowManager.swapBuffers();

	}

	skybox.destruct();
	crowbar.destruct();

	return EXIT_SUCCESS;
}
