#pragma once

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
			Texture(std::string n);

	};
}