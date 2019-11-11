#ifndef MAZE_H
#define MAZE_H

#include "../include/jararaca.h"

#include <fstream> // files
#include <string> // std::string
#include <cstring>

namespace SNAZE{

    class maze{

        private:

            size_t lRow; // number of rows
            size_t lCol; // number of cols

            std::vector< std::pair< size_t, size_t > > solution; //< vector for solution coordinates.

        public:

            std::vector< std::string > m_maze; //< matrix to represent the maze.

            maze(){/*EMPTY*/}

            ~maze(){}

            //< set the value for rows and cols
            void setMeasures( size_t row, size_t col )
            {
                lRow = row;
                lCol = col;


                /*---- Allocating vector ----*/
                std::string s;
                s.reserve( lCol );
                m_maze.resize( lRow, s );
            }

            //< create maze with initial config.
            void renderMaze( std::vector< std::string > & init );

            //< Algorithm to find the best way to apple
            void findSolution();

            //< Update snake position.
            void refreshSnake( std::vector< std::pair< size_t, size_t > > & s_body );

            //< Refresh maze config
            void refreshMaze();

            //< Print current config in terminal.
            void printMaze();


    };
} // END NAMESPACE

void configParser( std::vector< SNAZE::maze > & _levels, std::string filename );

// Source
#include "./maze.inl"

#endif
