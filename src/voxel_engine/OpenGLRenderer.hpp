#pragma once

#include <vector>
#include <glimac/glm.hpp>
#include <GL/glew.h>
#include <glimac/CustomProgram.hpp>

using namespace glimac;

enum bufferType { 
	INDEX_BUFFER = 0,

	POS_VB,
	NORMAL_VB,
	TEXT_COORDS_VB,
	WVP_MAT_VB,
	WORLD_MAT_VB,

	BUFFER_SIZE
};

class OpenGLRenderer
{
	GLuint m_vao;
    GLuint m_vbo;

    std::vector<glm::vec3> m_vertices;

    const static GLuint VERTEX_ATTR_POSITION = 0;
	const static GLuint VERTEX_ATTR_NORMAL = 1;
	const static GLuint VERTEX_ATTR_TEXTCOORD = 2;

public:

	// Constructor
	OpenGLRenderer();

	// Destructor
	~OpenGLRenderer();

	void addTriangle(glm::vec3 position_1, glm::vec3 position_2, glm::vec3 position_3);
	void finishVbo();
	void setVao();
	void renderMesh(GeneralProgram &program, const glm::mat4 &viewMatrix);
	
};