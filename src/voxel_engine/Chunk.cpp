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

void Chunk::createMesh()
{
    for (int x = 0; x < CHUNK_SIZE; x++)
    {
        for (int y = 0; y < CHUNK_SIZE; y++)
        {
            for (int z = 0; z < CHUNK_SIZE; z++)
            {
                if(m_pBlocks[x][y][z].isActive() == false)
                    continue;

                createCube(x, y, z);
            }
        }
    }

    m_pRenderer->finishVboPosition();
    m_pRenderer->finishVboTexture();
}

void Chunk::render(GeneralProgram &program, const glm::mat4 viewMatrix, GLuint idTexture)
{
	m_pRenderer->setVao();

	m_pRenderer->draw(program, viewMatrix, idTexture);
}

void Chunk::update(){}

void Chunk::createCube(const int &x, const int &y, const int &z)
{
    glm::vec3 v1(x-Block::BLOCK_RENDER_SIZE, y-Block::BLOCK_RENDER_SIZE, z+Block::BLOCK_RENDER_SIZE);
    glm::vec3 v2(x+Block::BLOCK_RENDER_SIZE, y-Block::BLOCK_RENDER_SIZE, z+Block::BLOCK_RENDER_SIZE);
    glm::vec3 v3(x+Block::BLOCK_RENDER_SIZE, y+Block::BLOCK_RENDER_SIZE, z+Block::BLOCK_RENDER_SIZE);
    glm::vec3 v4(x-Block::BLOCK_RENDER_SIZE, y+Block::BLOCK_RENDER_SIZE, z+Block::BLOCK_RENDER_SIZE);
    glm::vec3 v5(x+Block::BLOCK_RENDER_SIZE, y-Block::BLOCK_RENDER_SIZE, z-Block::BLOCK_RENDER_SIZE);
    glm::vec3 v6(x-Block::BLOCK_RENDER_SIZE, y-Block::BLOCK_RENDER_SIZE, z-Block::BLOCK_RENDER_SIZE);
    glm::vec3 v7(x-Block::BLOCK_RENDER_SIZE, y+Block::BLOCK_RENDER_SIZE, z-Block::BLOCK_RENDER_SIZE);
    glm::vec3 v8(x+Block::BLOCK_RENDER_SIZE, y+Block::BLOCK_RENDER_SIZE, z-Block::BLOCK_RENDER_SIZE);

    // A FAIRE NORMALE
    // glm::vec3 n1;

    // float r = 1.0f;
    // float g = 1.0f;
    // float b = 1.0f;
    // float a = 1.0f;


    // Front
    //n1 = glm::vec3(0.0f, 0.0f, 1.0f);

    m_pRenderer->addTriangle(v1, v2, v3);
        m_pRenderer->addTexture(glm::vec2(v1), glm::vec2(v2), glm::vec2(v3));
    m_pRenderer->addTriangle(v1, v3, v4);
        m_pRenderer->addTexture(glm::vec2(v1), glm::vec2(v3), glm::vec2(v4));


    // Back
    //n1 = glm::vec3(0.0f, 0.0f, -1.0f);

    m_pRenderer->addTriangle(v5, v6, v7);
        m_pRenderer->addTexture(glm::vec2(v5), glm::vec2(v6), glm::vec2(v7));

    m_pRenderer->addTriangle(v5, v7, v8);
        m_pRenderer->addTexture(glm::vec2(v5), glm::vec2(v7), glm::vec2(v8));


    // Right
    //n1 = glm::vec3(1.0f, 0.0f, 0.0f);

    m_pRenderer->addTriangle(v2, v5, v8);
        m_pRenderer->addTexture(glm::vec2(v2.y, v2.z), glm::vec2(v5.y, v5.z), glm::vec2(v8.y, v8.z));
    m_pRenderer->addTriangle(v2, v8, v3);
        m_pRenderer->addTexture(glm::vec2(v2.y, v2.z), glm::vec2(v8.y, v8.z), glm::vec2(v3.y, v3.z));


    // left
    // n1 = glm::vec3(-1.0f, 0.0f, 0.0f);

    m_pRenderer->addTriangle(v6, v1, v4);
        m_pRenderer->addTexture(glm::vec2(v6.y, v6.z), glm::vec2(v1.y, v1.z), glm::vec2(v4.y, v4.z));
    m_pRenderer->addTriangle(v6, v4, v7);
        m_pRenderer->addTexture(glm::vec2(v6.y, v6.z), glm::vec2(v4.y, v4.z), glm::vec2(v7.y, v7.z));



    // Top
    // n1 = glm::vec3(0.0f, 1.0f, 0.0f);

    m_pRenderer->addTriangle(v4, v3, v8);
        m_pRenderer->addTexture(glm::vec2(v4.x, v4.z), glm::vec2(v3.x, v3.z), glm::vec2(v8.x, v8.z));
    m_pRenderer->addTriangle(v4, v8, v7);
        m_pRenderer->addTexture(glm::vec2(v4.x, v4.z), glm::vec2(v3.x, v3.z), glm::vec2(v7.x, v7.z));


    // Bottom
    // n1 = glm::vec3(0.0f, -1.0f, 0.0f);

    m_pRenderer->addTriangle(v6, v5, v2);
        m_pRenderer->addTexture(glm::vec2(v6.x, v6.z), glm::vec2(v5.x, v5.z), glm::vec2(v2.x, v2.z));
    m_pRenderer->addTriangle(v6, v2, v1);
        m_pRenderer->addTexture(glm::vec2(v6.x, v6.z), glm::vec2(v2.x, v2.z), glm::vec2(v1.x, v1.z));

}