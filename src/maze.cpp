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

//==============================================================================================

    void maze::setStart_pos( size_t row, size_t col )
    {
        start_pos = std::make_pair( row, col );
        current_pos = start_pos;
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

    void maze::clear()
    {
        for( int i{0}; i < m_maze.size(); i++ )
        {
            for( int j{0}; j < m_maze[i].size(); j++ )
            {
                if( m_maze[i][j] == MARK or m_maze[i][j] == CROSS_MARK or m_maze[i][j] == '@'
                    or m_maze[i][j] == SNK_BDY )
                {
                    m_maze[i][j] = ' ';
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

        // Saving position.
        pelletPosition = std::make_pair( randRow, randCol );


    }
//==============================================================================================

    size_t maze::wallCount( std::pair< size_t, size_t > const currentPos ) const
    {
        size_t count = 0;

        for( short int i{-1}; i < 1; i++ )
        {
            for( short int j{-1}; j < 1; j++ )
            {
                if( m_maze[currentPos.first + i][currentPos.second + j] == '#' )
                {
                    count++;
                }
            }
        }

        return count;

    }

//==============================================================================================

    maze::Node maze::checkSides( std::pair< size_t, size_t > const currentPos ) const
    {
        maze::Node setDir;

        if( cobra.getDirection() == NULL_DIR )
        {
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
        }
        else // The snake can't reverse so we need to counter the opposite direction.
        {
            if( m_maze[currentPos.first + 1][currentPos.second] != '#' ) 
            {
                if( m_maze[currentPos.first + 1][currentPos.second] != CROSS_MARK )
                {
                    if( cobra.getDirection() != NORTH )
                    {
                        setDir.directions.push( SOUTH );
                    }
                }
            }

            if( m_maze[currentPos.first - 1][currentPos.second] != '#' )
            {
                if( m_maze[currentPos.first - 1][currentPos.second] != CROSS_MARK )
                {
                    if( cobra.getDirection() != SOUTH )
                    {
                        setDir.directions.push( NORTH );
                    }
                }
            }

            if( m_maze[currentPos.first][currentPos.second - 1] != '#' )
            {
                if( m_maze[currentPos.first][currentPos.second - 1] != CROSS_MARK )
                {
                    if( cobra.getDirection() != EAST )
                    {
                        setDir.directions.push( WEST );
                    }
                }
            }

            if( m_maze[currentPos.first][currentPos.second + 1] != '#' )
            {
                if( m_maze[currentPos.first][currentPos.second + 1] != CROSS_MARK )
                {
                    if( cobra.getDirection() != WEST )
                    {
                        setDir.directions.push( EAST );
                    }
                } 
            }

            // Inform if is a bifurcation or not.


            if( setDir.directions.size() > 1 )
            {
                if( this->wallCount( currentPos ) > 5 )
                {
                    setDir.bifurcation = true;
                }
            }

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

    void maze::backTracking( std::stack< Node > & coords, bool & deadline )
    {
        while( not ( coords.top().directions.size() > 1 )  )
        {
            coords.pop();
            // There is no more backtracking
            if( coords.size() == 0 )
            {
                deadline = true;
                Node temp;
                temp.coordinate = current_pos;
                coords.push(temp);
                return;
            }
        }

        // Remove old direction.
        if( coords.top().directions.size() > 1 )
        {
            coords.top().directions.pop();
        }
    }

//==============================================================================================
    void maze::findSolution( bool &deadline )
    {
        // Maze runner and marker( Trémaux algorithm ).
        std::pair< size_t, size_t > pathfinder = current_pos;
        std::pair< size_t, size_t > markerPrev;
        //===================================================

        // vector of bifurcations.
         std::stack< Node > coordinates;
        //=============================================

        // Snake fake body to avoid move conditionals mistakes.
        std::queue< std::pair< size_t, size_t >,
                    std::vector< std::pair< size_t, size_t > > > fakeBody( cobra.snakeBody );


        //< While not found the apple.
        while(  m_maze[pathfinder.first][pathfinder.second] != '@' )
        {
            // Set the position in the stack ===================
            coordinates.push( checkSides( pathfinder ) );

            coordinates.top().coordinate = pathfinder;
            //==================================================

            // No valid position is endline situation --> the snake die.
            if( coordinates.top().directions.size() == 0 )
            {
                backTracking( coordinates, deadline );

                pathfinder = std::make_pair( coordinates.top().coordinate.first,
                                             coordinates.top().coordinate.second );

                // After backtracking all over stack.
                if( deadline == true ){ return; }
            }

            // set the top direction in the stack of valid directions..
            cobra.setDirection( coordinates.top().directions.top() );


            /*-----------------------------------------------------------*/
            // Positioning marker and marking maze.
            markerPrev = pathfinder;

            if( m_maze[markerPrev.first][markerPrev.second] == MARK )
            {
                m_maze[markerPrev.first][markerPrev.second] = CROSS_MARK;
            }
            else
            {
                m_maze[markerPrev.first][markerPrev.second] = MARK;
            }

            // Moving.
            moveBody( pathfinder );

            // Mark current position only if are a bifurcation
            // to eliminate that way in case of endline.
            if( coordinates.top().bifurcation == true )
            {
                if( m_maze[pathfinder.first][pathfinder.second] == MARK )
                {
                    m_maze[pathfinder.first][pathfinder.second] = CROSS_MARK;
                }
                else
                {
                    m_maze[pathfinder.first][pathfinder.second] = MARK;
                }
            }
            /*-----------------------------------------------------------*/
        }

        // Adding the final position.
        coordinates.push( new Node );
        coordinates.top().coordinate = std::make_pair( pathfinder.first, pathfinder.second );

        // Reversing and passing positions to list of solutions.
        while( not coordinates.empty() )
        {
            solution.push_front( std::make_pair( coordinates.top().coordinate.first,
                                           coordinates.top().coordinate.second ) );
            coordinates.pop(); // Eliminating the stack
        }

        //< Setting new start position --> pellet old position.
        current_pos = pathfinder;

    }
//==============================================================================================

    void maze::refreshSnake( std::vector< std::pair< size_t, size_t > > & s_body )
    {
        auto first = s_body.begin();
        auto last = s_body.end() - 1;


        //< Start ou post death situation.
        if( cobra.size() == 1 )
        {
            cobra.snakeBody[0] = std::make_pair(solution.front().first, solution.front().second);
            solution.pop_front();
        }
        else
        {
            //< Moving body with solution positions.
            while( first != last )
            {
                std::iter_swap( last, first );
                last--;
            }

            //< adding next position to the front.
            cobra.snakeBody[0] = std::make_pair(solution.front().first, solution.front().second);
            solution.pop_front();
        }
    }


//==============================================================================================

    void maze::refreshMaze()
    {
          clear();

          auto first = cobra.snakeBody.begin();
          auto last = cobra.snakeBody.end();

          while ( first != last )
          {
              m_maze[first->first][first->second] = SNK_BDY;
              first++;
          }
    }
//==============================================================================================

    void maze::printMaze()
    {
        for( int i{0}; i < lRow; i++ )
        {
            for( int j{0}; j < lCol; j++ )
            {
                // START POS/SNAKE HEAD
                if( m_maze[i][j] == SNK_BDY )
                {
                   std::cout << "\x1b[94mO\x1b[0m";
                }
                // IGNORE OLD ORIGIN POS.
                else if( m_maze[i][j] == '#' ) // WALL
                {
                    std::cout << "█";
                }
                else if( i == pelletPosition.first and
                         j == pelletPosition.second ) // APPLE
                {
                    if( m_maze[i][j] == SNK_BDY )
                    {
                        continue;
                    }

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

