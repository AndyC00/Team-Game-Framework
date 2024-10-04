// This include:
#include "EnemyS.h"

// Local includes:
#include "entity.h"

// Library includes:
#include "renderer.h"
#include <cstdlib>
#include <cmath>
#include <iostream>


EnemyS::EnemyS() :Entity()

{

}

EnemyS::~EnemyS()
{
	
}

bool EnemyS::Initialise(Renderer& renderer)
{
	const char* pcFilename = "Sprites\\skeleton.png";

	if (!Entity::Initialise(renderer, pcFilename))
	{
		std::cout << "Enemy failed to spawn" << std::endl;
		return false;
	}


	return true;
}

