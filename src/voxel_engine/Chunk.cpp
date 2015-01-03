#include "Chunk.hpp"
#include <glimac/glm.hpp>
#include <iostream>
#include <glimac/PerlinNoise.hpp>

// Constructor
Chunk::Chunk()
{}

Chunk::Chunk(glm::vec3 position)
{
    m_position = position;
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
    // Create the blocks A MODIFIER CAR DEJA FAIT DANS LE SETUP
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
	m_pBlocks[15][2][7].setActive();
	m_pBlocks[13][1][6].setActive();
	m_pBlocks[4][1][8].setActive();
	m_pBlocks[12][1][10].setActive();
	m_pBlocks[12][1][11].setActive();
	m_pBlocks[13][1][12].setActive();
	m_pBlocks[10][1][2].setActive();
	m_pBlocks[13][1][9].setActive();
	m_pBlocks[13][1][11].setActive();


	m_pBlocks[2][4][2].setActive();
	m_pBlocks[2][5][2].setActive();
	m_pBlocks[3][5][3].setActive();
	m_pBlocks[3][5][2].setActive();


	m_pBlocks[8][1][9].setActive();
	m_pBlocks[7][1][9].setActive();
	m_pBlocks[9][1][9].setActive();
	m_pBlocks[7][1][7].setActive();
}

bool Chunk::blockExist(int x, int y, int z)
{
	return !(x < 0 || x > CHUNK_SIZE - 1 || y < 0 || y > CHUNK_SIZE - 1 || z < 0 || z > CHUNK_SIZE - 1);
}

bool Chunk::blockExist(glm::vec3 vec)
{
	return !(vec[0] < 0 || vec[0] > CHUNK_SIZE - 1 || vec[1] < 0 || vec[1] > CHUNK_SIZE - 1 || vec[2] < 0 || vec[2] > CHUNK_SIZE - 1);
}

