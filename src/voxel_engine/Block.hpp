#pragma once


enum BlockType { 
	BlockType_Default = 0,

	BlockType_Stone,

	BlockType_Count
};

class Block{

private:
	bool m_active;
	BlockType m_blockType;

public:
	static const int BLOCK_RENDER_SIZE = 1;

	// Constructors
	Block();
	Block(BlockType blockType);

	// Getters
	bool isActive();

	// Setters
	void setActive();
	void setInactive();

	// Destructors
	~Block();
};
