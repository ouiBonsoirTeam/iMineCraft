#include <glimac/Torch.hpp>
#include <glimac/Sphere.hpp>
#include <json/json.h>
#include <fstream>
#include <iostream>


namespace glimac 
{
	Torch::Torch()
	{}

	Torch::Torch(glm::vec3 position)
	{
		_position = position;
		_intensity = glm::vec3(25.5, 15.3, 5.1);

		//chargement de la texture
	    std::unique_ptr<Image> ptrTexture = loadImage("bin/assets/textures/light_test2.png");
	    if (ptrTexture == NULL)
	        std::cout << "Error while charging texture!" << std::endl;


	    glGenBuffers(1, &_vbo_pos);
	    
	    //bind le vbo
	    glBindBuffer (GL_ARRAY_BUFFER, _vbo_pos);

	    glm::vec3 vertices1[] = { 

			glm::vec3(-0.5, -0.5, 0),// premier sommet
			glm::vec3(0.5, -0.5, 0), // deuxième sommet
			glm::vec3(0.5, 0.5, 0), // troisième sommet
			//deuxieme triangle
			glm::vec3(-0.5, -0.5, 0),// premier sommet
			glm::vec3(-0.5, 0.5, 0),// deuxième sommet
			glm::vec3(0.5, 0.5, 0)// troisième sommet
		};

		glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(glm::vec3), vertices1, GL_STATIC_DRAW);

		//debind le vbo
		glBindBuffer (GL_ARRAY_BUFFER, 0);

		glGenBuffers(1, &_vbo_tex);

		glBindBuffer (GL_ARRAY_BUFFER, _vbo_tex);

		glm::vec4 vertices2[] = { 

			glm::vec4(0, 0, 0, 0),// premier sommet
			glm::vec4(1, 0, 1/24, 0), // deuxième sommet
			glm::vec4(1, 1, 1/24, 1/16), // troisième sommet
			//deuxieme triangle
			glm::vec4(0, 0, 0, 0), // premier sommet
			glm::vec4(0, 1, 0, 1/16),// deuxième sommet
			glm::vec4(1, 1, 1/24, 1/16)// troisième sommet
		};

		glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(glm::vec4), vertices2, GL_STATIC_DRAW);


		glBindBuffer (GL_ARRAY_BUFFER, 0);
	    
		
		
		
		glGenVertexArrays(1, &_vao);
		
		//bind le vao
		glBindVertexArray(_vao);
		
		const GLuint VERTEX_ATTR_POSITION = 0;
		const GLuint VERTEX_ATTR_TEXTURE = 2;
		
		//on active l'attribut position et l'attribut couleur
		glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
		glEnableVertexAttribArray(VERTEX_ATTR_TEXTURE);
		
		//bind le vbo
		glBindBuffer (GL_ARRAY_BUFFER, _vbo_pos);
		
		//rempli le vao avec les spécifications de comment on a rangé les données dans vbo
		glVertexAttribPointer( VERTEX_ATTR_POSITION, 3, GL_FLOAT,
					GL_FALSE, sizeof(glm::vec3), (const GLvoid*) (0*sizeof(GLfloat)));

		glBindBuffer (GL_ARRAY_BUFFER, 0);
		glBindBuffer (GL_ARRAY_BUFFER, _vbo_tex);

					
		glVertexAttribPointer( VERTEX_ATTR_TEXTURE, 4, GL_FLOAT,
					GL_FALSE, sizeof(glm::vec4), (const GLvoid*) (0*sizeof(GLfloat)));
		
		//debind le vbo
		glBindBuffer (GL_ARRAY_BUFFER, 0);

		//debind le vao
		glBindVertexArray(0);

	    glGenTextures(1, &_texture);

	    //bind la texture 
	    glBindTexture(GL_TEXTURE_2D, _texture);


	    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,  ptrTexture->getWidth(),
	                    ptrTexture->getHeight(), 0, GL_RGBA, GL_FLOAT, 
	                    ptrTexture->getPixels()
	                );

	    //minification et manification des textures (voir notes de cours)
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	    //debind de la texture
	    glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Torch::translatePos(glm::vec3 position)
	{
		_position += position;
	}

	void Torch::computeLight(LightsProgram & prog, FreeFlyCamera &ffCam)
	{
		glm::vec4 lightPosTmp = glm::mat4(1.f) * glm::vec4(_position - glm::vec3(0,0.1,0), 1);
		lightPosTmp = ffCam.getViewMatrix() * lightPosTmp;

		glUniform3f(prog.uLightPos_vs, lightPosTmp.r, lightPosTmp.g, lightPosTmp.b);
		glUniform3f(prog.uLightIntensityPoint, _intensity.r, _intensity.g, _intensity.b);

		glUniform3f(prog.uKd, 1, 1, 1);
		glUniform3f(prog.uKs, 1, 1, 1);

	}


	void Torch::drawBillboard(GeneralProgram & prog, FreeFlyCamera &ffCam)
	{
		glm::vec3 rightVector = glm::vec3(1,0,0);
		// glm::vec3 upVector = glm::vec3(0,1,0);
		glm::vec3 lookAtVector = glm::vec3(0,0,1);

		glm::vec3 posCam = ffCam.getPosition();

		glm::vec3 objToCamProj = posCam - _position;
		objToCamProj[1] = 0;

		objToCamProj = glm::normalize(objToCamProj);

		float angleCosine = glm::dot(objToCamProj, lookAtVector);

		float angle = glm::acos(angleCosine);

		glm::vec3 upAux = glm::cross(objToCamProj, lookAtVector);

		glm::mat4 matrixM = glm::mat4(1.0);

		matrixM = glm::translate(glm::mat4(1.0), _position);

		if ((angleCosine < 0.99990) && (angleCosine > -0.9999))
			matrixM = glm::rotate(matrixM, angle, -upAux);


		glm::vec3 objToCam = posCam - _position;

		objToCam = glm::normalize(objToCam);

		angleCosine = glm::dot(objToCamProj,objToCam);

		angle = glm::acos(angleCosine);

		if ((angleCosine < 0.99990) && (angleCosine > -0.9999))
		{
			if (objToCam[1] < 0)
				matrixM = glm::rotate(matrixM, angle, rightVector);
			else
				matrixM = glm::rotate(matrixM, angle, -rightVector);
		}

		glm::mat4 matrixMV = ffCam.getViewMatrix() * matrixM;

		glm::mat4 matrixP = glm::perspective(glm::radians(70.f), 800.f/600.f, 0.1f, 3000.f);

		//calcul de la matrixViewProjetée
		glm::mat4 matrixMVP = matrixP * matrixMV;

		//calcul de la normal matrix = (MVinverse)Transposée
		glm::mat4 normalMatrix = glm::transpose(glm::inverse(matrixMV));

		glUniformMatrix4fv(prog.uMVMatrix, 1, GL_FALSE,  glm::value_ptr(matrixMV));
		glUniformMatrix4fv(prog.uMVPMatrix, 1, GL_FALSE,  glm::value_ptr(matrixMVP));
		glUniformMatrix4fv(prog.uNormalMatrix, 1, GL_FALSE,  glm::value_ptr(normalMatrix));

		//bind de la texture
	    glBindTexture(GL_TEXTURE_2D, _texture);
	    glUniform1i(prog.uTexture, 0);

	    //bind du vao
		glBindVertexArray(_vao);
		
		//dessine
		glDrawArrays(GL_TRIANGLES, 0, 6);
		
		//debind du vao
		glBindVertexArray(0);

	    //debind de la texture
	    glBindTexture(GL_TEXTURE_2D, 0);
	}



	void Torch::save(const std::string &jsonFolderPath)
	{
		std::ofstream file;
		file.open(jsonFolderPath + "A_light.json");

		if (file.is_open())
		{
			Json::Value jsonValue;

			jsonValue["pos_x"] = Json::Value(_position[0]);
		    jsonValue["pos_y"] = Json::Value(_position[1]);
		    jsonValue["pos_z"] = Json::Value(_position[2]);

			Json::FastWriter l_writer;

	        file << l_writer.write(jsonValue);

	        file.close();

		}
		else
		{
			std::cerr << "Unable to open file" << std::endl;
			exit(1);
		}
	}

	bool Torch::load(const std::string & saveFolder)
	{
	    std::ifstream file;
	    std::string filePath = saveFolder + "A_light.json";
	    file.open(filePath);
	    std::string str, contents;

	    if (file.is_open())
	    {
	        while (std::getline(file, str))
	        {
	            contents += str;
	        }  
	        file.close();

	        Json::Value root;
	        Json::Reader reader;

	        bool parsingSuccessful = reader.parse(contents, root);
	        if ( !parsingSuccessful )
	        {
	            // report to the user the failure and their locations in the document.
	            std::cerr  << "Failed to parse configuration\n"
	                       << reader.getFormattedErrorMessages();
	            return false;
	        }
	        else
	        {
	            _position = glm::vec3(root["pos_x"].asFloat(), root["pos_y"].asFloat(), root["pos_z"].asFloat());

	            return true;
	        }
	    }

	    return false;
	}

}