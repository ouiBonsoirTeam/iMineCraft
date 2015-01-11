#version 330 core

in vec3 vFragViewCoord;
in vec3 vFragViewNormale;
in vec4 vFragTexture;

out vec4 fFragColor;

uniform sampler2D uTexture;

uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;

uniform vec3 uLightDir_vs; 
uniform vec3 uLightIntensity;

vec3 blinnPhong()
{
	vec3 uLightDir_vsNORME = normalize(uLightDir_vs);
	vec3 vFragViewNormaleNORME = normalize(vFragViewNormale);
	vec3 vFragViewCoordNORME = normalize(-vFragViewCoord);

	vec3 halfVector = normalize((vFragViewCoordNORME + uLightDir_vsNORME ) / 2.0);

	return uLightIntensity 
				* ( uKd 
				* max(0, dot(uLightDir_vsNORME, vFragViewNormaleNORME))
				+ uKs 
				* pow( ( max(0, dot(halfVector, vFragViewNormaleNORME))), uShininess)
				 );
}

void main() {

	vec4 textMain = texture(uTexture, vFragTexture.xy);
	vec4 textOcclu = vec4(1, 1, 1, 1) - texture(uTexture, vFragTexture.zw);
	fFragColor = vec4(vec3(textMain - textOcclu * 0.5) - (vec3(1,1,1) - blinnPhong()), 1);

};
