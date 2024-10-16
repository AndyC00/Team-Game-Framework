#pragma once
#include "entity.h"

class Ladder : public Entity
{
public:
    Ladder();
    virtual ~Ladder();

    bool Initialise(Renderer& renderer);

    virtual void Process(float deltaTime);
    virtual void Draw(Renderer& renderer);

    // Additional methods specific to the ladder, if needed
};
