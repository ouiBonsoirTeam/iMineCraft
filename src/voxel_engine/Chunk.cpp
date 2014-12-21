#include "Chunk.hpp"
#include <glimac/glm.hpp>


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
	for (int x = 0; x < CHUNK_SIZE; x++)
	{
		for (int y = 0; y < CHUNK_SIZE; y++)
		{
			for (int z = 0; z < CHUNK_SIZE; z++)
			{
				// Init a sphere
				if (sqrt((float) (x-CHUNK_SIZE/2)*(x-CHUNK_SIZE/2) + (y-CHUNK_SIZE/2)*(y-CHUNK_SIZE/2) + (z-CHUNK_SIZE/2)*(z-CHUNK_SIZE/2)) <= CHUNK_SIZE/2.0)
				{
					m_pBlocks[x][y][z].setActive();
				}
			}
		}		
	}
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
		m_pRenderer->addTexture(glm::vec2(0, 1), glm::vec2(1, 1), glm::vec2(1, 0));
		
		m_pRenderer->addTriangle(v4, v8, v7);
		m_pRenderer->addTexture(glm::vec2(0, 1), glm::vec2(1, 0), glm::vec2(0, 0));
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