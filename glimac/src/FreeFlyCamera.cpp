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
		m_Position += t * m_FrontVector;
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

	glm::mat4 FreeFlyCamera::getViewMatrix() const
	{
		return glm::lookAt(m_Position, m_Position + m_FrontVector, m_UpVector);
	}




}