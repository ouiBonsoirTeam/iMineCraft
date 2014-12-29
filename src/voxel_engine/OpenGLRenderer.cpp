#include "OpenGLRenderer.hpp"

// Constructor
OpenGLRenderer::OpenGLRenderer(){
	glGenBuffers(1, &m_vbo);
}

// Destructor
OpenGLRenderer::~OpenGLRenderer(){
	glDeleteBuffers(1, &m_vbo);
	glDeleteVertexArrays(1, &m_vao);
}

void OpenGLRenderer::addTriangle(glm::vec3 position_1, glm::vec3 position_2, glm::vec3 position_3){
	m_vertices.push_back(position_1);
	m_vertices.push_back(position_2);
	m_vertices.push_back(position_3);
}

void OpenGLRenderer::finishVbo(){
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(glm::vec3), m_vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGLRenderer::setVao(){
	glGenVertexArrays(1, &m_vao);

	glBindVertexArray(m_vao);

	glEnableVertexAttribArray(VERTEX_ATTR_POSITION);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (const GLvoid*)(0));

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void OpenGLRenderer::renderMesh(){

	glBindVertexArray(m_vao);

	glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());

	glBindVertexArray(0);
}

void OpenGLRenderer::getMeshInformation(int &numVerts){
	numVerts = m_vertices.size();
}