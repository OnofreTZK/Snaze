#ifndef MAZE_H
#define MAZE_H

/*
 * Class Maze file
 * Author: Tiago Onofre
 * */

#include "./list.h"
#include "./jararaca.h"

#include <fstream> // files
#include <string> // std::string
#include <cstring>
#include <cassert> // assert's tests
#include <chrono> // time manip
#include <random> // mt19937 ( random numbers )
#include <cctype> // isdigit()
#include <sstream> // stringstream
#include <stack> // std::deque( path )
#include <queue> // std::queue

#define MARK '-'
#define CROSS_MARK 'x'
#define SNK_BDY '|'

namespace SNAZE{

    class maze{

        private:

            size_t lRow; // number of rows
            size_t lCol; // number of cols

            std::string pellet; //< Apple

            sc::list< std::pair< size_t, size_t > > solution; //< list for solution coordinates.
            //< My doubly linked list.

            std::pair< size_t, size_t > start_pos; //< snake start position
            std::pair< size_t, size_t > current_pos; //< snake current position.
            std::pair< size_t, size_t > pelletPosition; //< Pellet position in the maze.
            std::pair< size_t, size_t > endLinePosition; //< Previous death position.

        public:

            //< Node to save the coordinate and number of bifurcations.
            struct Node{
                std::pair< size_t, size_t > coordinate;

                //< Valid directions to verify.
                std::stack< char > directions;

                //< tells to program if is a bifurcation.
                bool bifurcation;

                // Snake fake body to avoid move conditionals mistakes.
                std::vector< std::pair< size_t, size_t > > fakeBody;

                //< Basic constructor.
                Node( bool condition=false ) : bifurcation(condition) {/*EMPTY*/}
            };
            /*==================================================================*/

            // Public for snakegame manager.
            SNAZE::snake cobra; //< object snake


            // Maze body ========================================================
            std::vector< std::string > m_maze; //< matrix to represent the maze.
            //===================================================================


            // Constructor and destructor
            maze( size_t row, size_t col )
            {
                setMeasures( row, col );
                pellet = "\x1b[31m\x1b[0m";
            }

            ~maze(){}
            //---------------------------



            /*--------------------------- Main methods-------------------------------------*/

            //< set the value for rows and cols
            void setMeasures( size_t row, size_t col );

            //< get start position
            void setStart_pos( size_t row, size_t col );

            inline std::pair< size_t, size_t > getStart()
            {
                return start_pos;
            }

            inline void resetPos()
            {
                cobra.snakeBody[0] = start_pos;

                cobra.setDirection( NULL_DIR );
            }

            //< create maze with initial config.
            void renderMaze( std::vector< std::string > & init );

            //< Random valid position for pellet.
            void randPellet();

            //< Count walls around to verify the bifurcation.
            size_t wallCount( std::pair< size_t, size_t > const currentPos ) const;

            //< Avoid body clash.
            bool verifyBody( size_t const first, size_t const second,
                             std::vector< std::pair< size_t, size_t > > fakeBody ) const;

            //< Verify walls around the snake;
            Node checkSides( std::pair< size_t, size_t > const currentPos,
                             std::vector< std::pair< size_t, size_t > > fakeBody ) const;

            //< Move pathfinder and the fakebody.
            void moveBody( std::pair< size_t, size_t > &currentPos );

            /*-------------------------- Get Directions ------------------------------------*/

            inline std::pair< size_t, size_t > moveUp( std::pair<size_t, size_t> & currentPos )
            {
                return std::make_pair( currentPos.first - 1, currentPos.second );
            }

            inline std::pair< size_t, size_t > moveRight( std::pair<size_t, size_t> & currentPos )
            {
                return std::make_pair( currentPos.first, currentPos.second + 1 );
            }

            inline std::pair< size_t, size_t > moveDown( std::pair<size_t, size_t> & currentPos )
            {
                return std::make_pair( currentPos.first + 1, currentPos.second );
            }

            inline std::pair< size_t, size_t > moveLeft( std::pair<size_t, size_t> & currentPos )
            {
                return std::make_pair( currentPos.first, currentPos.second - 1 );
            }
            /*-----------------------------------------------------------------------------*/

            //< Backtracking algorithm.
            void backTracking( std::stack< Node > & coords );

            //< Algorithm to find the best way to apple
            void findSolution();

            //< Remove Trémaux marks.
            void clear();

            //< Update snake position.
            void refreshSnake( std::vector< std::pair< size_t, size_t > > & s_body );

            inline bool isEaten()
            {
                return this->cobra.snakeBody[0] == this->pelletPosition;
            }

            inline bool isENDLINE()
            {
                return this->cobra.snakeBody[0] == this->endLinePosition;
            }

            //< Refresh maze config with snake and pellet.
            void refreshMaze();

            //< Print current config in terminal.
            void printMaze();


    };
} // END NAMESPACE

// Parser function to get initial config.
void configParser( std::vector< SNAZE::maze > & _levels, std::string filename );

#endif
