#version 330 core

in vec3 vFragViewCoord;
in vec3 vFragViewNormale;

out vec3 fFragColor;

// uniform vec3 uColor;
//uniform sampler2D uTexture;
//uniform sampler2D uTextureNuage;

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

	//vec4 texAvery = texture(uTexture, vFragTexture) + texture(uTextureNuage, vFragTexture);

	//fFragColor.rgb = texAvery.rgb;

	fFragColor.rgb = blinnPhong();

	//fFragColor.rgb = uLightDir_vs.rgb;



}
