#pragma once

#include "Block.hpp"
#include "OpenGLRenderer.hpp"
#include <glimac/glm.hpp>
#include <json/json.h>
#include <glimac/PerlinNoise.hpp>


enum ADJACENT_LOOK { LOOK_TOP = 0, LOOK_BACK, LOOK_RIGHT, LOOK_FRONT, LOOK_LEFT, LOOK_BOTTOM };

class Chunk{

private:
	Block*** m_pBlocks;
	OpenGLRenderer *m_pRenderer;
	bool m_loaded = false;
	bool m_setup = false;
	bool m_emptyChunk = false;

	//postition
	glm::vec3 m_position;

	Json::Value m_blocksData;


public:
	Chunk();
	Chunk(glm::vec3 position);
	~Chunk();

	glm::vec3 getPosition(){ return m_position; }
	int getX(){ return m_position[0]; }
	int getY(){ return m_position[1]; }
	int getZ(){ return m_position[2]; }
	bool isEmpty() { return m_emptyChunk; }

	static const int CHUNK_SIZE = 24;
	static const int TAILLE_X_TEXTURE = 16;

	static const int TAILLE_Y_TEXTURE = 24;

	// Getter
	Block*** getBlocks() const;
	Block* getBlock(const int &x, const int &y, const int &z) const;
	
	// Create the world chunk model
	void init();

	// Occlusion management
	bool blockExist(int x, int y, int z);
	bool blockExist(glm::vec3 vec);
	glm::mat3 getAdjacentMap(int x, int y, int z, int adjacent_look);
	int countAdjacent(glm::mat3 adjacentMap);
	
	glm::vec2 computeCoordText(const int & x, const int & y, const bool crop = 0);

	glm::vec2 getOcclusionCoordText(glm::mat3 adjacentMap);

	// Create a cube with position and seen sides
	void createCube(const int &x, const int &y, const int &z, const BlockType &blockType);

	// Prepare the render of seen Triangles
	void createMesh();
	void createLandscape(PerlinNoise *pn);

	// render
	void render(GeneralProgram &program, const glm::mat4 viewMatrix, GLuint idTexture);
 
	void update();

	bool isBlockVisible(const int &x, const int &y, const int &z);

	bool isLoaded();

	bool isSetup();

	void buildMesh(); 

	void destructBlock(const int &x, const int &y, const int &z);

	void constructBlock(const int &x, const int &y, const int &z);

	void setup(PerlinNoise *pn);

	void load(const Json::Value &chunkData);

	void unload();

	void updateShouldRenderFlags();

	void createHeightMap();
};