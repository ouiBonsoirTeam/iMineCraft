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
uniform vec3 uLightPos_vs; 
uniform vec3 uLightIntensityPoint;
uniform vec3 uLightIntensityDir;

vec3 blinnPhongDirec()
{
	vec3 uLightDir_vsNORME = normalize(uLightDir_vs);
	vec3 vFragViewNormaleNORME = normalize(vFragViewNormale);
	vec3 vFragViewCoordNORME = normalize(-vFragViewCoord);

	vec3 halfVector = normalize((vFragViewCoordNORME + uLightDir_vsNORME ) / 2.0);

	return uLightIntensityDir 
				* ( uKd 
				* max(0, dot(uLightDir_vsNORME, vFragViewNormaleNORME))
				+ uKs 
				* pow( ( max(0, dot(halfVector, vFragViewNormaleNORME))), uShininess)
				 );
}

vec3 blinnPhongPoint()
{
	vec3 uLightPos_vsNORME = normalize(uLightPos_vs);
	vec3 vFragViewNormaleNORME = normalize(vFragViewNormale);
	vec3 vFragViewCoordNORME = normalize(-vFragViewCoord);

	vec3 wi = normalize(uLightPos_vs - vFragViewCoord);

	vec3 halfVector = normalize((vFragViewCoordNORME + uLightPos_vsNORME ) / 2.0);

	float d = distance(uLightPos_vs, vFragViewCoord);

	return (uLightIntensityPoint 
				* ( uKd 
				* max(0, dot(uLightPos_vsNORME, vFragViewNormaleNORME))
				+ uKs 
				* pow( ( max(0, dot(halfVector, vFragViewNormaleNORME))), uShininess)
				 ) ) / (d*d) ;
}

void main() 
{
	vec4 textMain = texture(uTexture, vFragTexture.xy);
	vec4 textOcclu = vec4(1, 1, 1, 1) - texture(uTexture, vFragTexture.zw);
	vec4 tex = textMain - textOcclu * 0.5;

	vec3 light = blinnPhongPoint() + 0.6 * blinnPhongDirec();
	vec3 invLight = vec3(1,1,1) - light;

	fFragColor = vec4(vec3(tex) - 0.4 * invLight, 1); 
}