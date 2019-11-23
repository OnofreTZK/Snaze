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


    }

//===============================================================================================

    void SnakeGame::render()
    {
        //print stats.
        _levels[num_levels].printMaze();
    }

//===============================================================================================

    void SnakeGame::process_events()
    {
        //Thread control (fps).
        std::this_thread::sleep_for( std::chrono::milliseconds( 1000/15 ) );
    }
//===============================================================================================

    void SnakeGame::update()
    {
        if( StateMachine == state::START )
        {
            _levels[num_levels].randPellet();

            _levels[num_levels].findSolution( DEAD );

            StateMachine = state::RUN;

        }

        if( StateMachine == state::RUN )
        {
            // Updating snake and maze
            _levels[num_levels].refreshSnake( _levels[num_levels].cobra.snakeBody );

            _levels[num_levels].refreshMaze();

            // When ate apple restart and increasebody.
            if( _levels[num_levels].isEaten() )
            {
                _levels[num_levels].cobra.increaseBody();

                StateMachine = state::START;
            }
        }

        if( DEAD )
        {
            _levels[num_levels].cobra.lostLife();

            _levels[num_levels].cobra.resetBody();

            _levels[num_levels].refreshMaze();

            _levels[num_levels].resetPos();

            StateMachine = state::DEAD;
        }
    }

//===============================================================================================
    bool SnakeGame::game_over()
    {
        if( StateMachine == state::DEAD )
        {
            if( _levels[num_levels].cobra.life() == 0 )
            {
                StateMachine = state::GAME_OVER;
                //print game over message.
            }
            else
            {
                StateMachine = state::START;
            }
        }

        if( _levels[num_levels].cobra.ate() == 10 )
        {
            StateMachine = state::GAME_OVER;
            std::cout << "\n>>>END GAME<<<\n";
        }

        if( StateMachine == state::GAME_OVER ){ return true; }

        return false;


    }
//===============================================================================================
} // END NAMESPACE.
