#pragma once

#include <iostream>
#include <glimac/texture.hpp>
#include <glimac/glm.hpp>

namespace glimac
{

	class Torch
	{
		private:
			glm::vec3 _position;
			glm::vec3 _intensity;
			float _radius;
			// Texture _texture;

		public:
			// Constructors
			Torch();
			Torch(glm::vec3 p, glm::vec3 i = glm::vec3(20, 20, 20), float r = 1 /*, Texture t = NULL*/);

			// Getters
			glm::vec3 getPosition(){ return _position; };
			glm::vec3 getIntensity(){ return _intensity; };
	};
	
}