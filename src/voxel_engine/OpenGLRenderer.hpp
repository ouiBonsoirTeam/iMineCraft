#pragma once

#include <vector>
#include <glimac/glm.hpp>
#include <GL/glew.h>
#include <glimac/CustomProgram.hpp>

enum VBO { POSITION, NORMAL, TEXTURE};


using namespace glimac;

class OpenGLRenderer
{
	GLuint m_vao;
    // GLuint m_vbo;
    GLuint m_buffer[3];

    std::vector<glm::vec3> m_vertices;
    std::vector<glm::vec3> m_normals;
    std::vector<glm::vec4> m_textures;

    const static GLuint VERTEX_ATTR_POSITION = 0;
	const static GLuint VERTEX_ATTR_NORMAL = 1;
	const static GLuint VERTEX_ATTR_TEXTCOORD = 2;

public:

	// Constructor
	OpenGLRenderer();

	// Destructor
	~OpenGLRenderer();

	// Setters
	void addTriangle(glm::vec3 position_1, glm::vec3 position_2, glm::vec3 position_3);
	void addTexture(glm::vec4 texPos_1, glm::vec4 texPos_2, glm::vec4 texPos_3);
	void addNormal(glm::vec3 n);

	// Fill the buffer data
	void finishVboPosition();
	void finishVboTexture();
	void finishVboNormal();
	void finishVbo();

	void setVao();

	void draw(GeneralProgram &program, const glm::mat4 &viewMatrix, GLuint idTexture);
	
};