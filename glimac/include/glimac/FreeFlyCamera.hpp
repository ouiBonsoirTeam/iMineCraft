#pragma once

#include <vector>
#include "common.hpp"
#include <cmath>

namespace glimac 
{


class FreeFlyCamera 
{

private:
	glm::vec3 m_Position;	//position de la camera
	float m_fPhi; 			//angle phi de F
	float m_fTheta; 		//angle teta de F
	
	glm::vec3 m_FrontVector;	//vecteur F
	glm::vec3 m_LeftVector;		//vecteur L
	glm::vec3 m_UpVector;		//vecteur U

	void computeDirectionVectors();

public:

	FreeFlyCamera()
	{
		m_Position = glm::vec3(0,0,0);
		m_fPhi = glm::pi<float>();
		m_fTheta = 0.f;
		computeDirectionVectors();
	}

	void moveLeft(float t);

	void moveFront(float t);

	void moveUp(float t);

	void rotateLeft(float degrees);

	void rotateUp(float degrees);

	glm::mat4 getViewMatrix() const;

};

}