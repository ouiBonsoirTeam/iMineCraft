#version 330 core

in vec3 vFragViewCoord;
in vec3 vFragViewNormale;
in vec4 vFragTexture;

out vec4 fFragColor;

uniform sampler2D uTexture;

void main() 
{
	vec4 textMain = texture(uTexture, vFragTexture.xy);
	fFragColor = textMain;
}