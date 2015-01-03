#include <glimac/CustomProgram.hpp>


class Light
{
private:
	glm::vec3 _Kd;
    glm::vec3 _Ks;
    float _shininess;
    glm::vec3 _lightIntensity;
    glm::vec3 _lightDir;

public:
	Light(){}
	Light(glm::vec3 lightIntensity, glm::vec3 lightDir)
	{
		_lightIntensity = lightIntensity;
		_lightDir = lightDir;
	}

	void initMaterial(glm::vec3 Kd, glm::vec3 Ks, float shininess)
	{
		_Kd = Kd;
		_Ks = Ks;
		_shininess = shininess;
	}

	void computeLight(DirectionalLightProgram &prog, glm::mat4 viewMatrix)
	{
        glm::vec3 lightDirTmp = glm::vec3(viewMatrix * glm::vec4(_lightDir, 0));

		glUniform3f(prog.uKd, _Kd.r, _Kd.g, _Kd.b);
        glUniform3f(prog.uKs, _Ks.r, _Ks.g, _Ks.b);
        glUniform1f(prog.uShininess, _shininess);
        glUniform3f(prog.uLightIntensity, _lightIntensity.r, _lightIntensity.g, _lightIntensity.b);
        glUniform3f(prog.uLightDir_vs, lightDirTmp.r, lightDirTmp.g, lightDirTmp.b);
	}

	~Light(){}
};