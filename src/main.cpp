#include <GL/glew.h>
#include <iostream>
#include <glimac/SDLWindowManager.hpp>
#include <glimac/Program.hpp>
#include <glimac/Image.hpp>
#include <glimac/glm.hpp>
#include <glimac/Sphere.hpp>
#include <glimac/CustomProgram.hpp>
#include <glimac/FreeFlyCamera.hpp>
#include <glimac/Torch.hpp>

#include "voxel_engine/Chunk.hpp"
#include "physics/Event_manager.hpp"

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

	/*********************************
	 * HERE SHOULD COME THE INITIALIZATION CODE
	 *********************************/

	glEnable(GL_DEPTH_TEST);

	FilePath applicationPath(argv[0]);

	GeneralProgram gProgram(applicationPath);
	pointLightProgram lProgram(applicationPath);
	gProgram.m_Program.use();

	//Load texture
	std::unique_ptr<Image> texturePointer;
	texturePointer = loadImage("../iMineCraft/assets/textures/ambiant_occlusion_17x16_trans.png");
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


	float lastTime = windowManager.getTime();
	int nbFrames = 0;

	// Application loop:
	bool done = false;
	while(!done) {
		// Event loop:
		event_manager(windowManager,ffCam,angleX,angleY,angleYfinal,CAMERA_ROT_FACTOR,done,chunk);

				

		// Measure speed
		float currentTime = windowManager.getTime();
		nbFrames++;
		if ( currentTime - lastTime >= 1.0 ){ // If last prinf() was more than 1 sec ago
		    // printf and reset timer
		    std::cout << "fps : " << nbFrames << std::endl;
		    nbFrames = 0;
		    lastTime += 1.0;
		    }


		/*********************************
		 * HERE SHOULD COME THE RENDERING CODE
		 *********************************/

		glm::mat4 matrixV = ffCam.getViewMatrix();
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		chunk.render(gProgram, matrixV, idTexture);

		// Update the display
		windowManager.swapBuffers();
	}

	return EXIT_SUCCESS;
}
