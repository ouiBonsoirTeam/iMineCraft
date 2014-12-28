#include <glimac/Torch.hpp>
#include <glimac/Sphere.hpp>


namespace glimac 
{
	void Torch::draw(pointLightProgram & prog, glm::mat4 & matrixV)
	{
		// glm::vec3 lightPos = glm::vec3(matrixV * glm::vec4(_position, 0));
		// glUniform3f(prog.uLightPos_vs, lightPos.r, lightPos.g, lightPos.b);
		// glUniform3f(prog.uLightIntensity, _intensity.r, _intensity.g, _intensity.b);

		// Sphere sphere (_radius, 32, 16);

		// //bind de la texture
  //       glBindTexture(GL_TEXTURE_2D, _texture.getId());
  //       glUniform1i(prog.uTexture, 0);
        
  //       //dessine triangles
  //       glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());

  //       //debind de la texture
  //       glBindTexture(GL_TEXTURE_2D, 0);

	}

}