glm::mat3 Chunk::getAdjacentMap(int x, int y, int z, int adjacent_look)
{
	glm::vec3 leftTop, top, topRight, right, rightBottom, bottom, bottomLeft, left;

	switch(adjacent_look)
	{
		case LOOK_TOP:
			leftTop 	= glm::vec3(x - 1,	y + 1,	z - 1 	);
			top 		= glm::vec3(x,		y + 1,	z - 1 	);
			topRight 	= glm::vec3(x + 1, 	y + 1, 	z - 1 	);
			right 		= glm::vec3(x + 1, 	y + 1, 	z 		);
			rightBottom = glm::vec3(x + 1, 	y + 1, 	z + 1 	);
			bottom 		= glm::vec3(x, 		y + 1,	z + 1 	);
			bottomLeft 	= glm::vec3(x - 1, 	y + 1, 	z + 1 	);
			left 		= glm::vec3(x - 1, 	y + 1, 	z 		);
		break;

		case LOOK_BACK:
			leftTop 	= glm::vec3(x + 1, y + 1, z - 1);
			top 		= glm::vec3(x, y + 1, z - 1);
			topRight 	= glm::vec3(x - 1, y + 1, z - 1);
			right 		= glm::vec3(x - 1, y, z - 1);
			rightBottom = glm::vec3(x - 1, y - 1, z - 1);
			bottom 		= glm::vec3(x, y - 1, z - 1);
			bottomLeft 	= glm::vec3(x + 1, y - 1, z - 1);
			left 		= glm::vec3(x + 1, y, z - 1);
		break;

		case LOOK_RIGHT:
			leftTop 	= glm::vec3(x + 1, y + 1, z + 1);
			top 		= glm::vec3(x + 1, y + 1, z);
			topRight	= glm::vec3(x + 1, y + 1, z - 1);
			right 		= glm::vec3(x + 1, y, z - 1);
			rightBottom = glm::vec3(x + 1, y - 1, z - 1);
			bottom 		= glm::vec3(x + 1, y - 1, z);
			bottomLeft 	= glm::vec3(x + 1, y - 1, z + 1);
			left 		= glm::vec3(x + 1, y, z + 1);
		break;

		case LOOK_FRONT:
			leftTop 	= glm::vec3(x - 1, y + 1, z + 1);
			top 		= glm::vec3(x, y + 1, z + 1);
			topRight 	= glm::vec3(x + 1, y + 1, z + 1);
			right 		= glm::vec3(x + 1, y, z + 1);
			rightBottom = glm::vec3(x + 1, y - 1, z + 1);
			bottom 		= glm::vec3(x, y - 1, z + 1);
			bottomLeft 	= glm::vec3(x - 1, y - 1, z + 1);
			left 		= glm::vec3(x - 1, y, z + 1);
		break;

		case LOOK_LEFT:
			leftTop 	= glm::vec3(x - 1, y + 1, z - 1);
			top 		= glm::vec3(x - 1, y + 1, z);
			topRight	= glm::vec3(x - 1, y + 1, z + 1);
			right 		= glm::vec3(x - 1, y, z + 1);
			rightBottom = glm::vec3(x - 1, y - 1, z + 1);
			bottom 		= glm::vec3(x - 1, y - 1, z);
			bottomLeft 	= glm::vec3(x - 1, y - 1, z - 1);
			left 		= glm::vec3(x - 1, y, z - 1);
		break;

		case LOOK_BOTTOM:
			leftTop 	= glm::vec3(x - 1, 	y - 1, 	z + 1 	);
			top 		= glm::vec3(x, 		y - 1,	z + 1 	);
			topRight 	= glm::vec3(x + 1, 	y - 1, 	z + 1 	);
			right 		= glm::vec3(x + 1, 	y - 1, 	z 		);
			rightBottom = glm::vec3(x + 1, 	y - 1, 	z - 1 	);
			bottom 		= glm::vec3(x,		y - 1,	z - 1 	);
			bottomLeft 	= glm::vec3(x - 1,	y - 1,	z - 1 	);
			left 		= glm::vec3(x - 1, 	y - 1, 	z 		);
		break;

		default:
		break;
	}

    glm::mat3 adjacentMap = glm::mat3(0);  

    //leftTop
    if(blockExist(leftTop)		&&		m_pBlocks[ (int) leftTop[0] ][ (int) leftTop[1] ][ (int) leftTop[2] ].isActive())
        adjacentMap[0][0] = 1;

    //top
    if(blockExist(top)			&&		m_pBlocks[ (int) top[0] ][ (int) top[1] ][ (int) top[2] ].isActive())
        adjacentMap[0][1] = 1;

     //topRight
    if(blockExist(topRight)		&&		m_pBlocks[ (int) topRight[0] ][ (int) topRight[1] ][ (int) topRight[2] ].isActive())
        adjacentMap[0][2] = 1;

     //right
    if(blockExist(right)		&&		m_pBlocks[ (int) right[0] ][ (int) right[1] ][ (int) right[2] ].isActive())
        adjacentMap[1][2] = 1;

    //rightBottom
    if(blockExist(rightBottom)	&&		m_pBlocks[ (int) rightBottom[0] ][ (int) rightBottom[1] ][ (int) rightBottom[2] ].isActive())
        adjacentMap[2][2] = 1;

    //bottom
    if(blockExist(bottom)		&&		m_pBlocks[ (int) bottom[0] ][ (int) bottom[1] ][ (int) bottom[2] ].isActive())
        adjacentMap[2][1] = 1;

    //bottomLeft
    if(blockExist(bottomLeft)	&&		m_pBlocks[ (int) bottomLeft[0] ][ (int) bottomLeft[1] ][ (int) bottomLeft[2] ].isActive())
        adjacentMap[2][0] = 1;

    //left
    if(blockExist(left)			&&		m_pBlocks[ (int) left[0] ][ (int) left[1] ][ (int) left[2] ].isActive())
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

glm::vec2 Chunk::computeCoordText(const int & x, const int & y, const bool crop)
{
	glm::vec2 size_text = glm::vec2((1.0 / TAILLE_X_TEXTURE), (1.0 / TAILLE_Y_TEXTURE));

	if(crop)
	{
		float cropX = size_text[0] * 0.015;
		float cropY = size_text[1] * 0.015;

		float signeX, signeY;
		if (x == 0)
			signeX = 1;
		else signeX = -1;
		if (y == 0)
			signeY = 1;
		else signeY = -1;

		return glm::vec2(x * size_text[0] + cropX * signeX, y * size_text[1] + cropY * signeY);
	}
	return	glm::vec2(x * size_text[0], y * size_text[1]);
}

glm::vec2 Chunk::getOcclusionCoordText(glm::mat3 adjacentMap)
{

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

	res = computeCoordText(res[0], res[1]);

	return res;
}

void Chunk::createMesh()
{
    for (int x = 0; x < CHUNK_SIZE; x++)
    {
        for (int y = 0; y < CHUNK_SIZE; y++)
        {
            // SPHERE
            for (int z = 0; z < CHUNK_SIZE; z++)
            {
                if (sqrt((float) (x-CHUNK_SIZE/2)*(x-CHUNK_SIZE/2) + (y-CHUNK_SIZE/2)*(y-CHUNK_SIZE/2) + (z-CHUNK_SIZE/2)*(z-CHUNK_SIZE/2)) <= CHUNK_SIZE/2)
                    {
                        m_pBlocks[x][y][z].setActive();
                    }
                else
                    continue;
            }
        }
    }
}


void Chunk::createLandscape(PerlinNoise *pn)
{
    for(int x = 0; x < CHUNK_SIZE; ++x)
    {
        for(int z = 0; z < CHUNK_SIZE; ++z)
        {
            // Use the noise library to get the height value of x, z
            int height = (int) glm::round(pn->GetHeight(m_position[0] * CHUNK_SIZE + x, m_position[2] * CHUNK_SIZE + z));

            int min_chunk_y = m_position[1] * CHUNK_SIZE;

            if(height < -24)
            	height = -24;

            if(height >= min_chunk_y)
            {
            	int end;
            	if(height < min_chunk_y + CHUNK_SIZE)
            		end = height;
            	else
            		end = min_chunk_y + CHUNK_SIZE - 1;

	            for (int y = min_chunk_y; y <= end; ++y)
	            {
	            	int y_bis = y - min_chunk_y;
	                m_pBlocks[x][y_bis][z].setActive();

	                if(height <= -24)
	               		m_pBlocks[x][y_bis][z].setType(BlockType_Lava);
	               	else if(y < -15)
	               		m_pBlocks[x][y_bis][z].setType(BlockType_Rock);
	               	else if(y < 15)
	               	{
	               		if(y == height)
	               			m_pBlocks[x][y_bis][z].setType(BlockType_Grass);
	               		else
	               			m_pBlocks[x][y_bis][z].setType(BlockType_Earth);
	               	}
	               	else if(y == 15)
	               		m_pBlocks[x][y_bis][z].setType(BlockType_1st_Snow);
	               	else if(y <	 36)
	               		m_pBlocks[x][y_bis][z].setType(BlockType_Snow);
	               	else
	               		m_pBlocks[x][y_bis][z].setType(BlockType_Ice);
	            }
	        }
        }
    }
}

void Chunk::render(LightsProgram &program, const glm::mat4 viewMatrix, GLuint idTexture)
{
	m_pRenderer->setVao();

    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.f), glm::vec3(m_position[0] * CHUNK_SIZE, m_position[1] * CHUNK_SIZE, m_position[2] * CHUNK_SIZE));
    glm::mat4 modelViewMatrix = viewMatrix * modelMatrix;

    // A sortir de la classe : Identique dans tout le programme
    glm::mat4 projMatrix = glm::perspective(glm::radians(70.f), 800.f/600.f, 0.1f, 100.f);

    glm::mat4 modelViewProjMatrix = projMatrix * modelViewMatrix;

   // Normale
    glm::mat4 normalMatrix = glm::transpose(glm::inverse(modelViewMatrix));

    glUniform1i(program.uTexture, 0);
    glUniformMatrix4fv(program.uMVMatrix, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
    glUniformMatrix4fv(program.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(modelViewProjMatrix));
    glUniformMatrix4fv(program.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(normalMatrix));

	m_pRenderer->renderMesh(idTexture);
}

