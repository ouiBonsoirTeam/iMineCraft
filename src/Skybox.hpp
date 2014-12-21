#pragma once

#include <glimac/CustomProgram.hpp>
#include <glimac/glm.hpp>

using namespace glimac;

class Skybox 
{
private:
	GLuint _texture;
	GLuint _vbo;
	GLushort _cube_indices[24];
	GLuint _ibo;

public:
	Skybox(){};

	void init(SkyboxProgram &skyProg);

	void draw(SkyboxProgram &skyProg, const glm::mat4 &viewMatrix);

	void destruct();

};