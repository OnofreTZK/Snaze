#include "../include/snakegame.h"

int main ( int argc, char *argv[] )
{
    SNAZE::SnakeGame game;

    game.initialize( argc, argv );

    return EXIT_SUCCESS;
}