void Chunk::update(){}

void Chunk::createCube(	const int &x, const int &y, const int &z, const bool & lXNegative, const bool &lXPositive,
						const bool &lYNegative, const bool &lYPositive, const bool &lZNegative, const bool &lZPositive, const BlockType &blockType)
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

	glm::vec2 textCoord_up = computeCoordText(0, 16 + blockType);
	glm::vec2 textCoord_side = computeCoordText(1, 16 + blockType);
	glm::vec2 textCoord_bottom = computeCoordText(2, 16 + blockType);

	// Front
	if(lZPositive)
	{
		glm::vec2 text_occlu = getOcclusionCoordText(getAdjacentMap(x, y, z, LOOK_FRONT));

		n1 = glm::vec3(0.0f, 0.0f, 1.0f);
		m_pRenderer->addNormal(n1);

		m_pRenderer->addTriangle(v1, v2, v3);
		// m_pRenderer->addTexture(glm::vec4(0, 1, 0, 0), glm::vec4(1, 1, 0, 0), glm::vec4(1, 0, 0, 0));
		m_pRenderer->addTexture(glm::vec4(textCoord_side + computeCoordText(0,1,true), 
											text_occlu + computeCoordText(0,1,true)),
								 glm::vec4(textCoord_side + computeCoordText(1,1,true), 
								 			text_occlu + computeCoordText(1,1,true)),
								  glm::vec4(textCoord_side + computeCoordText(1,0,true), 
								  			text_occlu + computeCoordText(1,0,true)));
		
		m_pRenderer->addTriangle(v1, v3, v4);
		// m_pRenderer->addTexture(glm::vec4(0, 1, 0, 0), glm::vec4(1, 0, 0, 0), glm::vec4(0, 0, 0, 0));
		m_pRenderer->addTexture(glm::vec4(textCoord_side + computeCoordText(0,1,1), 
											text_occlu + computeCoordText(0,1,1)),
								 glm::vec4(textCoord_side + computeCoordText(1,0,true), 
								 			text_occlu + computeCoordText(1,0,true)),
								  glm::vec4(textCoord_side + computeCoordText(0,0,true),
								  			text_occlu + computeCoordText(0,0,true)));
	}


	// Back
	if(lZNegative)
	{
		glm::vec2 text_occlu = getOcclusionCoordText(getAdjacentMap(x, y, z, LOOK_BACK));

		n1 = glm::vec3(0.0f, 0.0f, -1.0f);
		m_pRenderer->addNormal(n1);

		m_pRenderer->addTriangle(v5, v6, v7);
		// m_pRenderer->addTexture(glm::vec4(0, 1, 0, 0), glm::vec4(1, 1, 0, 0), glm::vec4(1, 0, 0, 0));
		m_pRenderer->addTexture(glm::vec4(textCoord_side + computeCoordText(0,1,true), 
											text_occlu + computeCoordText(0,1,true)),
								 glm::vec4(textCoord_side + computeCoordText(1,1,true), 
								 			text_occlu + computeCoordText(1,1,true)),
								  glm::vec4(textCoord_side + computeCoordText(1,0,true), 
								  			text_occlu + computeCoordText(1,0,true)));
		
		m_pRenderer->addTriangle(v5, v7, v8);
		// m_pRenderer->addTexture(glm::vec4(0, 1, 0, 0), glm::vec4(1, 0, 0, 0), glm::vec4(0, 0, 0, 0));
		m_pRenderer->addTexture(glm::vec4(textCoord_side + computeCoordText(0,1,true), 
											text_occlu + computeCoordText(0,1,true)),
								 glm::vec4(textCoord_side + computeCoordText(1,0,true), 
								 			text_occlu + computeCoordText(1,0,true)),
								  glm::vec4(textCoord_side + computeCoordText(0,0,true), 
								  			text_occlu + computeCoordText(0,0,true)));
	}

	// Right
	if(lXPositive)
	{
		glm::vec2 text_occlu = getOcclusionCoordText(getAdjacentMap(x, y, z, LOOK_RIGHT));

		n1 = glm::vec3(1.0f, 0.0f, 0.0f);
		m_pRenderer->addNormal(n1);

		m_pRenderer->addTriangle(v2, v5, v8);
		// m_pRenderer->addTexture(glm::vec4(0, 1, 0, 0), glm::vec4(1, 1, 0, 0), glm::vec4(1, 0, 0, 0));
		m_pRenderer->addTexture(glm::vec4(textCoord_side + computeCoordText(0,1,true), 
											text_occlu + computeCoordText(0,1,true)),
								 glm::vec4(textCoord_side + computeCoordText(1,1,true), 
								 			text_occlu + computeCoordText(1,1,true)),
								  glm::vec4(textCoord_side + computeCoordText(1,0,true), 
								  			text_occlu + computeCoordText(1,0,true)));
		
		m_pRenderer->addTriangle(v2, v8, v3);
		// m_pRenderer->addTexture(glm::vec4(0, 1, 0, 0), glm::vec4(1, 0, 0, 0), glm::vec4(0, 0, 0, 0));
		m_pRenderer->addTexture(glm::vec4(textCoord_side + computeCoordText(0,1,true), 
											text_occlu + computeCoordText(0,1,true)),
								 glm::vec4(textCoord_side + computeCoordText(1,0,true), 
								 			text_occlu + computeCoordText(1,0,true)),
								  glm::vec4(textCoord_side + computeCoordText(0,0,true), 
								  			text_occlu + computeCoordText(0,0,true)));
	}


	// left
	if(lXNegative)
	{
		glm::vec2 text_occlu = getOcclusionCoordText(getAdjacentMap(x, y, z, LOOK_LEFT));

		n1 = glm::vec3(-1.0f, 0.0f, 0.0f);
		m_pRenderer->addNormal(n1);

		m_pRenderer->addTriangle(v6, v1, v4);
		// m_pRenderer->addTexture(glm::vec4(0, 1, 0, 0), glm::vec4(1, 1, 0, 0), glm::vec4(1, 0, 0, 0));
		m_pRenderer->addTexture(glm::vec4(textCoord_side + computeCoordText(0,1,true), 
											text_occlu + computeCoordText(0,1,true)),
								 glm::vec4(textCoord_side + computeCoordText(1,1,true), 
								 			text_occlu + computeCoordText(1,1,true)),
								  glm::vec4(textCoord_side + computeCoordText(1,0,true), 
								  			text_occlu + computeCoordText(1,0,true)));
		
		m_pRenderer->addTriangle(v6, v4, v7);
		// m_pRenderer->addTexture(glm::vec4(0, 1, 0, 0), glm::vec4(1, 0, 0, 0), glm::vec4(0, 0, 0, 0));
		m_pRenderer->addTexture(glm::vec4(textCoord_side + computeCoordText(0,1,true), 
											text_occlu + computeCoordText(0,1,true)),
								 glm::vec4(textCoord_side + computeCoordText(1,0,true), 
								 			text_occlu + computeCoordText(1,0, true)),
								  glm::vec4(textCoord_side + computeCoordText(0,0,true), 
								  			text_occlu + computeCoordText(0,0,true)));
	}

	// Top
	if(lYPositive)
	{
		glm::vec2 text_occlu = getOcclusionCoordText(getAdjacentMap(x, y, z, LOOK_TOP));	

		n1 = glm::vec3(0.0f, 1.0f, 0.0f);
		m_pRenderer->addNormal(n1);

		m_pRenderer->addTriangle(v4, v3, v8);
		// m_pRenderer->addTexture(glm::vec2(0, 1), glm::vec2(1, 1), glm::vec2(1, 0));
		m_pRenderer->addTexture(glm::vec4(textCoord_up + computeCoordText(0,1,true), 
											text_occlu + computeCoordText(0,1,true)),
								 glm::vec4(textCoord_up + computeCoordText(1,1,true), 
								 			text_occlu + computeCoordText(1,1,true)),
								  glm::vec4(textCoord_up + computeCoordText(1,0,true), 
								  			text_occlu + computeCoordText(1,0,true)));
		
		m_pRenderer->addTriangle(v4, v8, v7);
		// m_pRenderer->addTexture(glm::vec2(0, 1), glm::vec2(1, 0), glm::vec2(0, 0));
		m_pRenderer->addTexture(glm::vec4(textCoord_up + computeCoordText(0,1,true), 
											text_occlu + computeCoordText(0,1,true)),
								 glm::vec4(textCoord_up + computeCoordText(1,0,true), 
								 			text_occlu + computeCoordText(1,0,true)),
								  glm::vec4(textCoord_up + computeCoordText(0,0,true), 
								  			text_occlu + computeCoordText(0,0,true)));

	}

	// Bottom
	if(lYNegative)
	{
		glm::vec2 text_occlu = getOcclusionCoordText(getAdjacentMap(x, y, z, LOOK_BOTTOM));

		n1 = glm::vec3(0.0f, -1.0f, 0.0f);
		m_pRenderer->addNormal(n1);

		m_pRenderer->addTriangle(v6, v5, v2);
		m_pRenderer->addTexture(glm::vec4(textCoord_bottom + computeCoordText(0,1,true), 
											text_occlu + computeCoordText(0,1,true)),
								 glm::vec4(textCoord_bottom + computeCoordText(1,1,true), 
								 			text_occlu + computeCoordText(1,1,true)),
								  glm::vec4(textCoord_bottom + computeCoordText(1,0,true), 
								  			text_occlu + computeCoordText(1,0,true)));
		
		m_pRenderer->addTriangle(v6, v2, v1);
		// m_pRenderer->addTexture(glm::vec4(0, 1, 0, 0), glm::vec4(1, 0, 0, 0), glm::vec4(0, 0, 0, 0));
		m_pRenderer->addTexture(glm::vec4(textCoord_bottom + computeCoordText(0,1,true), 
											text_occlu + computeCoordText(0,1,true)),
								 glm::vec4(textCoord_bottom + computeCoordText(1,0,true), 
								 			text_occlu + computeCoordText(1,0,true)),
								  glm::vec4(textCoord_bottom + computeCoordText(0,0,true), 
								  			text_occlu + computeCoordText(0,0,true)));
	}

}

