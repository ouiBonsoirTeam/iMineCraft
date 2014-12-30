#include "Skybox.hpp"

using namespace glimac;

bool Skybox::load_cube_map_side (GLenum side_target, const char* file_name)
{
  glBindTexture (GL_TEXTURE_CUBE_MAP, m_texture);

  int x, y, n;
  int force_channels = 4;
  unsigned char*  image_data = stbi_load (
    file_name, &x, &y, &n, force_channels);
  if (!image_data) {
    fprintf (stderr, "ERROR: could not load %s\n", file_name);
    return false;
  }
  // non-power-of-2 dimensions check
  if ((x & (x - 1)) != 0 || (y & (y - 1)) != 0) {
    fprintf (
      stderr, "WARNING: image %s is not power-of-2 dimensions\n", file_name
    );
  }
  
  // copy image data into 'target' side of cube map
  glTexImage2D (
	side_target,
	0,
	GL_RGBA,
	x,
	y,
	0,
	GL_RGBA,
	GL_UNSIGNED_BYTE,
	image_data
  );
  free (image_data);
  return true;
}

void Skybox::create_cube_map (
	const char* front,
	const char* back,
	const char* top,
	const char* bottom,
	const char* left,
	const char* right)
{
	// generate a cube-map texture to hold all the sides
	glActiveTexture (GL_TEXTURE0);
	glGenTextures (1, &m_texture);

	// load each image and copy into a side of the cube-map texture
	assert (load_cube_map_side (GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, front));
	assert (load_cube_map_side (GL_TEXTURE_CUBE_MAP_POSITIVE_Z, back));
	assert (load_cube_map_side (GL_TEXTURE_CUBE_MAP_POSITIVE_Y, top));
	assert (load_cube_map_side (GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, bottom));
	assert (load_cube_map_side (GL_TEXTURE_CUBE_MAP_NEGATIVE_X, left));
	assert (load_cube_map_side (GL_TEXTURE_CUBE_MAP_POSITIVE_X, right));
	// format cube map texture
	glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void Skybox::init(SkyboxProgram &skyProg)
{
	create_cube_map ( "bin/assets/skybox/v2/zneg.jpg",
					  "bin/assets/skybox/v2/zpos.jpg",
					  "bin/assets/skybox/v2/ypos.jpg",
					  "bin/assets/skybox/v2/yneg.png",
					  "bin/assets/skybox/v2/xneg.jpg",
					  "bin/assets/skybox/v2/xpos.jpg"
					);

	float vertices[] = {
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	glGenBuffers (1, &m_vbo);
	glBindBuffer (GL_ARRAY_BUFFER, m_vbo);
	glBufferData (GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

	glGenVertexArrays (1, &m_vao);
	glBindVertexArray (m_vao);
	glEnableVertexAttribArray (0);
	glBindBuffer (GL_ARRAY_BUFFER, m_vbo);
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	
}

void Skybox::draw(SkyboxProgram &skyProg, const glm::mat4 &viewMatrix)
{
	//cste dans le programme, a degager de la classe
	glm::mat4 projMatrix = glm::perspective(glm::radians(70.f), 800.f/600.f, 0.1f, 1000.f);

	glm::mat4 modelMatrix =  glm::scale(glm::mat4(1.0), glm::vec3(800, 800, 800)); 
	glm::mat4 modelViewMatrix = viewMatrix * modelMatrix;
	//calcul de la matrixViewProjetée
	glm::mat4 modelViewProjMatrix = projMatrix * modelViewMatrix;

	glm::mat4 rotateCamMat = modelViewProjMatrix;
	rotateCamMat[3][0] = 0;
	rotateCamMat[3][1] = 0;
	rotateCamMat[3][2] = 0;
	rotateCamMat[3][3] = 1;

	glUniformMatrix4fv(skyProg.PVM, 1, GL_FALSE, glm::value_ptr(rotateCamMat));

	glActiveTexture (GL_TEXTURE0);
	glBindTexture (GL_TEXTURE_CUBE_MAP, m_texture);
	glBindVertexArray (m_vao);
	glDrawArrays (GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

}

void Skybox::destruct()
{
	// release vertex and index buffer object
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);

	// release cube map
	glDeleteTextures(1, &m_texture);
}

