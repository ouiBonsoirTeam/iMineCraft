#pragma once

#include <iostream>
#include <glimac/Texture.hpp>
#include <glimac/glm.hpp>
#include <glimac/CustomProgram.hpp>
#include <glimac/FreeFlyCamera.hpp>

namespace glimac
{

	class Torch
	{
		private:
			glm::vec3 _position;
			glm::vec3 _intensity;
			GLuint _vbo_pos;
			GLuint _vbo_tex;
			GLuint _vao;
			GLuint _texture;

		public:
			// Constructors
			Torch();
			Torch(glm::vec3 position);

			// Getters
			glm::vec3 getPosition(){ return _position; };
			glm::vec3 getIntensity(){ return _intensity; };

			glm::vec3 translatePos(glm::vec3 position);

			// Methods
			void computeLight(LightsProgram & prog, FreeFlyCamera &ffCam);

			void drawBillboard(GeneralProgram & prog, FreeFlyCamera &ffCam);
	};
	
}