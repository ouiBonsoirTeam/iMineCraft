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
	BlockType m_blockType;

public:
	static const int BLOCK_RENDER_SIZE = 1;

	// Constructors
	Block();
	Block(BlockType blockType);

	// Getters
	bool isActive() const;
	BlockType getType() const;

	// Setters
	void setActive();
	void setInactive();
	void setType(int blockType);

	// Destructors
	~Block();
};
