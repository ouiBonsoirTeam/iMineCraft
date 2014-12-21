#include "Chunk.hpp"
#include <glimac/glm.hpp>
#include <iostream>

// Constructor
Chunk::Chunk(){
	// Create the blocks
	m_pBlocks = new Block**[CHUNK_SIZE];

	for(int i = 0; i < CHUNK_SIZE; i++)
	{
		m_pBlocks[i] = new Block*[CHUNK_SIZE];

		for(int j = 0; j < CHUNK_SIZE; j++)
		{
			m_pBlocks[i][j] = new Block[CHUNK_SIZE];
		}
	}

	m_pRenderer = new OpenGLRenderer;
}

// Destructor
Chunk::~Chunk(){
	// Delete the blocks
	for (int i = 0; i < CHUNK_SIZE; ++i)
	{
		for (int j = 0; j < CHUNK_SIZE; ++j)
		{
			delete [] m_pBlocks[i][j];
		}

		delete [] m_pBlocks[i];
	}
	delete [] m_pBlocks;

	delete m_pRenderer;
}

void Chunk::init()
{
	/*
	for (int x = 0; x < CHUNK_SIZE; x++)
	{
		for (int y = 0; y < CHUNK_SIZE; y++)
		{
			for (int z = 0; z < CHUNK_SIZE; z++)
			{
				// Init a sphere
				if (sqrt((float) (x-CHUNK_SIZE/2)*(x-CHUNK_SIZE/2) + (y-CHUNK_SIZE/2)*(y-CHUNK_SIZE/2) + (z-CHUNK_SIZE/2)*(z-CHUNK_SIZE/2)) <= CHUNK_SIZE/2.0)
				{
					// m_pBlocks[x][y][z].setActive();
				}
			}
		}		
	}
	*/

	int y = 0;
	for (int x = 0; x < CHUNK_SIZE; x++)
	{
		for (int z = 0; z < CHUNK_SIZE; z++)
		{
			m_pBlocks[x][y][z].setActive();
		}	
	}

	m_pBlocks[1][1][1].setActive();
	m_pBlocks[2][1][2].setActive();
	m_pBlocks[2][1][1].setActive();

}

bool Chunk::blockExist(int x, int y, int z)
{
	return !(x < 0 || x > CHUNK_SIZE - 1 || y < 0 || y > CHUNK_SIZE - 1 || z < 0 || z > CHUNK_SIZE - 1);
}

glm::mat3 Chunk::getAdjacentMap(int x, int y, int z)
{
	glm::mat3 adjacentMap = glm::mat3(0);
	int i, j, k;

	i = x + 1;
	j = y + 1;
	k = z - 1;
	// if(blockExist(i, j, k) && m_pBlocks[i][j][k].isActive())
	// 	adjacentMap[0][0] = 1;

	k = z + 1;
	i = x - 1;
	if(blockExist(i, j, k) && m_pBlocks[i][j][k].isActive())
		adjacentMap[1][0] = 1;

	k = z + 1;
	// if(blockExist(i, j, k) && m_pBlocks[i][j][k].isActive())
	// 	adjacentMap[2][0] = 1;

	i = x;
	k = z - 1;
	if(blockExist(i, j, k) && m_pBlocks[i][j][k].isActive())
		adjacentMap[0][1] = 1;

	k = z + 1;
	if(blockExist(i, j, k) && m_pBlocks[i][j][k].isActive())
		adjacentMap[2][1] = 1;

	i = x + 1;
	k = z - 1;
	// if(blockExist(i, j, k) && m_pBlocks[i][j][k].isActive())
	// 	adjacentMap[0][2] = 1;

	k = z;
	if(blockExist(i, j, k) && m_pBlocks[i][j][k].isActive())
		adjacentMap[1][2] = 1;

	k = z + 1;
	// if(blockExist(i, j, k) && m_pBlocks[i][j][k].isActive())
	// 	adjacentMap[2][2] = 1;

	return adjacentMap;
}


int Chunk::countAdjacent(glm::mat3 adjacentMap)
{
	int sum = 0;
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			sum += adjacentMap[i][j];
		}
	}

	return sum;
}

