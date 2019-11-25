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

    void SnakeGame::printInterface()
    {

        std::stringstream sHeart;
        for( int i{0}; i < _levels[num_levels].cobra.life(); i++ )
        {
            sHeart << heart;
        }


        std::stringstream ss;
        ss <<   "\x1b[32m********************************************\n" <<
                "*                                          *\n" <<
                "*  ******  *     *  ******  ******  ****** *\n" <<
                "*  *       * *   *  *    *      *   *      *\n" <<
                "*  ******  *  *  *  * ** *    *     ****   *\n" <<
                "*       *  *   * *  *    *  *       *      *\n" <<
                "*  ******  *    **  *    *  ******  ****** *\n" <<
                "*                                          *\n" <<
                "********************************************\n\x1b[0m"

            << "\x1b[32m" << std::setfill('*') << std::setw(45) << "\n"
            << "| LIFE: \x1b[31m" << sHeart.str()
            << "  \x1b[32m|" << " LEVEL: " << num_levels + 1 << " |"
            << " APPLES: \x1b[93m " << _levels[num_levels].cobra.ate() << "\x1b[32m  |"
            << "\n\x1b[32m" << std::setfill('*') << std::setw(48) << "\x1b[0m" << std::endl;

        std::cout << ss.str() << "\n";

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

        // Initializing score.
        score = 0;

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
        //print interface
        printInterface();

        //print score
        std::cout << "\n\t\x1b[32mSCORE: \x1b[93m" << score << "\x1b[0m\n\n";

        //print maze
        _levels[num_levels].printMaze();
        std::cout << "\n\n";

        std::cout << "\x1b[32m" << std::setfill('*') << std::setw(46) << "\n\n";
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

        // Change Level.
        if( StateMachine == state::LEVEL_UP )
        {
            if( _levels.size() > 2 )
            {
                num_levels++;

                // Avoid seg fault.
                if( num_levels >= _levels.size() - 1 )
                {
                     num_levels = 0;
                }

                if( num_levels == 0 )
                {
                    _levels[num_levels].cobra.getSnake(_levels[_levels.size()-2].cobra,
                                                       _levels[num_levels].getStart() );
                }
                else
                {
                    _levels[num_levels].cobra.getSnake(_levels[num_levels-1].cobra,
                                                       _levels[num_levels].getStart() );
                }
            }

            // Start maze level.
            StateMachine = state::START;
        }


        if( StateMachine == state::START )
        {
            // Positioning pellet in maze.
            _levels[num_levels].randPellet();

            _levels[num_levels].findSolution();

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

                // Score control.
                score += 200;

                _levels[num_levels].cobra.increaseBody();

                if( _levels.size() > 2 )
                {

                    if( _levels[num_levels].cobra.ate()%2 == 0 )
                    {
                        if( _levels[num_levels].cobra.ate() > 0 )
                        {
                            StateMachine = state::LEVEL_UP;
                        }
                    }
                    else
                    {
                        StateMachine = state::START;
                    }

                }
                else
                {
                    StateMachine = state::START;
                }

            }

        }


        if( _levels[num_levels].cobra.isDEATH() )
        {
            /*
            while( not _levels[num_levels].isENDLINE() )
            {
                // Updating snake and maze
                _levels[num_levels].refreshSnake( _levels[num_levels].cobra.snakeBody );

                _levels[num_levels].refreshMaze();

                //print interface
                printInterface();

                //print score
                std::cout << "\n\t\x1b[32mSCORE: \x1b[93m" << score << "\x1b[0m\n\n";

                //print maze
                _levels[num_levels].printMaze();
                std::cout << "\n\n\n";
            }*/

            _levels[num_levels].cobra.lostLife();

            _levels[num_levels].cobra.resetBody();

            _levels[num_levels].resetPos();

            _levels[num_levels].refreshMaze();

            StateMachine = state::DEAD;

            // Score control.
            score -= 100;

            if( score < 0 ){ score = 0; };

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
                std::cout << "  \x1b[31m" << std::setfill('*') << std::setw(24) << "\n"
                          << "  \x1b[31m*>>>THE SNAKE DIED!<<<*\x1b[0m\n"
                          << "  \x1b[31m" << std::setfill('*') << std::setw(24) << "\n";
            }
            else
            {
                StateMachine = state::START;
                _levels[num_levels].cobra.destiny = snake::snakeState::ALIVE;
            }
        }

        if( _levels[num_levels].cobra.ate() == 10 )
        {
            StateMachine = state::GAME_OVER;
            std::cout << "  \x1b[32m" << std::setfill('*') << std::setw(32) << "\n"
                      << "  \x1b[32m*>>>THE SNAKE WON THE GAME!<<<*\x1b[0m\n"
                      << "  \x1b[32m" << std::setfill('*') << std::setw(32) << "\n";
        }

        if( StateMachine == state::GAME_OVER ){ return true; }

        return false;


    }
//===============================================================================================
} // END NAMESPACE.
