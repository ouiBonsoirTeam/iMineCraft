#include "Block.hpp"

// Constructors
Block::Block(){}

Block::Block(BlockType blockType){
	m_blockType = blockType;
}

// Destructor
Block::~Block(){}

// Getter
bool Block::isActive(){
	return m_active;
}

// Setter
void Block::setActive(){
	m_active = true;
}

void Block::setInactive(){
	m_active = false;
}