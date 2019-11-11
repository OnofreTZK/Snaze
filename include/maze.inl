#include "./maze.h"

/*
 * file source: maze.h
 * Author: Tiago Onofre
 */


namespace SNAZE{

    void maze::renderMaze( std::vector< std::string > & init )
    {
        // insert starter config.
        for( int i{0}; i < lRow; i++ )
        {
            for( int j{0}; j < lCol; j++ )
            {
                m_maze[i][j] = init[i][j];
            }
        }
    }

    void maze::printMaze()
    {
        for( int i{0}; i < lRow; i++ )
        {
            for( int j{0}; j < lCol; j++ )
            {
                if( m_maze[i][j] == '#' )
                {
                    std::cout << "█";
                }
                if( m_maze[i][j] == '*' )
                {
                    std::cout << "V";
                }
                if( m_maze[i][j] == '.' )
                {
                    std::cout << " ";
                }
                if( m_maze[i][j] == ' ' )
                {
                    std::cout << " ";
                }
            }
            std::cout << "\n";
        }
    }

} // END NAMESPACE


void configParser( std::vector< SNAZE::maze > & _levels, std::string filename )
    {
        std::ifstream inFile;

        //< Open file for input operations.
        inFile.open( filename );

        if(!inFile) // verifying file
        {
            std::cerr <<">>>\x1b[31mUnable\x1b[0m to open file!\n"
                      << ">>>Please certify you given a correct directory or"
                      << " a valid file(.dat).\n";

            exit(1); // Can't open? Abort the program.
        }

        size_t row, col; // aux variables.

        int lvl = 0; // iterator for number of levels.

/*---------------------------------- Parsing --------------------------------------------*/

        while( inFile.good() )
        {
            SNAZE::maze aux;

            //< gettinr row and cols.
            inFile >> row;
            inFile >> col;

            aux.setMeasures( row+1, col );

            /*---- Allocating vector ----
            std::string s;
            s.reserve( col );
            aux.resize( row, s );
            ---------------------------*/

            std::string buffer; // string to get lines.

            _levels.push_back(aux); // getting space in vector of levels;

            // parser to get each char
            for( int i = 0; i < row+1; i++ )
            {
                std::getline( inFile, buffer );

                for( int j = 0; j < col; j++ )
                {
                    aux.m_maze[i][j] = buffer[j];
                }
            }

            //< Add maze to vector of levels.
            _levels[lvl++].renderMaze(aux.m_maze);

        }

        inFile.close();


  }
