#pragma once

#include "Block.hpp"
#include "OpenGLRenderer.hpp"
#include <glimac/glm.hpp>


class Chunk{

private:
	Block*** m_pBlocks;
	OpenGLRenderer *m_pRenderer;
	bool m_loaded = true;		//a modifier quand on fera la sauvegarde
	bool m_setup = true;

	//postition
	glm::vec3 m_position;


public:
	Chunk();
	Chunk(glm::vec3 position);
	~Chunk();

	static const int CHUNK_SIZE = 20;																																																																																																																																				;

	void createMesh();

	void render(GeneralProgram &program, const glm::mat4 viewMatrix);

	void update();

	void createCube(const int &x, const int &y, const int &z);

	bool isLoaded();

	bool isSetup();

	void rebuildMesh(); //a coder

	void destructBlock(const int &x, const int &y, const int &z);

	void constructBlock(const int &x, const int &y, const int &z);

	void setup();

	glm::vec3 getPosition(){ return m_position; }


	void load();	//a coder

	Block*** getBlocks();
};