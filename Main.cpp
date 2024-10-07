// Library includes:
#include <SDL.h>
#include <ctime> 

// Local includes: 
#include "game.h" 
#include "logmanager.h"

int main(int argc, char* argv[]) 
{
    srand(static_cast<unsigned>(time(0)));

    Game& gameInstance = Game::GetInstance(); 

    if (!gameInstance.Initialise())
    {
        LogManager::GetInstance().Log("Game initialise failed!");
        return 1;
    }

    while (gameInstance.DoGameLoop())
    {
        // No body.
    }

    Game::DestroyInstance(); 
    LogManager::DestroyInstance();

    return 0;

}
