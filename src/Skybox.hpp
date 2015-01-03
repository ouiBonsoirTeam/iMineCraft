#pragma once

#include <iostream>
#include <glimac/CustomProgram.hpp>
#include <glimac/glm.hpp>
#include <glimac/stb_image.h>

using namespace glimac;

class Skybox 
{
private:
	GLuint m_texture;
	GLuint m_vbo;
	GLuint m_vao;

public:
	Skybox(){};

	void init(SkyboxProgram &skyProgram);

	bool load_cube_map_side (GLenum side_target, const char* file_name);

	void create_cube_map (
		const char* front,
		const char* back,
		const char* top,
		const char* bottom,
		const char* left,
		const char* right);

	void draw(SkyboxProgram &skyProgram, const glm::mat4 &viewMatrix);

	void destruct();

};