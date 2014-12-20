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

void OpenGLRenderer::renderMesh(GeneralProgram &program, const glm::mat4 &viewMatrix){
	glm::mat4 modelMatrix = glm::mat4(1.f);
	glm::mat4 modelViewMatrix = viewMatrix * modelMatrix;

	// A sortir de la classe : Identique dans tout le programme
	glm::mat4 projMatrix = glm::perspective(glm::radians(70.f), 800.f/600.f, 0.1f, 100.f);

	glm::mat4 modelViewProjMatrix = projMatrix * modelViewMatrix;

	// Normale à faire

	glUniformMatrix4fv(program.uMVMatrix, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
	glUniformMatrix4fv(program.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(modelViewProjMatrix));

	glBindVertexArray(m_vao);

	glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());

	glBindVertexArray(0);
}