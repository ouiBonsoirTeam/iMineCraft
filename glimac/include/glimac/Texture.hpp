#pragma once

#include <iostream>
#include <string>
#include <glimac/Image.hpp>
#include <GL/glew.h>

namespace glimac
{

	class Texture
	{
		private:
			std::string _name;
			GLuint _idTexture;
			std::unique_ptr<Image> _texturePointer;

		public:
			// Constructors
			Texture(){};
			Texture(std::string n)
			{
				_name = n;

				_texturePointer = loadImage("../iMineCraft/assets/textures/" + _name);
				if(_texturePointer == NULL)
				{
					std::cerr << "Error while charging texture." << std::endl;
				}

				glGenTextures(1, &_idTexture);
				glBindTexture(GL_TEXTURE_2D,  _idTexture);
				glTexImage2D(GL_TEXTURE_2D,  0,  GL_RGBA,  _texturePointer->getWidth(),  _texturePointer->getHeight(),  0,  GL_RGBA,  GL_FLOAT,  _texturePointer->getPixels());
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glBindTexture(GL_TEXTURE_2D,  0);
			}

			GLuint getId(){return _idTexture;}

	};
}