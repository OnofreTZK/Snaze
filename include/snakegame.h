#ifndef SNKGAME_H
#define SNKGAME_H

#include "./maze.h"

#include <iomanip> // menu interface
#include <thread> // fps control.


namespace SNAZE{



    class SnakeGame{

        private:
            //< State machine for game loop pattern.
            enum class state{ START=0, RUN, DEAD, LEVEL_UP, GAME_OVER };

            std::vector< std::pair< size_t, size_t > > measures; //< level's maze measures.

            std::vector< SNAZE::maze > _levels; //< vector of mazes.

            state StateMachine;

            //< index to level up.
            size_t num_levels;

            std::string heart = "♡";

            //< count score game.
            size_t score;

        public:


            SnakeGame(){ num_levels=0; }

            //< Print SNAZE in output.
            friend void printIntro();

            //< print number of lifes, level, eaten apples.
            void printInterface();

            //< Destructor.
            virtual ~SnakeGame(){}

            /*< Game initializer
             * -> Read file
             * -> Render levels( mazes )
             * -> Set snake starter position.
             * -> Rand first pellet.
             * -> Alloc levels vector.
             */
            virtual void initialize( int argc, char *argv[] );

            /*< Game render
             * print the game in terminal.
             */
            virtual void render();

            /* Main game process
             * Solve the maze
             * Pass solution to the snake
             * refresh maze status
             * manipulate snake size and life
             */
            virtual void update();

            /*< Event control.
             * thread sleep here.
             */
            virtual void process_events();

            /* Verify game over situations
             * No lifes
             * Max apple eatens.
             */
            virtual bool game_over();
    };

} // END NAMESPACE.


#endif
