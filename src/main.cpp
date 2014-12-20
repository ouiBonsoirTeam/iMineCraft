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

#include <glimac/glhelper.hpp>
#include <glimac/misc.hpp>

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



	std::cerr << "debut" << std::endl;

	/*********************************
	 * HERE SHOULD COME THE INITIALIZATION CODE
	 *********************************/
	// set up the cube map texture
	// SDL_Surface *xpos = IMG_Load("/assets/skybox/xpos.png");	SDL_Surface *xneg = IMG_Load("/assets/skybox/xneg.png");
	// SDL_Surface *ypos = IMG_Load("/assets/skybox/ypos.png");	SDL_Surface *yneg = IMG_Load("/assets/skybox/yneg.png");
	// SDL_Surface *zpos = IMG_Load("/assets/skybox/zpos.png");	SDL_Surface *zneg = IMG_Load("/assets/skybox/zneg.png");
	
	// std::cerr << "girafe" << std::endl;


	// GLuint cubemap_texture;
	// setupCubeMap(cubemap_texture, xpos, xneg, ypos, yneg, zpos, zneg);

	// std::cerr << "koala" << std::endl;


	// SDL_FreeSurface(xneg);	SDL_FreeSurface(xpos);
	// SDL_FreeSurface(yneg);	SDL_FreeSurface(ypos);
	// SDL_FreeSurface(zneg);	SDL_FreeSurface(zpos);

	std::cerr << "tortue" << std::endl;


	//Chargement des shaders
    FilePath applicationPath(argv[0]);
    Program glProgram = loadProgram(
        applicationPath.dirPath() + "shaders/skybox.vs.glsl",
        applicationPath.dirPath() + "shaders/skybox.fs.glsl"
    );
    glProgram.use();

    std::cerr << "lapin" << std::endl;



	// grab the pvm matrix and vertex location from our shader program
	GLint PVM    = glGetUniformLocation(glProgram.getGLId(), "PVM");
	GLint vertex = glGetAttribLocation(glProgram.getGLId(), "vertex");

	// these won't change for now
	// glm::mat4 Projection = glm::perspective(45.0f, 800.f/600.f, 0.1f, 100.0f); 
	// glm::mat4 View       = glm::mat4(1.0f);
	// glm::mat4 Model      = glm::scale(glm::mat4(1.0f),glm::vec3(50,50,50));


	// cube vertices for vertex buffer object
	GLfloat cube_vertices[] = {
	  -1.0,  1.0,  1.0,
	  -1.0, -1.0,  1.0,
	   1.0, -1.0,  1.0,
	   1.0,  1.0,  1.0,
	  -1.0,  1.0, -1.0,
	  -1.0, -1.0, -1.0,
	   1.0, -1.0, -1.0,
	   1.0,  1.0, -1.0
	};
	GLuint vbo_cube_vertices;
	glGenBuffers(1, &vbo_cube_vertices);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
		glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), cube_vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// glEnableVertexAttribArray(vertex);
	// glVertexAttribPointer(vertex, 3, GL_FLOAT, GL_FALSE, 0, 0);
		// glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
		// glEnableVertexAttribArray(vertex);
		// glVertexAttribPointer(vertex, 3, GL_FLOAT, GL_FALSE, 0, 0);
		// glBindBuffer(GL_ARRAY_BUFFER, 0);
	// glEnableVertexAttribArray(vbo_cube_vertices);
	// glVertexAttribPointer(vbo_cube_vertices, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), 0);

	// cube indices for index buffer object
	GLushort cube_indices[] = {
	  0, 1, 2, 3,
	  3, 2, 6, 7,
	  7, 6, 5, 4,
	  4, 5, 1, 0,
	  0, 3, 7, 4,
	  1, 2, 6, 5
	};
	GLuint ibo_cube_indices;
	glGenBuffers(1, &ibo_cube_indices);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_indices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 24 * sizeof(GLushort), cube_indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	



	GLuint vao_cube;
	glGenVertexArrays(1, &vao_cube);

	glBindVertexArray(vao_cube);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_indices);

			glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
	glEnableVertexAttribArray(vertex);
	glVertexAttribPointer(vertex, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);




	// rotation angles
	float alpha = 0.0f, beta = 0.0f;










	// FilePath applicationPath(argv[0]);

	// GeneralProgram gProgram(applicationPath);
	// pointLightProgram lProgram(applicationPath);
	// gProgram.m_Program.use();


	// Sphere sphere (1, 32, 16);

	// //création vbo
	// GLuint vbo;
	// glGenBuffers (1, &vbo);

	// //bind le vbo
	// glBindBuffer (GL_ARRAY_BUFFER, vbo);

	// //buffer data void glBufferData(GLenum  target,  GLsizeiptr  size,  const GLvoid *  data,  GLenum  usage);
	// glBufferData (GL_ARRAY_BUFFER, sphere.getVertexCount() * sizeof(ShapeVertex),
	// 					 sphere.getDataPointer(), GL_STATIC_DRAW);

	// //debind le vbo
	// glBindBuffer(GL_ARRAY_BUFFER, 0);

	// //creation vao
	// GLuint vao;
	// glGenVertexArrays(1, &vao);

	// //bind le vao 
	// glBindVertexArray(vao);

	// const GLuint VERTEX_ATTR_POSITION = 0;
	// const GLuint VERTEX_ATTR_NORMALE = 1;

	// glEnableVertexAttribArray( VERTEX_ATTR_POSITION);
	// glEnableVertexAttribArray( VERTEX_ATTR_NORMALE);

	// glBindBuffer ( GL_ARRAY_BUFFER, vbo);

	// //void glVertexAttribPointer(GLuint  index,  GLint  size,  GLenum  type,  GLboolean  normalized,  GLsizei  stride,  const GLvoid *  pointer);
	// glVertexAttribPointer ( VERTEX_ATTR_POSITION, 3, GL_FLOAT,
	// 				GL_FALSE, 1 * sizeof(ShapeVertex), (const GLvoid*) (0*sizeof(GLfloat)));
	// glVertexAttribPointer ( VERTEX_ATTR_NORMALE, 3, GL_FLOAT,
	// 				GL_FALSE, 1 * sizeof(ShapeVertex), (const GLvoid*) (3*sizeof(GLfloat)));

	// //debind le vbo
	// glBindBuffer (GL_ARRAY_BUFFER, 0);
	
	// //debind le vao
	// glBindVertexArray(0);

	//Initialisation camera freefly
	FreeFlyCamera ffCam;

	//initialisation angle
	float angleX = 0;
	float angleY = 0;
	float angleYfinal = 0;

	const float CAMERA_ROT_FACTOR = 0.05f;

	//comme P ne change jamais on peut la declarer a l'initialisation
	glm::mat4 matrixP = glm::perspective(glm::radians(70.f), 800.f/600.f, 0.1f, 100.f);

	// make me a torch
	// Torch torch(glm::vec3(6, 0, 0));

	std::cerr << "lol" << std::endl;

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


		glm::mat4 matrixV = ffCam.getViewMatrix();
		glm::mat4 matrixM = glm::mat4(1.0); 
		glm::mat4 matrixMV = matrixV * matrixM;
		//calcul de la matrixViewProjetée
		glm::mat4 matrixMVP = matrixP * matrixMV;
		//calcul de la normal matrix = (MVinverse)Transposée
		glm::mat4 normalMatrix = glm::transpose(glm::inverse(matrixMV));



		// glm::mat4 RotateX = glm::rotate(glm::mat4(1.0f), angleX, glm::vec3(-1.0f, 0.0f, 0.0f));
		// glm::mat4 RotateY = glm::rotate(RotateX, angleY, glm::vec3(0.0f, 1.0f, 0.0f));
		// glm::mat4 M = Projection * View * Model * RotateY;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		

		glUniformMatrix4fv(PVM, 1, GL_FALSE, glm::value_ptr(matrixMVP));

		glBindVertexArray(vao_cube);
			glDrawElements(GL_QUADS, sizeof(cube_indices)/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
		glBindVertexArray(0);



		// lProgram.m_Program.use();
		// 	torch.draw(lProgram, matrixV);

		// 	glm::vec3 Kd = glm::vec3(1,1,1);
		// 	glm::vec3 Ks = glm::vec3(1,1,1);
		// 	float shininess = 3.f;



		// 	glUniform3f(lProgram.uKd, Kd.r, Kd.g, Kd.b);
		// 	glUniform3f(lProgram.uKs, Ks.r, Ks.g, Ks.b);
		// 	glUniform1f(lProgram.uShininess, shininess);


			

		// 	glUniformMatrix4fv(lProgram.uMVMatrix, 1, GL_FALSE,  glm::value_ptr(matrixMV));
		// 	glUniformMatrix4fv(lProgram.uMVPMatrix, 1, GL_FALSE,  glm::value_ptr(matrixMVP));
		// 	glUniformMatrix4fv(lProgram.uNormalMatrix, 1, GL_FALSE,  glm::value_ptr(normalMatrix));

		// //bind du vao
		// glBindVertexArray(vao);
		
		

		// //dessine triangles
		// glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());

		// //debind du vao
		// glBindVertexArray(0);

		// Update the display
		windowManager.swapBuffers();

	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// glDeleteBuffers(1, &vbo);
	// glDeleteVertexArrays(1, &vao);









	// release vertex and index buffer object
	glDeleteBuffers(1, &ibo_cube_indices);
	glDeleteBuffers(1, &vbo_cube_vertices);

	// release cube map
	//deleteCubeMap(cubemap_texture);

	// detach shaders from program and release
	// glDetachShader(glProgram, glShaderF);
	// glDetachShader(glProgram, glShaderV);
	// glDeleteShader(glShaderF);
	// glDeleteShader(glShaderV);
	// glDeleteProgram(glProgram);

	return EXIT_SUCCESS;
}
