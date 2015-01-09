#include <vector>
#include <cmath>
#include "glimac/common.hpp"
#include "glimac/FreeFlyCamera.hpp"
#include <json/json.h>
#include <fstream>
#include <iostream>

namespace glimac 
{


	void FreeFlyCamera::computeDirectionVectors()
	{
		//calcul de F
		m_FrontVector = glm::vec3( cos(m_fTheta) * sin(m_fPhi),
						  		   sin(m_fTheta),
						  		   cos(m_fTheta) * cos(m_fPhi));

		//calcul de L
		m_LeftVector = glm::vec3(sin(m_fPhi + glm::pi<float>() / 2.f),
								 0,
								 cos(m_fPhi + glm::pi<float>() / 2.f));

		m_UpVector = glm::cross(m_FrontVector, m_LeftVector);

	}

	void FreeFlyCamera::moveLeft(float t)
	{
		m_Position += t * m_LeftVector;
	}

	void FreeFlyCamera::moveFront(float t)
	{
		m_Position += t * glm::vec3(m_FrontVector.x,0,m_FrontVector.z);
	}

	void FreeFlyCamera::moveUp(float t)
	{
		m_Position += t * glm::vec3(0,1,0);
	}

	void FreeFlyCamera::slide(glm::vec3 vec)
	{
		m_Position += vec;
	}

	void FreeFlyCamera::setInertia(glm::vec3 vec)
	{
		m_Inertia = vec;
	}

	void FreeFlyCamera::setJumpInertia(glm::vec3 vec)
	{
		m_JumpInertia = vec;
	}

	void FreeFlyCamera::updateInertia(glm::vec3 vec)
	{
		m_Inertia += vec;
	}

	void FreeFlyCamera::divideInertia(float t)
	{
		m_Inertia = glm::vec3(m_Inertia.x/t,0,m_Inertia.z/t);
	}

	void FreeFlyCamera::divideJumpInertia(float t)
	{
		m_JumpInertia = glm::vec3(0,m_JumpInertia.y/t,0);
	}

	void FreeFlyCamera::rotateLeft(float degrees)
	{
		m_fPhi += glm::radians(degrees);
		computeDirectionVectors();
	}

	void FreeFlyCamera::rotateUp(float degrees)
	{
		m_fTheta += glm::radians(degrees);
		computeDirectionVectors();
	}

	void FreeFlyCamera::setTheta(float degrees)
	{
		m_fTheta = glm::radians(degrees);
		computeDirectionVectors();
	}

	void FreeFlyCamera::setPosition(glm::vec3 vec)
	{
		m_Position = vec;
	}


	glm::mat4 FreeFlyCamera::getViewMatrix() const
	{
		return glm::lookAt(m_Position, m_Position + m_FrontVector, m_UpVector);
	}

	glm::vec3 FreeFlyCamera::getPosition() const
	{
		return m_Position;
	}

	glm::vec3 FreeFlyCamera::getFrontVector() const
	{
		return m_FrontVector;
	}

	glm::vec3 FreeFlyCamera::getLeftVector() const
	{
		return m_LeftVector;
	}

	glm::vec3 FreeFlyCamera::getUpVector() const
	{
		return m_UpVector;
	}

	glm::vec3 FreeFlyCamera::getInertia() const
	{
		return m_Inertia;
	}

	glm::vec3 FreeFlyCamera::getJumpInertia() const
	{
		return m_JumpInertia;
	}

	void FreeFlyCamera::save(const std::string &jsonFolderPath)
	{
		std::ofstream file;
		file.open(jsonFolderPath + "A_camera.json");

		if (file.is_open())
		{
			Json::Value jsonValue;

			jsonValue["pos_x"] = Json::Value(m_Position[0]);
		    jsonValue["pos_y"] = Json::Value(m_Position[1]);
		    jsonValue["pos_z"] = Json::Value(m_Position[2]);
		    jsonValue["m_fTheta"] = Json::Value(m_fTheta);
		    jsonValue["m_fPhi"] = Json::Value(m_fPhi);

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

	bool FreeFlyCamera::load(const std::string & saveFolder)
	{
	    std::ifstream file;
	    std::string filePath = saveFolder + "A_camera.json";
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
	        	m_fTheta = root["m_fTheta"].asFloat();
	        	m_fPhi = root["m_fPhi"].asFloat();
	            m_Position = glm::vec3(root["pos_x"].asFloat(), root["pos_y"].asFloat(), root["pos_z"].asFloat());

	            computeDirectionVectors();

	            return true;
	        }
	    }

	    return false;
	}

}