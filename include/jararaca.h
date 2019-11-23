#ifndef SNAKE_H
#define SNAKE_H

#define NORTH 'N'
#define EAST 'E'
#define SOUTH 'S'
#define WEST 'W'
#define NULL_DIR '0'

/*-------- Libraries --------*/
#include <iostream> // cin cout
#include <cstdlib> // size_t
#include <vector> // sc vector
#include <utility> // std::pair
#include <iterator>


/*-------- ALIAS --------*/
using life_size = size_t;
using body_size = size_t;

namespace SNAZE{

    class snake{

        private:

            life_size lifes; //< Number of lifes

        public:

            // Public for class maze.
            std::vector< std::pair< size_t, size_t > > snakeBody; //< Snake body.

            // Public for class SnakeGame
            char direction; //< Current direction status --> DEFAULT = NULL.
      /*--------------------------------- Methods ----------------------------------------*/

            //< Constructor
            snake()
            {

                // Setting default start direction.
                direction = NULL_DIR;

                // Allocating head.
                snakeBody.resize(1);

                // default number of lifes;
                lifes = 5;

            }


            //< Destructor
            ~snake()
            {/*EMPTY*/}


            //< Return current direction
            inline char const getDirection() const
            {
                return direction;
            }

            //<  Change direction
            inline void setDirection( char DIRECT )
            {
                direction = DIRECT;
            }

            //< Increase body after eat
            inline void increaseBody( size_t const i, size_t const j )
            {
                snakeBody.push_back( std::make_pair( i, j ) );
            }

            //< Reset body to the initial config.
            inline void resetBody()
            {
                snakeBody.erase( snakeBody.begin() + 1, snakeBody.end() );
            }

            //< Decrement life
            inline void lostLife()
            {
                lifes--;
            }

            //< return body size
            inline body_size size()
            {
                return snakeBody.size();
            }

            //< return number of lifes
            inline life_size life()
            {
                return lifes;
            }


    };

} // NAMESPACE END

#endif

