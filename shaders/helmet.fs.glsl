#version 330 core

in vec3 vFragViewCoord;
in vec3 vFragViewNormale;
in vec4 vFragTexture;

out vec4 fFragColor;

// uniform vec3 uColor;
uniform sampler2D uTexture;

void main() {

	vec4 text = texture(uTexture, vFragTexture.xy);

	fFragColor = text;
}