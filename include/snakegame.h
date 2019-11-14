#ifndef SNKGAME_H
#define SNKGAME_H

#include "./maze.h"

#include <iomanip>


namespace SNAZE{



    class SnakeGame{

        private:
            //< State machine for game loop pattern.
            enum class state{ START=0, RUN, DEAD, LEVEL_UP, GAME_OVER };

            std::vector< std::pair< size_t, size_t > > measures; //< level's maze measures.

            std::vector< SNAZE::maze > _levels; //< vector of mazes.

            SNAZE::snake cobra; //< object snake

            state StateMachine;

        public:

            SnakeGame(){/*EMPTY*/}

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


            virtual void render(){}
            virtual void update(){}
            virtual void process_events(){}
            virtual bool game_over(){ return true;}
    };

} // END NAMESPACE.


#endif
