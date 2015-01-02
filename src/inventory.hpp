#pragma once


#include <iostream>
#include <iterator>

#include <vector>
#include "voxel_engine/Block.hpp"


class Inventory
{
private:
	std::vector<int> m_blocks;

public:
	Inventory()
	{
		for (int i = 0; i < BlockType_Count; ++i)
		{
			m_blocks.push_back(0);
		}
	}

	int getNbBlock(BlockType bt)
	{
		return m_blocks[bt];
	}
	void addBlock(BlockType bt)
	{
		m_blocks[bt]++;
	}
	void deleteBlock(BlockType bt)
	{
		m_blocks[bt]--;
	}
	void affiche()
	{
		std::copy(m_blocks.begin(), m_blocks.end(), std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
	}





	~Inventory(){}



};