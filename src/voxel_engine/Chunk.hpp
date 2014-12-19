#pragma once

#include "Block.hpp"

class Chunk{

private:
	Block*** m_pBlocks;

public:
	Chunk();
	~Chunk();

	static const int CHUNK_SIZE = 16;

	void Render();

	void Update();
};