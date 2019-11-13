#include "./maze.h"

/*
 * file source: maze.h
 * Author: Tiago Onofre
 */


namespace SNAZE{
//==============================================================================================
    void maze::setMeasures( size_t row, size_t col )
    {
        lRow = row;
        lCol = col;


        /*---- Allocating vector ----*/
        std::string s;
        s.resize( lCol );
        m_maze.resize( lRow, s );
    }


    void maze::setStart_pos( size_t row, size_t col )
    {
        start_pos = std::make_pair( row, col );

        // Testing.
        assert( m_maze[start_pos.first][start_pos.second] == '*' );

    }
//==============================================================================================

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
//==============================================================================================


    void maze::randPellet()
    {
        unsigned seedRow = std::chrono::system_clock::now().time_since_epoch().count();
        unsigned seedCol = std::chrono::system_clock::now().time_since_epoch().count();

        std::mt19937 generatorRow ( seedRow );
        std::mt19937 generatorCol ( seedCol );

        size_t randRow;
        size_t randCol;

        while( true )
        {
            randRow =  generatorRow()%lRow;
            randCol = generatorCol()%lCol;

            if( m_maze[randRow][randCol] == ' ' )
            {
                break;
            }
        }

        m_maze[randRow][randCol] = '@';


    }
//==============================================================================================


    /*void maze:findSolution()
    {
        
    }*/
//==============================================================================================


    void maze::printMaze( SNAZE::snake const & cobra )
    {
        for( int i{0}; i < lRow; i++ )
        {
            for( int j{0}; j < lCol; j++ )
            {
                if( m_maze[i][j] == '#' ) // WALL
                {
                    std::cout << "█";
                }
                if( m_maze[i][j] == '*' and cobra.direction == NORTH ) // START POS/SNAKE HEAD
                {
                    std::cout << "\x1b[94m▲\x1b[0m";
                }
                if( m_maze[i][j] == '.' ) // INVISIBLE WALL
                {
                    std::cout << " ";
                }
                if( m_maze[i][j] == '@' ) // APPLE
                {
                    std::cout << pellet;
                }
                if( m_maze[i][j] == ' ' ) // FREE WAY
                {
                    std::cout << " ";
                }
            }
            std::cout << "\n";
        }

    }
//==============================================================================================

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

        std::string buffer; // buffering the content in file
        std::string line; // buffering a line.
        std::stringstream ss;

        // Getting each line and putting in a stringstream.
        while( std::getline( inFile, buffer ) )
        {
            ss << buffer << std::endl;
        }

        //buffer.clear();
        //buffer = ss.str();

        inFile.close();

        while( ss.good() )
        {

            //< getting row and cols.
            ss >> row >> col;

            SNAZE::maze aux( row, col );

            _levels.push_back(aux); // getting space in vector of levels;

            // Countering blank space and endlines.
            while( ss.peek() == ' ' or ss.peek() == '\n' )
            {
                ss.get();
            }

            for( int i{0}; i < row; i ++ )
            {
                std::getline( ss, line );
                aux.m_maze[i] = line;
            }

            //< Add maze to vector of levels.
            _levels[lvl++].renderMaze(aux.m_maze);

        }


  }
