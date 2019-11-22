#include "../include/maze.h"

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

                // Get start position in maze and add to snake body( the head );
                if( m_maze[i][j] == '*' )
                {
                    setStart_pos( i, j );
                    cobra.snakeBody[0] = std::make_pair( i, j );
                }
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
            randRow = generatorRow()%lRow;
            randCol = generatorCol()%lCol;

            // Check a valid position.
            if( m_maze[randRow][randCol] == ' ' )
            {
                break;
            }
        }

        // Setting the position in maze;
        m_maze[randRow][randCol] = '@';


    }
//==============================================================================================

    maze::Node maze::checkSides( std::pair< size_t, size_t > currentPos )
    {
        maze::Node setDir;

        if( m_maze[currentPos.first + 1][currentPos.second] != '#' )
        {
            setDir.directions.push( SOUTH );
        }

        if( m_maze[currentPos.first - 1][currentPos.second] != '#' )
        {
            setDir.directions.push( NORTH );
        }

        if( m_maze[currentPos.first][currentPos.second - 1] != '#' )
        {
            setDir.directions.push( WEST );
        }

        if( m_maze[currentPos.first][currentPos.second + 1] != '#' )
        {
            setDir.directions.push( EAST );
        }

        return setDir;
    }

//==============================================================================================

    void maze::moveBody( std::pair< size_t, size_t > & currentPos )
    {
        switch( cobra.direction )
        {
            case NORTH : currentPos = this->moveUp( currentPos );
                         break;
            case SOUTH : currentPos = this->moveDown( currentPos );
                         break;
            case WEST : currentPos = this->moveLeft( currentPos );
                        break;
            case EAST : currentPos = this->moveRight( currentPos );
                        break;
            default : std::cerr << "\x1b[31mERROR:\x1b[0msnake can't be moved\n"; exit(1);

        }


    }
//==============================================================================================
    void maze::findSolution()
    {
        // Maze runner.
        std::pair< size_t, size_t > pathfinder = start_pos;

        // Snake fake body to avoid move mistakes conditionals.
        std::vector< std::pair< size_t, size_t > > fakeBody;


        //=============================================
        // Bifurcation and direction data.
        //=============================================
        // vector of bifurcations.
         std::vector< Node > coordinates(1);

        // Number of bifurcations.
        size_t curr_Pos = 0;
        //=============================================


        //< While not found the apple.
        while( m_maze[pathfinder.first][pathfinder.second] != '@' )
        {
            //< get number of bifurcations in the current position.
            coordinates[curr_Pos] = this->checkSides( pathfinder );

            // set the top direction in the stack of valid directions..
            cobra.setDirection( coordinates[curr_Pos].directions.top() );

            std::cout << "coordenada: " << pathfinder.first << ", " << pathfinder.second << "\n";

            std::cout << "direction: " << cobra.getDirection() << "\n";
            this->moveBody( pathfinder );

            cobra.snakeBody[0] = pathfinder;

            std::cout << "coordenada: " << pathfinder.first << ", " << pathfinder.second << "\n";
            break;
        }

        setStart_pos( pathfinder.first, pathfinder.second );

    }
//==============================================================================================


    void maze::printMaze()
    {
        for( int i{0}; i < lRow; i++ )
        {
            for( int j{0}; j < lCol; j++ )
            {
                // START POS/SNAKE HEAD
                if( i == cobra.snakeBody[0].first and j == cobra.snakeBody[0].second )
                {
                    std::cout << "\x1b[94m▲\x1b[0m";
                }
                // IGNORE OLD ORIGIN POS.
                else if( m_maze[i][j] == '*' )
                {
                    if( m_maze[i][j] == m_maze[cobra.snakeBody[0].first][cobra.snakeBody[0].second])
                    {
                        continue;
                    }
                    std::cout << " ";
                }
                else if( m_maze[i][j] == '#' ) // WALL
                {
                    std::cout << "█";
                }
                else if( m_maze[i][j] == '@' ) // APPLE
                {
                    std::cout << pellet;
                }
                else
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

        while( !ss.eof() )
        {

            // getting row and cols.
            ss >> row >> col;

            SNAZE::maze aux( row, col );

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

            _levels.push_back(aux); // getting space in vector of levels;

            //< Add maze to vector of levels.
            _levels[lvl++].renderMaze(aux.m_maze);

        }


  }

