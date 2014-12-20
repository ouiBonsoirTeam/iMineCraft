#version 330 core

in vec3 vFragViewCoord;
in vec3 vFragViewNormale;
in vec2 vFragTexture;

out vec3 fFragColor;

// uniform vec3 uColor;
uniform sampler2D uTexture;

void main() {

	// fFragColor = vec3(1, 1, 1);
	// fFragColor = vec3(texture(uTexture, vFragTexture));
	
	fFragColor = vFragViewNormale;
}