#pragma once


enum BlockType { 

	BlockType_Lava,
	BlockType_Rock,
	BlockType_Earth,
	BlockType_Grass,
	BlockType_1st_Snow,
	BlockType_Snow,
	BlockType_Ice,

	BlockType_Count
};

class Block{

private:
	bool m_active = false;
	BlockType m_blockType = BlockType_Lava;

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
