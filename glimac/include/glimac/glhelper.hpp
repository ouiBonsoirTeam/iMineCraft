#ifndef GLHELPER_H
#define GLHELPER_H

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

void setupTexture(GLuint& texture);
void setupTexture(GLuint& texture, SDL_Surface *s);
void deleteTexture(GLuint& texture);

void setupCubeMap(GLuint& texture);
void setupCubeMap(GLuint& texture, SDL_Surface *xpos, SDL_Surface *xneg, SDL_Surface *ypos, SDL_Surface *yneg, SDL_Surface *zpos, SDL_Surface *zneg);
void deleteCubeMap(GLuint& texture);

void saveTGA(unsigned char* buffer, int width, int height, bool video = false);

#endif
