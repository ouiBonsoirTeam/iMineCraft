#pragma once


#include <iostream>
#include <iterator>

#include <vector>
#include "voxel_engine/Block.hpp"

#include <json/json.h>
#include <fstream>


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

	void save(const std::string &jsonFolderPath)
	{
		std::ofstream file;
		file.open(jsonFolderPath + "A_inventory.json");
		std::string str;

		if (file.is_open())
		{
			Json::Value jsonValue;

			for(int i = 0; i < BlockType_Count; ++i)
			{
				str = "BlockType_" + std::to_string(i);
				jsonValue[str] = Json::Value(m_blocks.at(i));
			}

			Json::FastWriter l_writer;

	        file << l_writer.write(jsonValue);

	        file.close();

		}
		else
		{
			std::cerr << "Unable to open file" << std::endl;
			exit(1);
		}
	}

	bool load(const std::string & saveFolder)
	{
	    std::ifstream file;
	    std::string filePath = saveFolder + "A_inventory.json";
	    file.open(filePath);
	    std::string str, contents;

	    if (file.is_open())
	    {
	        while (std::getline(file, str))
	        {
	            contents += str;
	        }  
	        file.close();

	        Json::Value root;
	        Json::Reader reader;

	        bool parsingSuccessful = reader.parse(contents, root);
	        if ( !parsingSuccessful )
	        {
	            // report to the user the failure and their locations in the document.
	            std::cerr  << "Failed to parse configuration\n"
	                       << reader.getFormattedErrorMessages();
	            return false;
	        }
	        else
	        {
	        	std::string str;
	        	for(int i = 0; i < BlockType_Count; ++i)
	        	{
	        		str = "BlockType_" + std::to_string(i);
	        		m_blocks.at(i) = root[str].asInt();
	        	}

	            return true;
	        }
	    }

	    return false;
	}



	~Inventory(){}



};