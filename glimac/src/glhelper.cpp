#include "glimac/glhelper.hpp"

using namespace glimac;

void setupTexture(GLuint& texture) {
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

//void setupTexture(GLuint& texture, const std::unique_ptr<Image> &s) {
void setupTexture(GLuint& texture, SDL_Surface *s) {
	setupTexture(texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, s->w, s->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, s->pixels);
}

void deleteTexture(GLuint& texture) {
	glDeleteTextures(1, &texture);
}

void setupCubeMap(GLuint& texture) {
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_CUBE_MAP);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

//void setupCubeMap(GLuint& texture, const std::unique_ptr<Image> &xpos, const std::unique_ptr<Image> &xneg, const std::unique_ptr<Image> &ypos, const std::unique_ptr<Image> &yneg, const std::unique_ptr<Image> &zpos, const std::unique_ptr<Image> &zneg)
void setupCubeMap(GLuint& texture, SDL_Surface *xpos, SDL_Surface *xneg, SDL_Surface *ypos, SDL_Surface *yneg, SDL_Surface *zpos, SDL_Surface *zneg)
{
	setupCubeMap(texture);
	// glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, xpos->getWidth(), xpos->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, xpos->getPixels());
	// glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, xneg->getWidth(), xneg->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, xneg->getPixels());
	// glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, ypos->getWidth(), ypos->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, ypos->getPixels());
	// glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, yneg->getWidth(), yneg->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, yneg->getPixels());
	// glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, zpos->getWidth(), zpos->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, zpos->getPixels());
	// glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, zneg->getWidth(), zneg->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, zneg->getPixels());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, xpos->w, xpos->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, xpos->pixels);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, xneg->w, xneg->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, xneg->pixels);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, ypos->w, ypos->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, ypos->pixels);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, yneg->w, yneg->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, yneg->pixels);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, zpos->w, zpos->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, zpos->pixels);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, zneg->w, zneg->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, zneg->pixels);

}

void deleteCubeMap(GLuint& texture) {
	glDeleteTextures(1, &texture);
}

void saveTGA(unsigned char* buffer, int width, int height, bool video) {
	static int i = 0;
	std::stringstream out;
	if (video) {
		if      (i < 10)
			out << "video000" << (i++) << ".tga";
		else if (i < 100)
			out << "video00" << (i++) << ".tga";
		else if (i < 1000)
			out << "video0" << (i++) << ".tga";
		else if (i < 10000)
			out << "video" << (i++) << ".tga";
	} else {
		out << "capture" << (i++) << ".tga";
	}
	std::string s = out.str();
	
	glReadPixels(0, 0, width, height, GL_BGRA, GL_UNSIGNED_BYTE, buffer);
	std::fstream of(s.c_str(), std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
	char header[18] = { 0 };
	header[2] = 2;
	header[12] = width & 0xff;
	header[13] = width >> 8;
	header[14] = height & 0xff;
	header[15] = height >> 8;
	header[16] = 32;
	of.write(header, 18);
	of.write((char *)buffer, width * height * 4);
}