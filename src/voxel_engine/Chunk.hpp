#pragma once

#include "Block.hpp"
#include "OpenGLRenderer.hpp"

enum ADJACENT_LOOK { LOOK_TOP = 0, LOOK_BACK, LOOK_RIGHT, LOOK_FRONT, LOOK_LEFT, LOOK_BOTTOM };

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

	// Getter
	Block*** getBlocks() const;
	
	// Create the world chunk model
	void init();

	// Occlusion management
	bool blockExist(int x, int y, int z);
	bool blockExist(glm::vec3 vec);
	glm::mat3 getAdjacentMap(int x, int y, int z, int adjacent_look);
	int countAdjacent(glm::mat3 adjacentMap);
	glm::vec2 computeCoordText(const int & x, const int & y, const int & taille_x = TAILLE_X_TEXTURE, const int & taille_y = TAILLE_Y_TEXTURE);
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