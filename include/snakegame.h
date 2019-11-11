#ifndef SNKGAME_H
#define SNKGAME_H

#include "./maze.h"

#include <iomanip>


namespace SNAZE{



    class SnakeGame{

        private:
            //< State machine for game loop pattern.
            enum class g_state{ START=0, RUN, DEAD, LEVEL_UP, GAME_OVER };

            std::vector< std::pair< size_t, size_t > > measures; //< level's maze measures.

            std::vector< SNAZE::maze > _levels; //< vector of mazes.

            SNAZE::snake cobra; //< object snake

        public:

            SnakeGame(){/*EMPTY*/}

            //< Destructor.
            virtual ~SnakeGame(){}


            virtual void initialize( int argc, char *argv[] )
            {
                if( argc < 2 )
                {
                    std::cerr << ">>>\x1b[93mToo few arguments!\x1b[0m\n"
                              << ">>>The program need a config file to start the simulation!\n"
                              << ">>>Simulation \x1b[31m[FAILED]\x1b[0m\n";
                    exit(1);
                }

                std::string filename = argv[1];

                configParser( _levels, filename );

                _levels[0].randPellet();

                _levels[0].printMaze(cobra);

            }


            virtual void render(){}
            virtual void update(){}
            virtual void process_events(){}
            virtual bool game_over(){ return true;}
    };

} // END NAMESPACE.


#endif
