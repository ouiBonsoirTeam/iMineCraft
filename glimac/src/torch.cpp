#include <glimac/torch.hpp>
#include <glimac/texture.hpp>

namespace glimac 
{
	Torch::Torch()
	{
		_position = glm::vec3(0, 0, 0);
		_intensity = glm::vec3(1, 1, 1);
		_radius = 1;	
	}

	Torch::Torch(glm::vec3 p, glm::vec3 i, float r, Texture t)
	{
		_position = p;
		_intensity = i;
		_radius = r;
		_texture = t;	
	}
}