glm::vec2 Chunk::getOcclusionCoordText(glm::mat3 adjacentMap)
{
	int nb_adj = countAdjacent(adjacentMap);
	float row = 0, col = 0;

	int top = adjacentMap[0][1];
	int bottom = adjacentMap[2][1];
	int left = adjacentMap[1][0];
	int right = adjacentMap[1][2];

	switch(nb_adj)
	{
		case 0:
		break;

		case 1:
			row = 0.25;

			if(top == 1)
				col = 0;

			else if(right == 1)
				col = 0.25;

			else if(bottom == 1)
				col = 0.5;

			else if(left == 1)
				col = 0.75;
		break;

		case 2:
			if(top == 1 && bottom == 1)
			{
				row = 0;
				col = 0.25;
			}
			else if(left == 1 && right == 1)
			{
				row = 0;
				col = 0.5;
			}
			else if(top == 1 && right == 1)
			{
				row = 0.5;
				col = 0;
			}
			else if(right == 1 && bottom == 1)
			{
				row = 0.5;
				col = 0.25;
			}
			else if(bottom == 1 && left == 1)
			{
				row = 0.5;
				col = 0.5;
			}
			else if(left == 1 && top == 1)
			{	
				row = 0.5;
				col = 0.75;
			}
		break;

		case 3:
			std::cerr << "ca va gueuler" << std::endl << std::endl;
			std::cerr << adjacentMap << std::endl << std::endl;
			row = 0.75;

			if(left == 0)
				col = 0;

			else if(top == 0)
				col = 0.25;

			else if(right == 0)
				col = 0.5;

			else if(bottom == 0)
				col = 0.75;
		break;

		case 4:
			col = 0.75;
		break;

		default:
		break;
	}

	return glm::vec2(row, col);
}

void Chunk::createMesh()
{
	bool lDefault = true;

	for (int x = 0; x < CHUNK_SIZE; x++)
	{
		for (int y = 0; y < CHUNK_SIZE; y++)
		{
			for (int z = 0; z < CHUNK_SIZE; z++)
			{
				if(m_pBlocks[x][y][z].isActive() == false)
				{
					continue;
				}

				bool lXNegative = lDefault;
				if(x > 0)
					lXNegative = !m_pBlocks[x-1][y][z].isActive();

				bool lXPositive = lDefault;
				if(x < CHUNK_SIZE - 1)
					lXPositive = !m_pBlocks[x+1][y][z].isActive();

				bool lYNegative = lDefault;
				if(y > 0)
					lYNegative = !m_pBlocks[x][y-1][z].isActive();

				bool lYPositive = lDefault;
				if(y < CHUNK_SIZE - 1)
					lYPositive = !m_pBlocks[x][y+1][z].isActive();

				bool lZNegative = lDefault;
				if(z > 0)
					lZNegative = !m_pBlocks[x][y][z-1].isActive();

				bool lZPositive = lDefault;
				if(z < CHUNK_SIZE - 1)
					lZPositive = !m_pBlocks[x][y][z+1].isActive();

				createCube(x, y, z, lXNegative, lXPositive, lYNegative, lYPositive, lZNegative, lZPositive);
			}
		}
	}

	m_pRenderer->finishVbo();
}

void Chunk::render(GeneralProgram &program, const glm::mat4 viewMatrix, GLuint idTexture)
{
	m_pRenderer->setVao();

	m_pRenderer->draw(program, viewMatrix, idTexture);
}

void Chunk::update(){}

