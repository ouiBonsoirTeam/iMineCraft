#pragma once

#include "Block.hpp"
#include "OpenGLRenderer.hpp"

class Chunk{

private:
	Block*** m_pBlocks;
	OpenGLRenderer *m_pRenderer;

public:
	Chunk();
	~Chunk();

	static const int CHUNK_SIZE = 32;

	void init();

	void createMesh();

	void render(GeneralProgram &program, const glm::mat4 viewMatrix, GLuint idTexture);

	void update();

	void createCube(const int &x, const int &y, const int &z, const bool & lXNegative, const bool &lXPositive,
					const bool &lYNegative, const bool &lYPositive, const bool &lZNegative, const bool &lZPositive);
};