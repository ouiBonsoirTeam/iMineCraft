#include <glimac/Torch.hpp>
#include <glimac/Texture.hpp>

namespace glimac 
{
	Torch::Torch()
	{
		_position = glm::vec3(0, 0, 0);
		_intensity = glm::vec3(20, 20, 20);
		_radius = 1;	
	}

	Torch::Torch(glm::vec3 p, glm::vec3 i, float r/*, Texture t*/)
	{
		_position = p;
		_intensity = i;
		_radius = r;
		// _texture = t;	
	}

	void Torch::draw(pointLightProgram & prog, glm::mat4 & matrixV)
	{
		glm::vec3 lightPos = glm::vec3(matrixV * glm::vec4(_position, 0));
		glUniform3f(prog.uLightPos_vs, lightPos.r, lightPos.g, lightPos.b);
		glUniform3f(prog.uLightIntensity, _intensity.r, _intensity.g, _intensity.b);
	}

}