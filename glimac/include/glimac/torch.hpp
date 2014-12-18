#pragma once

#include <glimac/texture.hpp>

namespace glimac
{

	class Torch
	{
		private:
			glm::vec3 _position;
			glm::vec3 _intensity;
			float _radius;
			Texture _texture;

		public:
			// Constructors
			Torch();
			Torch(glm::vec3 p, glm::vec3 i, float r, Texture t);
	};
	
}