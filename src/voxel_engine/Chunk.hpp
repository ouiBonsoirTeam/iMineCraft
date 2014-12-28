#pragma once

#include "Block.hpp"
#include "OpenGLRenderer.hpp"
#include <glimac/glm.hpp>
#include <json/json.h>


class Chunk{

private:
	Block*** m_pBlocks;
	OpenGLRenderer *m_pRenderer;
	bool m_loaded = false;
	bool m_setup = true;

	//postition
	glm::vec3 m_position;

	Json::Value m_blocksData;


public:
	Chunk();
	Chunk(glm::vec3 position);
	~Chunk();

	static const int CHUNK_SIZE = 20;																																																																																																																																				;

	void createMesh();

	void render(GeneralProgram &program, const glm::mat4 viewMatrix);

	void update();

	void createCube(const int &x, const int &y, const int &z, const BlockType &blockType);

	bool isLoaded();

	bool isSetup();

	void buildMesh(); 

	void destructBlock(const int &x, const int &y, const int &z);

	void constructBlock(const int &x, const int &y, const int &z);

	void setup();

	glm::vec3 getPosition(){ return m_position; }

	void load(const Json::Value &chunkData);

	Block*** getBlocks();

	void unload();
};