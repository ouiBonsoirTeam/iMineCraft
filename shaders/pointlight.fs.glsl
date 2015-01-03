#version 330 core

in vec3 vFragViewCoord;
in vec3 vFragViewNormale;
in vec4 vFragTexture;

out vec3 fFragColor;

uniform sampler2D uTexture;

//parametre du materiau
uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;

//info sur la lumiere
uniform vec3 uLightPos_vs;
uniform vec3 uLightIntensity;	//Li

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

	vec4 textCaisse = texture(uTexture, vFragTexture.xy);
	vec4 textOcclu = vec4(1, 1, 1, 1) - texture(uTexture, vFragTexture.zw);
	fFragColor = vec3(textCaisse - textOcclu * 0.5) + blinnPhong();

	//fFragColor.rgb = blinnPhong();



}
