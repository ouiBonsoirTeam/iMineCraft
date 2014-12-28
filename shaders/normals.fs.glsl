#version 330 core

in vec3 vFragViewCoord;
in vec3 vFragViewNormale;
in vec4 vFragTexture;

out vec3 fFragColor;

// uniform vec3 uColor;
uniform sampler2D uTexture;

void main() {

	// fFragColor = vec3(1, 1, 1);
	vec4 textCaisse = texture(uTexture, vFragTexture.xy);
	vec4 textOcclu = vec4(1, 1, 1, 1) - texture(uTexture, vFragTexture.zw);
	fFragColor = vec3(textCaisse - textOcclu * 0.5);

	// vec4 colKitten = texture(texKitten, Texcoord);
	// vec4 colPuppy = texture(texPuppy, Texcoord);
	// outColor = mix(colKitten, colPuppy, 0.5);

	//fFragColor = vec3(texture(uTexture, vFragTexture.xy));//+ (texture(uTexture, vFragTexture.zw)));
	
	// fFragColor = normalize(vFragViewNormale + vec3(0.2, 0.2, 0.2));
}