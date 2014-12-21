#pragma once

#include "Block.hpp"
#include "OpenGLRenderer.hpp"

class Chunk{

private:
	Block*** m_pBlocks;
	OpenGLRenderer *m_pRenderer;

public:

	static const int CHUNK_SIZE = 32;
	// Construtors
	Chunk();
	
	// Create the world chunk model
	void init();

	// Create a cube with position and seen sides
	void createCube(const int &x, const int &y, const int &z, const bool & lXNegative, const bool &lXPositive,
					const bool &lYNegative, const bool &lYPositive, const bool &lZNegative, const bool &lZPositive);

	// Prepare the render of seen Triangles
	void createMesh();

	// render
	void render(GeneralProgram &program, const glm::mat4 viewMatrix, GLuint idTexture);

	// 
	void update();


	// Destructors
	~Chunk();
};