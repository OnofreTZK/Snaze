#include "../include/jiripoca.h"

int main ( int agrc, char *argv[] )
{
    SNAZE::snake cobra;

    std::cout << cobra.getDirection() << "\n";
    cobra.setDirection( EAST );
    std::cout << cobra.getDirection() << "\n";
    return EXIT_SUCCESS;
}
