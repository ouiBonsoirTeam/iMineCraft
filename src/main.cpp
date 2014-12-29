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
#include <glimac/Geometry.hpp>

#include "voxel_engine/Chunk.hpp"
#include "physics/Event_manager.hpp"
#include "Skybox.hpp"

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

	glEnable(GL_DEPTH_TEST);

	//Chargement des shaders
    FilePath applicationPath(argv[0]);

	//comme P ne change jamais on peut la declarer a l'initialisation
	glm::mat4 matrixP = glm::perspective(glm::radians(70.f), 800.f/600.f, 0.1f, 100.f);

	GeneralProgram gProgram(applicationPath);
	pointLightProgram lProgram(applicationPath);
	SkyboxProgram skyProg(applicationPath);

	// gProgram.m_Program.use();
	// lProgram.m_Program.use();
	// skyProg.m_Program.use();

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
	skybox.init(skyProg);

	Geometry geo;

	//load obj
	if (!geo.loadOBJ("bin/assets/obj/hairball.obj", "bin/assets/obj/mitsuba.mtl", true))
		std::cerr << "Impossible de charger l'objet" << std::endl;

	std::cerr << "tortue" << std::endl;

	//load texture
	std::unique_ptr<Image> texturePointer2;
	texturePointer2 = loadImage("../iMineCraft/assets/textures/default.png");
	if(texturePointer2 == NULL)
	{
		std::cerr << "Error while charging texture." << std::endl;
	}

	std::cerr << "dindon" << std::endl;

	GLuint idTexture2;
	glGenTextures(1, &idTexture2);
	glBindTexture(GL_TEXTURE_2D,  idTexture2);
	glTexImage2D(GL_TEXTURE_2D,  0,  GL_RGBA,  texturePointer2->getWidth(),  
					texturePointer2->getHeight(),  0,  GL_RGBA,  GL_FLOAT,  texturePointer2->getPixels());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D,  0);

	std::cerr << "koala" << std::endl;


	GLuint vbo;
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, geo.getVertexCount() * sizeof(glm::vec3), geo.getVertexBuffer(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	std::cerr << "bison" << std::endl;

	GLuint ibo;
    glGenBuffers(1, &ibo);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, geo.getIndexCount() * sizeof(uint32_t), geo.getIndexBuffer(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);




	const static GLuint VERTEX_ATTR_POSITION = 0;
	const static GLuint VERTEX_ATTR_NORMAL = 1;
	const static GLuint VERTEX_ATTR_TEXTCOORD = 2;

	GLuint vao;
	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);


		glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
		glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
		glEnableVertexAttribArray(VERTEX_ATTR_TEXTCOORD);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Geometry::Vertex), (const GLvoid*)(0* sizeof(GLfloat)));
			glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Geometry::Vertex), (const GLvoid*)(3 * sizeof(GLfloat)));
			glVertexAttribPointer(VERTEX_ATTR_TEXTCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(Geometry::Vertex), (const GLvoid*)(6 * sizeof(GLfloat)));

		glBindBuffer(GL_ARRAY_BUFFER, 0);


		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	std::cerr << "lapin" << std::endl;






	// // make me a torch
	// Torch torch;

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

		skyProg.m_Program.use();
			skybox.draw(skyProg, viewMatrix);

		gProgram.m_Program.use();
			// torch.draw(lProgram, viewMatrix);

		chunk.render(gProgram, viewMatrix, idTexture);


		//render pour l'obj
			glm::mat4 modelMatrix = glm::translate(glm::mat4(1.f), glm::vec3(12,3,12));
			glm::mat4 modelViewMatrix = viewMatrix * modelMatrix;

			// A sortir de la classe : Identique dans tout le programme
			glm::mat4 projMatrix = glm::perspective(glm::radians(70.f), 800.f/600.f, 0.1f, 100.f);

			glm::mat4 modelViewProjMatrix = projMatrix * modelViewMatrix;

			// Normale
			glm::mat4 normalMatrix = glm::transpose(glm::inverse(modelViewMatrix));

			glUniformMatrix4fv(gProgram.uMVMatrix, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
			glUniformMatrix4fv(gProgram.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(modelViewProjMatrix));
			glUniformMatrix4fv(gProgram.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(normalMatrix));

			glBindVertexArray(vao);

			glBindTexture(GL_TEXTURE_2D, idTexture2);
			glUniform1i(gProgram.uTexture, 0);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);


			glDrawElements(GL_TRIANGLES, geo.getIndexCount(), GL_UNSIGNED_INT, 0);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


			glBindTexture(GL_TEXTURE_2D, 0);

			glBindVertexArray(0);


		// Update the display
		windowManager.swapBuffers();

	}

	skybox.destruct();
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);

	return EXIT_SUCCESS;
}
