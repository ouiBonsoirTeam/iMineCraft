#include "Block.hpp"

// Constructors
Block::Block(){}
Block::Block(BlockType blockType) : m_blockType(blockType) {}

// Getters
bool Block::isActive() const { return m_active; }
BlockType Block::getType() const { return m_blockType; }

// Setters
void Block::setActive(){ m_active = true; }
void Block::setInactive(){ m_active = false; }
void Block::setType(int blockType){	m_blockType = (BlockType)blockType; }

// Destructor
Block::~Block(){}