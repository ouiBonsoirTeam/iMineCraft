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

	//Chargement des shaders
    FilePath applicationPath(argv[0]);

	//Initialisation camera freefly
	FreeFlyCamera ffCam;

	//initialisation angle
	float angleX = 0;
	float angleY = 0;
	float angleYfinal = 0;

	const float CAMERA_ROT_FACTOR = 0.05f;

	//comme P ne change jamais on peut la declarer a l'initialisation
	glm::mat4 matrixP = glm::perspective(glm::radians(70.f), 800.f/600.f, 0.1f, 100.f);


	GeneralProgram gProgram(applicationPath);
	pointLightProgram lProgram(applicationPath);
	SkyboxProgram skyProg(applicationPath);

	// gProgram.m_Program.use();
	// lProgram.m_Program.use();
	// skyProg.m_Program.use();

	
	//make me a skybox
	Skybox skybox;
	skybox.init(skyProg);


	// make me a torch
	Torch torch(glm::vec3(6, 0, 0));
	Sphere sphere (0.5, 32, 16);

	//création vbo
	GLuint vbo;
	glGenBuffers (1, &vbo);

	//bind le vbo
	glBindBuffer (GL_ARRAY_BUFFER, vbo);

	//buffer data void glBufferData(GLenum  target,  GLsizeiptr  size,  const GLvoid *  data,  GLenum  usage);
	glBufferData (GL_ARRAY_BUFFER, sphere.getVertexCount() * sizeof(ShapeVertex),
						 sphere.getDataPointer(), GL_STATIC_DRAW);

	//debind le vbo
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//creation vao
	GLuint vao;
	glGenVertexArrays(1, &vao);

	//bind le vao 
	glBindVertexArray(vao);

	const GLuint VERTEX_ATTR_POSITION = 0;
	const GLuint VERTEX_ATTR_NORMALE = 1;

	glEnableVertexAttribArray( VERTEX_ATTR_POSITION);
	glEnableVertexAttribArray( VERTEX_ATTR_NORMALE);

	glBindBuffer ( GL_ARRAY_BUFFER, vbo);

	//void glVertexAttribPointer(GLuint  index,  GLint  size,  GLenum  type,  GLboolean  normalized,  GLsizei  stride,  const GLvoid *  pointer);
	glVertexAttribPointer ( VERTEX_ATTR_POSITION, 3, GL_FLOAT,
					GL_FALSE, 1 * sizeof(ShapeVertex), (const GLvoid*) (0*sizeof(GLfloat)));
	glVertexAttribPointer ( VERTEX_ATTR_NORMALE, 3, GL_FLOAT,
					GL_FALSE, 1 * sizeof(ShapeVertex), (const GLvoid*) (3*sizeof(GLfloat)));

	//debind le vbo
	glBindBuffer (GL_ARRAY_BUFFER, 0);
	
	//debind le vao
	glBindVertexArray(0);


	// Application loop:
	bool done = false;
	while(!done) {
		// Event loop:
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
		
		//touche clavier
		if(windowManager.isKeyPressed(SDLK_z)) 
		{
			ffCam.moveFront(0.1f);
		}
		else if(windowManager.isKeyPressed(SDLK_s)) 
		{
			ffCam.moveFront(-0.1f);
		}
		else if(windowManager.isKeyPressed(SDLK_q)) 
		{
			ffCam.moveLeft(0.1f);
		}
		else if(windowManager.isKeyPressed(SDLK_d)) 
		{
			ffCam.moveLeft(-0.1f);
		}

		/*********************************
		 * HERE SHOULD COME THE RENDERING CODE
		 *********************************/

		glm::mat4 viewMatrix = ffCam.getViewMatrix();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		skyProg.m_Program.use();
			skybox.draw(skyProg, viewMatrix);

		lProgram.m_Program.use();
			torch.draw(lProgram, viewMatrix);

			glm::vec3 Kd = glm::vec3(1,1,1);
			glm::vec3 Ks = glm::vec3(1,1,1);
			float shininess = 3.f;

			glUniform3f(lProgram.uKd, Kd.r, Kd.g, Kd.b);
			glUniform3f(lProgram.uKs, Ks.r, Ks.g, Ks.b);
			glUniform1f(lProgram.uShininess, shininess);

			glm::mat4 matrixM = glm::mat4(1.0); 

			glm::mat4 matrixMV = viewMatrix * matrixM;

			//calcul de la matrixViewProjetée
			glm::mat4 matrixMVP = matrixP * matrixMV;

			//calcul de la normal matrix = (MVinverse)Transposée
			glm::mat4 normalMatrix = glm::transpose(glm::inverse(matrixMV));

			glUniformMatrix4fv(lProgram.uMVMatrix, 1, GL_FALSE,  glm::value_ptr(matrixMV));
			glUniformMatrix4fv(lProgram.uMVPMatrix, 1, GL_FALSE,  glm::value_ptr(matrixMVP));
			glUniformMatrix4fv(lProgram.uNormalMatrix, 1, GL_FALSE,  glm::value_ptr(normalMatrix));

		
		//bind du vao
		glBindVertexArray(vao);

		//dessine triangles
		glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());

		//debind du vao
		glBindVertexArray(0);


		// Update the display
		windowManager.swapBuffers();

	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	skybox.destruct();


	return EXIT_SUCCESS;
}
