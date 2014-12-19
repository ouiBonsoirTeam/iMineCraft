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
	Block();
	Block(BlockType blockType);
	~Block();

	static const int BLOCK_RENDER_SIZE = 1;

	bool isActive();
	void setActive();
	void setInactive();
};
