#version 330 core

in vec3 vFragViewCoord;
in vec3 vFragViewNormale;
in vec2 vFragTexture;

out vec3 fFragColor;

// uniform vec3 uColor;
// uniform sampler2D uTexture;

void main() {

	fFragColor = glm::vec3(1, 1, 1);

}