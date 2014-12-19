#pragma once

#include <vector>
#include <glimac/glm.hpp>
#include <GL/glew.h>
#include <glimac/CustomProgram.hpp>


using namespace glimac;

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

	void addVertex(glm::vec3 position);
	void finishVbo();
	void setVao();
	void draw(GeneralProgram &program, const glm::mat4 &viewMatrix);
	
};