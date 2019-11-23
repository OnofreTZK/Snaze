#include "../include/snakegame.h"

int main ( int argc, char *argv[] )
{
    SNAZE::SnakeGame game;

    game.initialize( argc, argv );

    game.render();

    while( not game.game_over() )
    {
        game.process_events();
        game.update();
        game.render();
    }

    return EXIT_SUCCESS;
}
