// This include:
#include "EnemyS.h"

// Local includes:
#include "entity.h"

// Library includes:
#include "renderer.h"
#include <cstdlib>
#include <cmath>

EnemyS::EnemyS() :Entity()

{

}

EnemyS::~EnemyS()
{
	
}

bool EnemyS::Initialise(Renderer& renderer)
{
	const char* pcFilename = "Sprites\\";

	if (!Entity::Initialise(renderer, pcFilename))
	{
		printf("Enemy failed to spawn\n");
		return false;
	}


	return true;
}

