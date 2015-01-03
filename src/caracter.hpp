#pragma once

#include "../inventory.hpp"
#include <glimac/FreeFlyCamera.hpp>

class Caracter
{
public:
	Caracter()
	{
		FreeFlyCamera * m_ffCam;
		Inventory * m_inventory;
	}

	*FreeFlyCamera getCam()
	{
		return &m_ffCam;
	}
	*Inventory getInventory()
	{
		return &m_inventory;
	}

	~Caracter();


};