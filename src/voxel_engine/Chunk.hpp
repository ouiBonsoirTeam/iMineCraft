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

	static const int CHUNK_SIZE = 16;																																																																																																																																				;

	void createMesh();

	void render(GeneralProgram &program, const glm::mat4 viewMatrix);

	void update();

	void createCube(const int &x, const int &y, const int &z);
};