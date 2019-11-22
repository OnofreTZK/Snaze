#include "../include/snakegame.h"


namespace SNAZE{

//===============================================================================================
    void printIntro()
    {
        std::stringstream ss;
        ss <<   "\033[93;1;m********************************************\n" <<
                "*                                          *\n" <<
                "*  ******  *     *  ******  ******  ****** *\n" <<
                "*  *       * *   *  *    *      *   *      *\n" <<
                "*  ******  *  *  *  * ** *    *     ****   *\n" <<
                "*       *  *   * *  *    *  *       *      *\n" <<
                "*  ******  *    **  *    *  ******  ****** *\n" <<
                "*                                          *\n" <<
                "********************************************\n\033[0m";

        std::cout << ss.str() << std::endl;
    }

//===============================================================================================
    void SnakeGame::initialize( int argc, char *argv[] )
    {
        // Checking parameter.
        if( argc < 2 )
        {
            std::cerr << ">>>\x1b[93mToo few arguments!\x1b[0m\n"
                      << ">>>The program need a config file to start the simulation!\n"
                      << ">>>Simulation \x1b[31m[FAILED]\x1b[0m\n";
            exit(1);
        }

        std::cout << ">>>Loading level(s)...\n";

        /*------- Loading config -------*/
        std::string filename = argv[1];

        configParser( _levels, filename );
        /*------------------------------*/

        // Initializing state machine.
        StateMachine = state::START;

        std::cout << ">>>Levels loaded: " << _levels.size()-1 << "\n";

        printIntro();

        std::cout << "\n\t->Press \x1b[94m<ENTER>\x1b[0m to start.\n";

        while( std::cin.get() != '\n' )
        {
            continue;
        }

        _levels[0].randPellet();

        _levels[0].printMaze();

        _levels[0].findSolution();

        _levels[0].printMaze();

}

} // END NAMESPACE.
