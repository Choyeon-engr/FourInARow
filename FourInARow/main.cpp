#include "Game.hpp"

int main()
{
    Game game;
    
    if (game.Init())
        game.Loop();
    
    game.Quit();
    
    return 0;
}
