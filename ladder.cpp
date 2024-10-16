#include "ladder.h"
#include "renderer.h"
#include <iostream> // For error output

Ladder::Ladder()
{
}

Ladder::~Ladder()
{
}

bool Ladder::Initialise(Renderer& renderer)
{
    if (!Entity::Initialise(renderer, "Sprites\\ladder.png"))
    {
        std::cerr << "Failed to load ladder sprite" << std::endl;
        return false;
    }
    return true;
}

void Ladder::Process(float deltaTime)
{
    Entity::Process(deltaTime);
}

void Ladder::Draw(Renderer& renderer)
{

    Entity::Draw(renderer);
}
