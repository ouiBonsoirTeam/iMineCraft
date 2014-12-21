#include "Skybox.hpp"

#include <glimac/glhelper.hpp>
#include <glimac/misc.hpp>

using namespace glimac;

void Skybox::init(SkyboxProgram &skyProg)
{
	// set up the cube map texture

	// std::unique_ptr<Image> xpos = loadImage("bin/assets/skybox/xpos.png");
	// std::unique_ptr<Image> ypos = loadImage("bin/assets/skybox/xpos.png");
	// std::unique_ptr<Image> zpos = loadImage("bin/assets/skybox/xpos.png");
	// std::unique_ptr<Image> xneg = loadImage("bin/assets/skybox/xpos.png");
	// std::unique_ptr<Image> yneg = loadImage("bin/assets/skybox/xpos.png");
	// std::unique_ptr<Image> zneg = loadImage("bin/assets/skybox/xpos.png");

	SDL_Surface *xpos = IMG_Load("bin/assets/skybox/xpos.png");	SDL_Surface *xneg = IMG_Load("bin/assets/skybox/xneg.png");
	SDL_Surface *ypos = IMG_Load("bin/assets/skybox/ypos.png");	SDL_Surface *yneg = IMG_Load("bin/assets/skybox/yneg.png");
	SDL_Surface *zpos = IMG_Load("bin/assets/skybox/zpos.png");	SDL_Surface *zneg = IMG_Load("bin/assets/skybox/zneg.png");
	
	if (xpos == NULL)
		std::cerr << "Error while charging texture. " << std::endl;
	if (ypos == NULL)
		std::cerr << "Error while charging texture. " << std::endl;
	if (zpos == NULL)
		std::cerr << "Error while charging texture. " << std::endl;
	if (xneg == NULL)
		std::cerr << "Error while charging texture. " << std::endl;
	if (yneg == NULL)
		std::cerr << "Error while charging texture. " << std::endl;
	if (zneg == NULL)
		std::cerr << "Error while charging texture. " << std::endl;


	GLuint _texture;
	setupCubeMap(_texture, xpos, xneg, ypos, yneg, zpos, zneg);


	SDL_FreeSurface(xneg);	SDL_FreeSurface(xpos);
	SDL_FreeSurface(yneg);	SDL_FreeSurface(ypos);
	SDL_FreeSurface(zneg);	SDL_FreeSurface(zpos);


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
	GLuint _vbo;
	glGenBuffers(1, &_vbo);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);


	// cube indices for index buffer object
	GLushort _cube_indices[] = {
	  0, 1, 2, 3,
	  3, 2, 6, 7,
	  7, 6, 5, 4,
	  4, 5, 1, 0,
	  0, 3, 7, 4,
	  1, 2, 6, 5
	};

	GLuint _ibo;
	glGenBuffers(1, &_ibo);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_cube_indices), _cube_indices, GL_STATIC_DRAW);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glEnableVertexAttribArray(skyProg.vertex);
	glVertexAttribPointer(skyProg.vertex, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
}

void Skybox::draw(SkyboxProgram &skyProg, const glm::mat4 &viewMatrix)
{
	//cste dans le programme, a degager de la classe
	glm::mat4 projMatrix = glm::perspective(glm::radians(70.f), 800.f/600.f, 0.1f, 100.f);

	glm::mat4 modelMatrix =  glm::scale(glm::mat4(1.0), glm::vec3(100, 100, 100)); 
	glm::mat4 modelViewMatrix = viewMatrix * modelMatrix;
	//calcul de la matrixViewProjetée
	glm::mat4 modelViewProjMatrix = projMatrix * modelViewMatrix;

	glm::mat4 rotateCamMat = modelViewProjMatrix;
	rotateCamMat[3][0] = 0;
	rotateCamMat[3][1] = 0;
	rotateCamMat[3][2] = 0;
	rotateCamMat[3][3] = 1;

	glUniformMatrix4fv(skyProg.PVM, 1, GL_FALSE, glm::value_ptr(rotateCamMat));

	glDrawElements(GL_QUADS, sizeof(_cube_indices)/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

}

void Skybox::destruct()
{
	// release vertex and index buffer object
	glDeleteBuffers(1, &_ibo);
	glDeleteBuffers(1, &_vbo);

	// release cube map
	deleteCubeMap(_texture);
}

