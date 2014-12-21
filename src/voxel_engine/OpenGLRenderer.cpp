#include "OpenGLRenderer.hpp"

// Constructor
OpenGLRenderer::OpenGLRenderer(){
	glGenBuffers(1, &m_buffer[POSITION]);
	glGenBuffers(1, &m_buffer[NORMAL]);
	glGenBuffers(1, &m_buffer[TEXTURE]);
}

// Destructor
OpenGLRenderer::~OpenGLRenderer(){
	glDeleteBuffers(1, &m_buffer[POSITION]);
	glDeleteBuffers(1, &m_buffer[NORMAL]);
	glDeleteBuffers(1, &m_buffer[TEXTURE]);
	glDeleteVertexArrays(1, &m_vao);
}

void OpenGLRenderer::addTriangle(glm::vec3 position_1, glm::vec3 position_2, glm::vec3 position_3){
	m_vertices.push_back(position_1);
	m_vertices.push_back(position_2);
	m_vertices.push_back(position_3);
}

void OpenGLRenderer::addTexture(glm::vec2 texPos_1, glm::vec2 texPos_2, glm::vec2 texPos_3){
	m_textures.push_back(texPos_1);
	m_textures.push_back(texPos_2);
	m_textures.push_back(texPos_3);
}

void OpenGLRenderer::addNormal(glm::vec3 n){
	m_normals.push_back(n);
	m_normals.push_back(n);
	m_normals.push_back(n);
	m_normals.push_back(n);
	m_normals.push_back(n);
	m_normals.push_back(n);

}

void OpenGLRenderer::finishVboPosition(){
	glBindBuffer(GL_ARRAY_BUFFER, m_buffer[POSITION]);

	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(glm::vec3), m_vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGLRenderer::finishVboTexture(){
	glBindBuffer(GL_ARRAY_BUFFER, m_buffer[TEXTURE]);

	glBufferData(GL_ARRAY_BUFFER, m_textures.size() * sizeof(glm::vec2), m_textures.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGLRenderer::finishVboNormal(){
	glBindBuffer(GL_ARRAY_BUFFER, m_buffer[NORMAL]);

	glBufferData(GL_ARRAY_BUFFER, m_normals.size() * sizeof(glm::vec2), m_normals.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGLRenderer::finishVbo()
{
	finishVboPosition();
	finishVboTexture();
	finishVboNormal();
}

void OpenGLRenderer::setVao(){
	glGenVertexArrays(1, &m_vao);

	glBindVertexArray(m_vao);

	glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, m_buffer[POSITION]);
	glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (const GLvoid*)(0));

	if(m_textures.size() != 0)
	{
		glEnableVertexAttribArray(VERTEX_ATTR_TEXTCOORD);
		glBindBuffer(GL_ARRAY_BUFFER, m_buffer[TEXTURE]);
		glVertexAttribPointer(VERTEX_ATTR_TEXTCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (const GLvoid*)(0));
	}
	
	glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
	glBindBuffer(GL_ARRAY_BUFFER, m_buffer[NORMAL]);
	glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (const GLvoid*)(0));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void OpenGLRenderer::draw(GeneralProgram &program, const glm::mat4 &viewMatrix, GLuint idTexture){
	glm::mat4 modelMatrix = glm::mat4(1.f);
	glm::mat4 modelViewMatrix = viewMatrix * modelMatrix;

	// A sortir de la classe : Identique dans tout le programme
	glm::mat4 projMatrix = glm::perspective(glm::radians(70.f), 800.f/600.f, 0.1f, 100.f);

	glm::mat4 modelViewProjMatrix = projMatrix * modelViewMatrix;

	// Normale
	glm::mat4 normalMatrix = glm::transpose(glm::inverse(modelViewMatrix));

	glUniformMatrix4fv(program.uMVMatrix, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
	glUniformMatrix4fv(program.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(modelViewProjMatrix));
	glUniformMatrix4fv(program.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(normalMatrix));

	glBindVertexArray(m_vao);

	glBindTexture(GL_TEXTURE_2D, idTexture);
	glUniform1i(program.uTexture, 0);

	glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());

	glBindTexture(GL_TEXTURE_2D, 0);

	glBindVertexArray(0);
}