bool Chunk::isLoaded()
{
    return m_loaded;
}

bool Chunk::isSetup()
{
    return m_setup;
}

void Chunk::load(const Json::Value &chunkData)
{
    m_blocksData = chunkData;
    m_loaded = true;
}

void Chunk::setup(PerlinNoise *pn)
{
    // Create the blocks
    if (m_loaded)
    {
        m_pBlocks = new Block**[CHUNK_SIZE];

        for(int i = 0; i < CHUNK_SIZE; ++i)
        {
            m_pBlocks[i] = new Block*[CHUNK_SIZE];

            for(int j = 0; j < CHUNK_SIZE; ++j)
            {
                m_pBlocks[i][j] = new Block[CHUNK_SIZE];

                for (int k = 0; k < CHUNK_SIZE; ++k)
                {
                    if (m_blocksData["block"][i][j][k]["active"] == true)
                        m_pBlocks[i][j][k].setActive();

                    m_pBlocks[i][j][k].setType(m_blocksData["block"][i][j][k]["type"].asInt());
                }
            }
        }

        m_blocksData.clear();
    }
    else
    {
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

        createLandscape(pn);

        m_loaded = true;
    }

    m_pRenderer = new OpenGLRenderer;

    m_setup = true; 
}

void Chunk::buildMesh()
{
    bool lDefault = true;
    m_pRenderer->clean();

    for (int x = 0; x < CHUNK_SIZE; x++)
    {
        for (int y = 0; y < CHUNK_SIZE; y++)
        {
            for (int z = 0; z < CHUNK_SIZE; z++)
            {
                if(m_pBlocks[x][y][z].isActive() == false)
                	continue;

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

                createCube(x, y, z, lXNegative, lXPositive, lYNegative, lYPositive, lZNegative, lZPositive, m_pBlocks[x][y][z].getType());
            }
        }
    }

    m_pRenderer->finishVbo();
}

bool Chunk::destructBlock(const int &x, const int &y, const int &z, BlockType & bt)
{
	if (m_pBlocks[x][y][z].isActive()){
		bt = m_pBlocks[x][y][z].getType();
		m_pBlocks[x][y][z].setInactive();
		return true;
	}
	return false;

}

bool Chunk::constructBlock(const int &x, const int &y, const int &z, BlockType type)
{
	
	m_pBlocks[x][y][z].setActive();
	m_pBlocks[x][y][z].setType(type);
	return true;
}

void Chunk::unload()
{
    // Save à faire

    delete this;
}

void Chunk::updateShouldRenderFlags()
{
    int numVerts;
    m_pRenderer->getMeshInformation(numVerts);

    if(numVerts == 0)
    {
        m_emptyChunk = true;
    }
}