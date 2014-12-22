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

// Getter
Block*** Chunk::getBlocks() const
{
	return m_pBlocks;
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

	m_pBlocks[2][1][2].setActive();
	m_pBlocks[3][1][3].setActive();
	m_pBlocks[3][1][2].setActive();

	m_pBlocks[5][1][4].setActive();
	m_pBlocks[5][1][6].setActive();

	m_pBlocks[8][1][6].setActive();
	m_pBlocks[9][1][6].setActive();
	m_pBlocks[10][1][6].setActive();
	m_pBlocks[4][1][16].setActive();
	m_pBlocks[6][1][16].setActive();
	m_pBlocks[6][1][16].setActive();
	m_pBlocks[6][1][16].setActive();
	m_pBlocks[16][1][8].setActive();
	m_pBlocks[16][1][7].setActive();
	m_pBlocks[15][1][6].setActive();
	m_pBlocks[13][1][6].setActive();
	m_pBlocks[4][1][8].setActive();
	m_pBlocks[12][1][10].setActive();
	m_pBlocks[12][1][11].setActive();
	m_pBlocks[13][1][12].setActive();
	m_pBlocks[10][1][2].setActive();
	m_pBlocks[13][1][9].setActive();
	m_pBlocks[13][1][11].setActive();


}

bool Chunk::blockExist(int x, int y, int z)
{
	return !(x < 0 || x > CHUNK_SIZE - 1 || y < 0 || y > CHUNK_SIZE - 1 || z < 0 || z > CHUNK_SIZE - 1);
}

