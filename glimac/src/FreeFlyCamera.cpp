#include <vector>
#include <cmath>
#include "glimac/common.hpp"
#include "glimac/FreeFlyCamera.hpp"

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

	void FreeFlyCamera::updateInertia(glm::vec3 vec)
	{
		m_Inertia += vec;
	}

	void FreeFlyCamera::divideInertia(float t)
	{
		m_Inertia = glm::vec3(m_Inertia.x/t,0,m_Inertia.z/t);
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
		return glm::vec3(m_FrontVector.x,0,m_FrontVector.z);
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

}