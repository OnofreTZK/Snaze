#ifndef MAZE_H
#define MAZE_H

#include "../include/jararaca.h"

#include <fstream> // files
#include <string> // std::string
#include <cstring>
#include <cassert> // assert's tests
#include <chrono> // time manip
#include <random> //mt19937 ( random numbers )
#include <cctype> // isdigit()
#include <sstream> // stringstream

namespace SNAZE{

    class maze{

        private:

            size_t lRow; // number of rows
            size_t lCol; // number of cols

            std::string pellet; //< Apple

            std::vector< std::pair< size_t, size_t > > solution; //< vector for solution coordinates.
            std::pair< size_t, size_t > start_pos; //< snake start position

        public:

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

            //< create maze with initial config.
            void renderMaze( std::vector< std::string > & init );

            //< Random valid position for pellet.
            void randPellet();

            //< Algorithm to find the best way to apple
            void findSolution();

            //< Update snake position.
            void refreshSnake( std::vector< std::pair< size_t, size_t > > & s_body );

            //< Refresh maze config with snake and pellet.
            void refreshMaze();

            //< Print current config in terminal.
            void printMaze(SNAZE::snake const & cobra);


    };
} // END NAMESPACE

// Parser function to get initial config.
void configParser( std::vector< SNAZE::maze > & _levels, std::string filename );

#endif
