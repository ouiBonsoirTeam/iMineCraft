#pragma once


enum BlockType { 

	BlockType_Lava,
	BlockType_Rock,
	BlockType_Earth,
	BlockType_Grass,
	BlockType_1st_Snow,
	BlockType_Snow,
	BlockType_Ice,
	BlockType_Wood,
	BlockType_Leaf,
	BlockType_Wood2,
	BlockType_Leaf2,

	BlockType_Count
};


class Block{

private:
	bool m_active = false;
	BlockType m_blockType = BlockType_Grass;

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
