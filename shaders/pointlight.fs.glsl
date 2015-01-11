#version 330 core

in vec3 vFragViewCoord;
in vec3 vFragViewNormale;
in vec4 vFragTexture;

out vec4 fFragColor;

uniform sampler2D uTexture;

uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;

uniform vec3 uLightPos_vs;
uniform vec3 uLightIntensity;

vec3 blinnPhong()
{
	vec3 uLightPos_vsNORME = normalize(uLightPos_vs);
	vec3 vFragViewNormaleNORME = normalize(vFragViewNormale);
	vec3 vFragViewCoordNORME = normalize(-vFragViewCoord);

	vec3 wi = normalize(uLightPos_vs - vFragViewCoord);

	vec3 halfVector = normalize((vFragViewCoordNORME + uLightPos_vsNORME ) / 2.0);

	float d = distance(uLightPos_vs, vFragViewCoord);

	return (uLightIntensity 
				* ( uKd 
				* max(0, dot(uLightPos_vsNORME, vFragViewNormaleNORME))
				+ uKs 
				* pow( ( max(0, dot(halfVector, vFragViewNormaleNORME))), uShininess)
				 ) ) / (d*d) ;
}

void main() {

	vec4 textMain = texture(uTexture, vFragTexture.xy);
	vec4 textOcclu = vec4(1, 1, 1, 1) - texture(uTexture, vFragTexture.zw);
	vec4 tex = textMain - textOcclu * 0.5;

	vec3 invLight = vec3(1,1,1) - blinnPhong();

	fFragColor = vec4(vec3(tex) - invLight , 1);
}
