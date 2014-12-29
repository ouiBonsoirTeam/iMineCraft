#pragma once


enum BlockType { 
	BlockType_Default = 0,

	BlockType_Stone,

	BlockType_Count
};

class Block{

private:
	bool m_active = false;
	BlockType m_blockType = BlockType_Default;

public:
	static const int BLOCK_RENDER_SIZE = 1;

	// Constructors
	Block();
	Block(BlockType blockType);

	bool isActive() const;
	void setActive();
	void setInactive();
	BlockType getType() const;
	void setType(int blockType);

	// Destructors
	~Block();
};
