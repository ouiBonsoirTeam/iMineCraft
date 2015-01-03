#include "OpenGLRenderer.hpp"
#include "Block.hpp"
#include <iostream>

// Constructor
OpenGLRenderer::OpenGLRenderer(){
	glGenBuffers(1, &m_buffer[INDEX_BUFFER]);
	glGenBuffers(1, &m_buffer[CUBE_VERT_VB]);
	glGenBuffers(1, &m_buffer[POS_VB]);
	glGenBuffers(1, &m_buffer[NORMAL_VB]);
	glGenBuffers(1, &m_buffer[TEXT_COORDS_VB]);

	glm::vec3 v1(-Block::BLOCK_RENDER_SIZE * 0.5, -Block::BLOCK_RENDER_SIZE * 0.5, +Block::BLOCK_RENDER_SIZE * 0.5);
    glm::vec3 v2(+Block::BLOCK_RENDER_SIZE * 0.5, -Block::BLOCK_RENDER_SIZE * 0.5, +Block::BLOCK_RENDER_SIZE * 0.5);
    glm::vec3 v3(+Block::BLOCK_RENDER_SIZE * 0.5, +Block::BLOCK_RENDER_SIZE * 0.5, +Block::BLOCK_RENDER_SIZE * 0.5);
    glm::vec3 v4(-Block::BLOCK_RENDER_SIZE * 0.5, +Block::BLOCK_RENDER_SIZE * 0.5, +Block::BLOCK_RENDER_SIZE * 0.5);
    glm::vec3 v5(+Block::BLOCK_RENDER_SIZE * 0.5, -Block::BLOCK_RENDER_SIZE * 0.5, -Block::BLOCK_RENDER_SIZE * 0.5);
    glm::vec3 v6(-Block::BLOCK_RENDER_SIZE * 0.5, -Block::BLOCK_RENDER_SIZE * 0.5, -Block::BLOCK_RENDER_SIZE * 0.5);
    glm::vec3 v7(-Block::BLOCK_RENDER_SIZE * 0.5, +Block::BLOCK_RENDER_SIZE * 0.5, -Block::BLOCK_RENDER_SIZE * 0.5);
    glm::vec3 v8(+Block::BLOCK_RENDER_SIZE * 0.5, +Block::BLOCK_RENDER_SIZE * 0.5, -Block::BLOCK_RENDER_SIZE * 0.5); 

	// Set a model cube
	m_cube_vertices = {	v1, v2, v3,
						v1, v3, v4,
						v5, v6, v7,
						v5, v7, v8,
						v2, v5, v8,
						v2, v8, v3,
						v6, v1, v4,
						v6, v4, v7,
						v4, v3, v8,
						v4, v8, v7,
						v6, v5, v2,
						v6, v2, v1
					};

	// m_cube_normals = {
	// 					glm::vec3(0.0f, 0.0f, 1.0f),
	// 					glm::vec3(0.0f, 0.0f, -1.0f),
	// 					glm::vec3(1.0f, 0.0f, 0.0f),
	// 					glm::vec3(-1.0f, 0.0f, 0.0f),
	// 					glm::vec3(0.0f, 1.0f, 0.0f),
	// 					glm::vec3(0.0f, -1.0f, 0.0f)
	// 				};

}

// Destructor
OpenGLRenderer::~OpenGLRenderer(){
	glDeleteBuffers(1, &m_buffer[INDEX_BUFFER]);
	glDeleteBuffers(1, &m_buffer[CUBE_VERT_VB]);
	glDeleteBuffers(1, &m_buffer[POS_VB]);
	glDeleteBuffers(1, &m_buffer[NORMAL_VB]);
	glDeleteBuffers(1, &m_buffer[TEXT_COORDS_VB]);
	glDeleteVertexArrays(1, &m_vao);
}

// void OpenGLRenderer::addTriangle(unsigned int index_1, unsigned int index_2, unsigned int index_3){
// 	m_index.push_back(index_1);
// 	m_index.push_back(index_2);
// 	m_index.push_back(index_3);
// }

