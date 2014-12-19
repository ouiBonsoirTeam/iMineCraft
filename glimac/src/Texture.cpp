#include <glimac/Texture.hpp>

namespace glimac 
{

	Texture::Texture(std::string n)
	{
		_name = n;

		_texturePointer = loadImage("../iMineCraft/assets/textures/" + _name);
		if(_texturePointer == NULL)
		{
			std::cerr << "Probleme de chargement de texture." << std::endl;
		}

		glGenTextures(1, &_idTexture);
		glBindTexture(GL_TEXTURE_2D,  _idTexture);
		glTexImage2D(GL_TEXTURE_2D,  0,  GL_RGBA,  _texturePointer->getWidth(),  _texturePointer->getHeight(),  0,  GL_RGBA,  GL_FLOAT,  _texturePointer->getPixels());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D,  0);
	}
}