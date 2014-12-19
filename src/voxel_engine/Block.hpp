#pragma once

class Block{

	bool m_active;
	BlockType m_blockType;

public:
	Block();
	~Block();

	bool isActive();
	void setActive();
};
