#ifndef SNAKE_H
#define SNAKE_H

#define NORTH 'N'
#define EAST 'E'
#define SOUTH 'S'
#define WEST 'W'

/*-------- Libraries --------*/
#include <iostream> // cin cout
#include <cstdlib> // size_t
#include <vector> // sc vector

using life_size = size_t;

namespace SNAZE{

    class snake{

        private:

            char current_direction; //< Current direction status --> DEAFULT = NORTH.

            std::vector< size_t > body; //< body size; DEFAULT = 1.

            life_size lifes; //< Number of lifes

        public:

            //< Constructor
            snake()
            {

                // Setting default start direction.
                current_direction = NORTH;

                // head
                body.resize(1);

                // default number of lifes;
                lifes = 5;

            }


            //< Destructor
            ~snake()
            {/*EMPTY*/}


            //< Return current direction
            char const getDirection() const
            {
                return current_direction;
            }

            //<  Change direction
            void setDirection( char DIRECT )
            {
                current_direction = DIRECT;
            }


    };

} // NAMESPACE END

#endif

