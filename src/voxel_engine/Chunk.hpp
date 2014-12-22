#pragma once

#include "Block.hpp"
#include "OpenGLRenderer.hpp"

class Chunk{

private:
	Block*** m_pBlocks;
	OpenGLRenderer *m_pRenderer;

public:

	static const int CHUNK_SIZE = 24;
	static const int TAILLE_X_TEXTURE = 16;
	static const int TAILLE_Y_TEXTURE = 17;
	// Construtors
	Chunk();
	
	// Create the world chunk model
	void init();

	// Occlusion management
	bool blockExist(int x, int y, int z);
	glm::mat3 getAdjacentMap(int x, int y, int z);
	int countAdjacent(glm::mat3 adjacentMap);
	glm::vec2 getCoordText(const int & x, const int & y, const int & taille_x = TAILLE_X_TEXTURE, const int & taille_y = TAILLE_Y_TEXTURE);
	glm::vec2 getOcclusionCoordText(glm::mat3 adjacentMap);

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