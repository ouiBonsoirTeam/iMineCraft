#include "Block.hpp"

enum BlockType { 
	BlockType_Default = 0,

	BlockType_Stone,

	BlockType_Count
};

// Constructors
Block::Block(){}

Block::Block(BlockType blockType){
	m_blockType = blockType;
}

// Destructor
Block::~Block(){}