#pragma once

#include <vector>
#include <glimac/glm.hpp>
#include <GL/glew.h>
#include <glimac/CustomProgram.hpp>

using namespace glimac;

enum bufferType { 
	INDEX_BUFFER = 0,

	CUBE_VERT_VB,
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

    GLuint m_buffer[5];

    unsigned int visibleBlockCount = 0;

    std::vector<unsigned int> m_index;
    std::vector<glm::vec3> m_cube_vertices;
    std::vector<glm::vec3> m_positions;
    std::vector<glm::vec3> m_cube_normals;
    std::vector<glm::vec4> m_textures;

    const static GLuint CUBE_VERTEX = 0;
	const static GLuint VERTEX_ATTR_NORMAL = 1;
	const static GLuint VERTEX_ATTR_TEXTCOORD = 2;
	const static GLuint VERTEX_ATTR_POSITION = 3;

public:

	// Constructor
	OpenGLRenderer();

	// Destructor
	~OpenGLRenderer();

	// Setters
	void addTriangle(unsigned int index_1, unsigned int index_2, unsigned int index_3);
	void addPosition(glm::vec3 position);
	void addTexture(glm::vec4 texPos_1, glm::vec4 texPos_2, glm::vec4 texPos_3);
	void addNormal(glm::vec3 n);
	void incrementBlockCount(){ ++visibleBlockCount; };

	// Fill the buffer data
	void finishIbo();
	void finishVboCubeModel();
	void finishVboPosition();
	void finishVboTexture();
	void finishVboNormal();
	void finishVbo();

	void setVao();

	void renderMesh(GLuint idTexture);
	void getMeshInformation(int &numVerts);
};