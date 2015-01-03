#pragma once

#include <vector>
#include "common.hpp"
#include <cmath>

namespace glimac 
{


class FreeFlyCamera 
{

private:
	glm::vec3 m_Position;		//position de la camera
	glm::vec3 m_Inertia;		//inertie de la camera
	glm::vec3 m_JumpInertia;	//inertie du saut de la camera	
	float m_fPhi; 				//angle phi de F
	float m_fTheta; 			//angle teta de F
	
	glm::vec3 m_FrontVector;	//vecteur F
	glm::vec3 m_LeftVector;		//vecteur L
	glm::vec3 m_UpVector;		//vecteur U

	void computeDirectionVectors();

public:

	FreeFlyCamera()
	{
		m_Position = glm::vec3(5,8,5);
		m_Inertia = glm::vec3(0,0,0);
		m_fPhi = 0;
		m_fTheta = 0.f;
		computeDirectionVectors();
	}

	void moveLeft(float t);

	void moveFront(float t);

	void moveUp(float t);

	void slide(glm::vec3 vec);

	void setInertia(glm::vec3 vec);

	void setJumpInertia(glm::vec3 vec);

	void updateInertia(glm::vec3 vec);

	void divideInertia(float t);

	void divideJumpInertia(float t);

	void rotateLeft(float degrees);

	void rotateUp(float degrees);

	void setPosition(glm::vec3 vec);


	glm::mat4 getViewMatrix() const;

	glm::vec3 getPosition() const;

	glm::vec3 getFrontVector() const;

	glm::vec3 getLeftVector() const;

	glm::vec3 getUpVector() const;

	glm::vec3 getInertia() const;

	glm::vec3 getJumpInertia() const;


};

}