glm::mat3 Chunk::getAdjacentMap(int x, int y, int z)
{
    glm::mat3 adjacentMap = glm::mat3(0);  

    //leftTop
    if(blockExist(x-1, y+1, z-1) && m_pBlocks[x-1][y+1][z-1].isActive())
        adjacentMap[0][0] = 1;

    //top
    if(blockExist(x, y+1, z-1) && m_pBlocks[x][y+1][z-1].isActive())
        adjacentMap[0][1] = 1;

     //topRight
    if(blockExist(x+1, y+1, z-1) && m_pBlocks[x+1][y+1][z-1].isActive())
        adjacentMap[0][2] = 1;

     //right
    if(blockExist(x+1, y+1, z) && m_pBlocks[x+1][y+1][z].isActive())
        adjacentMap[1][2] = 1;

    //rightBottom
    if(blockExist(x+1, y+1, z+1) && m_pBlocks[x+1][y+1][z+1].isActive())
        adjacentMap[2][2] = 1;

    //bottom
    if(blockExist(x, y+1, z+1) && m_pBlocks[x][y+1][z+1].isActive())
        adjacentMap[2][1] = 1;

    //bottomLeft
    if(blockExist(x-1, y+1, z+1) && m_pBlocks[x-1][y+1][z+1].isActive())
        adjacentMap[2][0] = 1;

    //left
    if(blockExist(x-1, y+1, z) && m_pBlocks[x-1][y+1][z].isActive())
        adjacentMap[1][0] = 1;

    

   

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

glm::vec2 Chunk::getCoordText(const int & x, const int & y, const int & taille_x, const int & taille_y)
{
	return	glm::vec2(x * (1.0 / taille_x), y * (1.0 / taille_y));
}

glm::vec2 Chunk::getOcclusionCoordText(glm::mat3 adjacentMap)
{
	int nb_adj = countAdjacent(adjacentMap);

	int topLeft = adjacentMap[0][0];
	int top = adjacentMap[0][1];
	int topRight = adjacentMap[0][2];
	int right = adjacentMap[1][2];
	int bottomRight = adjacentMap[2][2];
	int bottom = adjacentMap[2][1];
	int bottomLeft = adjacentMap[2][0];
	int left = adjacentMap[1][0];
	
	glm::vec2 res;

	//texture coord x
	if (left)
		if (top)
			if (topRight)
				if(topLeft)
					res[0] = 15;
				else res[0] = 14;

			else if (topLeft)
				res[0] = 11;

			else res[0] = 10;

		else if (topRight)
			if(topLeft)
				res[0] = 13;
			else res[0] = 12;

		else if (topLeft)
			res[0] = 9;

		else res[0] = 8;

	else if (top)
		if (topRight)
			if (topLeft)
				res[0] = 7;
			else
				res[0] = 6;
		else if (topLeft)
			res[0] = 3;
		else res[0] = 2;
	
	else if (topRight)
		if (topLeft)
			res[0] = 5;
		else res[0] = 4;
	else if (topLeft)
		res[0] = 1;
	else res[0] = 0;



	if (bottom)
		if(right)
			if(bottomRight)
				if(bottomLeft)
					res[1] = 13;
				else res[1] = 7;

			else if (bottomLeft)
				res[1] = 15;
			else res[1] = 5;

		else if (bottomRight)
			if (bottomLeft)
				res[1] = 14;
			else res[1] = 12;

		else if (bottomLeft)
			res[1] = 6;

		else res[1] = 4;

	else if (right)
		if (bottomRight)
			if(bottomLeft)
				res[1] = 11;
			else res[1] = 9;

		else if (bottomLeft)
			res[1] = 3;
		else res[1] = 1;

	else if (bottomRight)
		if (bottomLeft)
			res[1] = 10;
		else res[1] = 8;
	else if (bottomLeft)
		res[1] = 2;
	else res[1] = 0;


	// if (!top && !bottom && !left && !right && !bottomLeft && !bottomRight && !topLeft && !topRight)
	// {
	// 	res = getCoordText(1, 16);
	// }
	// else 
	res = getCoordText(res[0], res[1]);

	return res;
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


	// Normal
	glm::vec3 n1;

	glm::vec2 textCoord_caisse = getCoordText(0, 16);

	glm::vec2 occluDefault = getCoordText(0,0);


	// Front
	if(lZPositive)
	{
		n1 = glm::vec3(0.0f, 0.0f, 1.0f);
		m_pRenderer->addNormal(n1);

		m_pRenderer->addTriangle(v1, v2, v3);
		// m_pRenderer->addTexture(glm::vec4(0, 1, 0, 0), glm::vec4(1, 1, 0, 0), glm::vec4(1, 0, 0, 0));
		m_pRenderer->addTexture(glm::vec4(textCoord_caisse + getCoordText(0,1), occluDefault + getCoordText(0,1)),
								 glm::vec4(textCoord_caisse + getCoordText(1,1), occluDefault + getCoordText(1,1)),
								  glm::vec4(textCoord_caisse + getCoordText(1,0), occluDefault + getCoordText(1,0)));
		
		m_pRenderer->addTriangle(v1, v3, v4);
		// m_pRenderer->addTexture(glm::vec4(0, 1, 0, 0), glm::vec4(1, 0, 0, 0), glm::vec4(0, 0, 0, 0));
		m_pRenderer->addTexture(glm::vec4(textCoord_caisse + getCoordText(0,1), occluDefault + getCoordText(0,1)),
								 glm::vec4(textCoord_caisse + getCoordText(1,0), occluDefault + getCoordText(1,0)),
								  glm::vec4(textCoord_caisse + getCoordText(0,0), occluDefault + getCoordText(0,0)));
	}


	// Back
	if(lZNegative)
	{
		n1 = glm::vec3(0.0f, 0.0f, -1.0f);
		m_pRenderer->addNormal(n1);

		m_pRenderer->addTriangle(v5, v6, v7);
		// m_pRenderer->addTexture(glm::vec4(0, 1, 0, 0), glm::vec4(1, 1, 0, 0), glm::vec4(1, 0, 0, 0));
		m_pRenderer->addTexture(glm::vec4(textCoord_caisse + getCoordText(0,1), occluDefault + getCoordText(0,1)),
								 glm::vec4(textCoord_caisse + getCoordText(1,1), occluDefault + getCoordText(1,1)),
								  glm::vec4(textCoord_caisse + getCoordText(1,0), occluDefault + getCoordText(1,0)));
		
		m_pRenderer->addTriangle(v5, v7, v8);
		// m_pRenderer->addTexture(glm::vec4(0, 1, 0, 0), glm::vec4(1, 0, 0, 0), glm::vec4(0, 0, 0, 0));
		m_pRenderer->addTexture(glm::vec4(textCoord_caisse + getCoordText(0,1), occluDefault + getCoordText(0,1)),
								 glm::vec4(textCoord_caisse + getCoordText(1,0), occluDefault + getCoordText(1,0)),
								  glm::vec4(textCoord_caisse + getCoordText(0,0), occluDefault + getCoordText(0,0)));
	}

	// Right
	if(lXPositive)
	{
		n1 = glm::vec3(1.0f, 0.0f, 0.0f);
		m_pRenderer->addNormal(n1);

		m_pRenderer->addTriangle(v2, v5, v8);
		// m_pRenderer->addTexture(glm::vec4(0, 1, 0, 0), glm::vec4(1, 1, 0, 0), glm::vec4(1, 0, 0, 0));
		m_pRenderer->addTexture(glm::vec4(textCoord_caisse + getCoordText(0,1), occluDefault + getCoordText(0,1)),
								 glm::vec4(textCoord_caisse + getCoordText(1,1), occluDefault + getCoordText(1,1)),
								  glm::vec4(textCoord_caisse + getCoordText(1,0), occluDefault + getCoordText(1,0)));
		
		m_pRenderer->addTriangle(v2, v8, v3);
		// m_pRenderer->addTexture(glm::vec4(0, 1, 0, 0), glm::vec4(1, 0, 0, 0), glm::vec4(0, 0, 0, 0));
		m_pRenderer->addTexture(glm::vec4(textCoord_caisse + getCoordText(0,1), occluDefault + getCoordText(0,1)),
								 glm::vec4(textCoord_caisse + getCoordText(1,0), occluDefault + getCoordText(1,0)),
								  glm::vec4(textCoord_caisse + getCoordText(0,0), occluDefault + getCoordText(0,0)));
	}


	// left
	if(lXNegative)
	{
		n1 = glm::vec3(-1.0f, 0.0f, 0.0f);
		m_pRenderer->addNormal(n1);

		m_pRenderer->addTriangle(v6, v1, v4);
		// m_pRenderer->addTexture(glm::vec4(0, 1, 0, 0), glm::vec4(1, 1, 0, 0), glm::vec4(1, 0, 0, 0));
		m_pRenderer->addTexture(glm::vec4(textCoord_caisse + getCoordText(0,1), occluDefault + getCoordText(0,1)),
								 glm::vec4(textCoord_caisse + getCoordText(1,1), occluDefault + getCoordText(1,1)),
								  glm::vec4(textCoord_caisse + getCoordText(1,0), occluDefault + getCoordText(1,0)));
		
		m_pRenderer->addTriangle(v6, v4, v7);
		// m_pRenderer->addTexture(glm::vec4(0, 1, 0, 0), glm::vec4(1, 0, 0, 0), glm::vec4(0, 0, 0, 0));
		m_pRenderer->addTexture(glm::vec4(textCoord_caisse + getCoordText(0,1), occluDefault + getCoordText(0,1)),
								 glm::vec4(textCoord_caisse + getCoordText(1,0), occluDefault + getCoordText(1,0)),
								  glm::vec4(textCoord_caisse + getCoordText(0,0), occluDefault + getCoordText(0,0)));
	}



	// Top
	if(lYPositive)
	{
		glm::vec2 text_occlu = getOcclusionCoordText(getAdjacentMap(x, y, z));	

		n1 = glm::vec3(0.0f, 1.0f, 0.0f);
		m_pRenderer->addNormal(n1);

		m_pRenderer->addTriangle(v4, v3, v8);
		// m_pRenderer->addTexture(glm::vec2(0, 1), glm::vec2(1, 1), glm::vec2(1, 0));
		m_pRenderer->addTexture(glm::vec4(textCoord_caisse + getCoordText(0,1), text_occlu + getCoordText(0,1)),
								 glm::vec4(textCoord_caisse + getCoordText(1,1),  text_occlu + getCoordText(1,1)),
								  glm::vec4(textCoord_caisse + getCoordText(1,0), text_occlu + getCoordText(1,0)));
		
		m_pRenderer->addTriangle(v4, v8, v7);
		// m_pRenderer->addTexture(glm::vec2(0, 1), glm::vec2(1, 0), glm::vec2(0, 0));
		m_pRenderer->addTexture(glm::vec4(textCoord_caisse + getCoordText(0,1), text_occlu + getCoordText(0,1)),
								 glm::vec4(textCoord_caisse + getCoordText(1,0), text_occlu + getCoordText(1,0)),
								  glm::vec4(textCoord_caisse + getCoordText(0,0), text_occlu + getCoordText(0,0)));

	}

	

	// Bottom
	if(lYNegative)
	{
		n1 = glm::vec3(0.0f, -1.0f, 0.0f);
		m_pRenderer->addNormal(n1);

		m_pRenderer->addTriangle(v6, v5, v2);
		m_pRenderer->addTexture(glm::vec4(textCoord_caisse + getCoordText(0,1), occluDefault + getCoordText(0,1)),
								 glm::vec4(textCoord_caisse + getCoordText(1,1), occluDefault + getCoordText(1,1)),
								  glm::vec4(textCoord_caisse + getCoordText(1,0), occluDefault + getCoordText(1,0)));
		
		m_pRenderer->addTriangle(v6, v2, v1);
		// m_pRenderer->addTexture(glm::vec4(0, 1, 0, 0), glm::vec4(1, 0, 0, 0), glm::vec4(0, 0, 0, 0));
		m_pRenderer->addTexture(glm::vec4(textCoord_caisse + getCoordText(0,1), occluDefault + getCoordText(0,1)),
								 glm::vec4(textCoord_caisse + getCoordText(1,0), occluDefault + getCoordText(1,0)),
								  glm::vec4(textCoord_caisse + getCoordText(0,0), occluDefault + getCoordText(0,0)));
	}

}