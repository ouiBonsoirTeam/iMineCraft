#version 330 core

in vec3 vFragViewCoord;
in vec3 vFragViewNormale;
in vec4 vFragTexture;

out vec4 fFragColor;

uniform sampler2D uTexture;

void main() {

	vec4 textMain = texture(uTexture, vFragTexture.xy);
	//vec4 textOcclu = vec4(1, 1, 1, 1) - texture(uTexture, vFragTexture.zw);
	//fFragColor = vec3(textMain - textOcclu * 0.5);
	fFragColor = textMain;

}