// void OpenGLRenderer::addTriangle(const glm::vec3 &position_1, const glm::vec3 &position_2, const glm::vec3 &position_3){
// 	m_index.push_back(position_1);
// 	m_index.push_back(position_2);
// 	m_index.push_back(position_3);
// }

void OpenGLRenderer::addPosition(glm::vec3 position){
	m_positions.push_back(position);
}

void OpenGLRenderer::addTexture(glm::vec4 texPos_1, glm::vec4 texPos_2, glm::vec4 texPos_3){
	m_textures.push_back(texPos_1);
	m_textures.push_back(texPos_2);
	m_textures.push_back(texPos_3);
}

void OpenGLRenderer::addNormal(glm::vec3 n){
	m_cube_normals.push_back(n);
}

// void OpenGLRenderer::finishIbo(){
// 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer[INDEX_BUFFER]);

// 	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_index.size()*sizeof(unsigned int), m_index.data(), GL_STATIC_DRAW);

// 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
// }

void OpenGLRenderer::finishVboCubeModel(){
	glBindBuffer(GL_ARRAY_BUFFER, m_buffer[CUBE_VERT_VB]);

    glBufferData(GL_ARRAY_BUFFER, m_cube_vertices.size() * sizeof(glm::vec3), m_cube_vertices.data() , GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGLRenderer::finishVboPosition(){
	glBindBuffer(GL_ARRAY_BUFFER, m_buffer[POS_VB]);

	glBufferData(GL_ARRAY_BUFFER, m_positions.size() * sizeof(glm::vec3), m_positions.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGLRenderer::finishVboTexture(){
	glBindBuffer(GL_ARRAY_BUFFER, m_buffer[TEXT_COORDS_VB]);

	glBufferData(GL_ARRAY_BUFFER, m_textures.size() * sizeof(glm::vec4), m_textures.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGLRenderer::finishVboNormal(){
	glBindBuffer(GL_ARRAY_BUFFER, m_buffer[NORMAL_VB]);

	glBufferData(GL_ARRAY_BUFFER, m_cube_normals.size() * sizeof(glm::vec3), m_cube_normals.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGLRenderer::finishVbo()
{
	//finishIbo();
	finishVboCubeModel();
	finishVboPosition();
	finishVboTexture();
	finishVboNormal();
}

void OpenGLRenderer::setVao(){
	glGenVertexArrays(1, &m_vao);
	
	glBindVertexArray(m_vao);

	glEnableVertexAttribArray(CUBE_VERTEX);

	glBindBuffer(GL_ARRAY_BUFFER, m_buffer[CUBE_VERT_VB]);
	glVertexAttribPointer(CUBE_VERTEX, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (const GLvoid*)(0));

	glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, m_buffer[POS_VB]);
	glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (const GLvoid*)(0));  
    glVertexAttribDivisor(VERTEX_ATTR_POSITION, 1); 

	glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
	glBindBuffer(GL_ARRAY_BUFFER, m_buffer[NORMAL_VB]);
	glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (const GLvoid*)(0));
	glVertexAttribDivisor(VERTEX_ATTR_NORMAL, 1); 

	// if(m_textures.size() != 0)
	// {
	// 	glEnableVertexAttribArray(VERTEX_ATTR_TEXTCOORD);
	// 	glBindBuffer(GL_ARRAY_BUFFER, m_buffer[TEXT_COORDS_VB]);
	// 	glVertexAttribPointer(VERTEX_ATTR_TEXTCOORD, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (const GLvoid*)(0));
	// }
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void OpenGLRenderer::renderMesh(GLuint idTexture){

	
	glBindVertexArray(m_vao);

	// glBindTexture(GL_TEXTURE_2D, idTexture);

	//glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());

	glDrawArraysInstanced(GL_TRIANGLES, 0, m_cube_vertices.size(), m_positions.size());


	// glBindTexture(GL_TEXTURE_2D, 0)

	glBindVertexArray(0);
}

void OpenGLRenderer::getMeshInformation(int &numVerts){
	numVerts = m_positions.size();
}