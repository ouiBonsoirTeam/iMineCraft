#include <iostream>
#include <glimac/glm.hpp>
#include <glimac/CustomProgram.hpp>
#include <glimac/Image.hpp>
#include <glimac/FreeFlyCamera.hpp>

using namespace glimac;

class Helmet
{
private:
	glm::vec3 _position;
	GLuint _vbo_pos;
	GLuint _vbo_tex;
	GLuint _vao;
	GLuint _texture;


public:
	Helmet();
	~Helmet(){}

	void drawBillboard(HelmetProgram & prog, FreeFlyCamera &ffCam);

	void setPosition(glm::vec3 pos);



};