void Chunk::createCube(	const int &x, const int &y, const int &z, const bool & lXNegative, const bool &lXPositive,
						const bool &lYNegative, const bool &lYPositive, const bool &lZNegative, const bool &lZPositive)
{
	glm::vec3 v1(x-Block::BLOCK_RENDER_SIZE * 0.5, y-Block::BLOCK_RENDER_SIZE * 0.5, z+Block::BLOCK_RENDER_SIZE * 0.5);
	glm::vec3 v2(x+Block::BLOCK_RENDER_SIZE * 0.5, y-Block::BLOCK_RENDER_SIZE * 0.5, z+Block::BLOCK_RENDER_SIZE * 0.5);
	glm::vec3 v3(x+Block::BLOCK_RENDER_SIZE * 0.5, y+Block::BLOCK_RENDER_SIZE * 0.5, z+Block::BLOCK_RENDER_SIZE * 0.5);
	glm::vec3 v4(x-Block::BLOCK_RENDER_SIZE * 0.5, y+Block::BLOCK_RENDER_SIZE * 0.5, z+Block::BLOCK_RENDER_SIZE * 0.5);
	glm::vec3 v5(x+Block::BLOCK_RENDER_SIZE * 0.5, y-Block::BLOCK_RENDER_SIZE * 0.5, z-Block::BLOCK_RENDER_SIZE * 0.5);
	glm::vec3 v6(x-Block::BLOCK_RENDER_SIZE * 0.5, y-Block::BLOCK_RENDER_SIZE * 0.5, z-Block::BLOCK_RENDER_SIZE * 0.5);
	glm::vec3 v7(x-Block::BLOCK_RENDER_SIZE * 0.5, y+Block::BLOCK_RENDER_SIZE * 0.5, z-Block::BLOCK_RENDER_SIZE * 0.5);
	glm::vec3 v8(x+Block::BLOCK_RENDER_SIZE * 0.5, y+Block::BLOCK_RENDER_SIZE * 0.5, z-Block::BLOCK_RENDER_SIZE * 0.5);

	glm::vec2 text_coord = getOcclusionCoordText(getAdjacentMap(x, y, z));

	text_coord = glm::vec2(text_coord[1], text_coord[0]);


	std::cerr << "cube " << x << y << z;;
	std::cerr << getAdjacentMap(x, y, z) << std::endl << std::endl;
	std::cerr << text_coord << std::endl << std::endl;



	// Normal
	glm::vec3 n1;


	// Front
	if(lZPositive)
	{
		n1 = glm::vec3(0.0f, 0.0f, 1.0f);
		m_pRenderer->addNormal(n1);

		m_pRenderer->addTriangle(v1, v2, v3);
		m_pRenderer->addTexture(glm::vec2(0, 1), glm::vec2(1, 1), glm::vec2(1, 0));
		
		m_pRenderer->addTriangle(v1, v3, v4);
		m_pRenderer->addTexture(glm::vec2(0, 1), glm::vec2(1, 0), glm::vec2(0, 0));
	}


	// Back
	if(lZNegative)
	{
		n1 = glm::vec3(0.0f, 0.0f, -1.0f);
		m_pRenderer->addNormal(n1);

		m_pRenderer->addTriangle(v5, v6, v7);
		m_pRenderer->addTexture(glm::vec2(0, 1), glm::vec2(1, 1), glm::vec2(1, 0));
		
		m_pRenderer->addTriangle(v5, v7, v8);
		m_pRenderer->addTexture(glm::vec2(0, 1), glm::vec2(1, 0), glm::vec2(0, 0));
	}

	// Right
	if(lXPositive)
	{
		n1 = glm::vec3(1.0f, 0.0f, 0.0f);
		m_pRenderer->addNormal(n1);

		m_pRenderer->addTriangle(v2, v5, v8);
		m_pRenderer->addTexture(glm::vec2(0, 1), glm::vec2(1, 1), glm::vec2(1, 0));
		
		m_pRenderer->addTriangle(v2, v8, v3);
		m_pRenderer->addTexture(glm::vec2(0, 1), glm::vec2(1, 0), glm::vec2(0, 0));
	}


	// left
	if(lXNegative)
	{
		n1 = glm::vec3(-1.0f, 0.0f, 0.0f);
		m_pRenderer->addNormal(n1);

		m_pRenderer->addTriangle(v6, v1, v4);
		m_pRenderer->addTexture(glm::vec2(0, 1), glm::vec2(1, 1), glm::vec2(1, 0));
		
		m_pRenderer->addTriangle(v6, v4, v7);
		m_pRenderer->addTexture(glm::vec2(0, 1), glm::vec2(1, 0), glm::vec2(0, 0));
	}



	// Top
	if(lYPositive)
	{
		n1 = glm::vec3(0.0f, 1.0f, 0.0f);
		m_pRenderer->addNormal(n1);

		m_pRenderer->addTriangle(v4, v3, v8);
		// m_pRenderer->addTexture(glm::vec2(0, 1), glm::vec2(1, 1), glm::vec2(1, 0));
		m_pRenderer->addTexture(text_coord + glm::vec2(0, 0.25), text_coord + glm::vec2(0.25, 0.25), text_coord + glm::vec2(0.25, 0));
		
		std::cerr << text_coord + glm::vec2(0.25, 0) << std::endl;
		std::cerr << text_coord + glm::vec2(0.25, 0.25) << std::endl;
		std::cerr << text_coord + glm::vec2(0, 0.25) << std::endl << std::endl;
		
		m_pRenderer->addTriangle(v4, v8, v7);
		 //m_pRenderer->addTexture(glm::vec2(0, 1), glm::vec2(1, 0), glm::vec2(0, 0));
		m_pRenderer->addTexture(text_coord + glm::vec2(0, 0.25), text_coord + glm::vec2(0.25, 0), text_coord + glm::vec2(0, 0));

		std::cerr << text_coord + glm::vec2(0.25, 0) << std::endl;
		std::cerr << text_coord + glm::vec2(0, 0.25) << std::endl;
		std::cerr << text_coord + glm::vec2(0, 0) << std::endl << std::endl;
		std::cerr << "_________________________________" << std::endl << std::endl;
	}


	// Bottom
	if(lYNegative)
	{
		n1 = glm::vec3(0.0f, -1.0f, 0.0f);
		m_pRenderer->addNormal(n1);

		m_pRenderer->addTriangle(v6, v5, v2);
		m_pRenderer->addTexture(glm::vec2(0, 1), glm::vec2(1, 1), glm::vec2(1, 0));
		
		m_pRenderer->addTriangle(v6, v2, v1);
		m_pRenderer->addTexture(glm::vec2(0, 1), glm::vec2(1, 0), glm::vec2(0, 0));
	}

}