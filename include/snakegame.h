#ifndef SNKGAME_H
#define SNKGAME_H


namespace SNAZE{



    class SnakeGame{

        private:
            enum class g_state = { START=0, RUN, DEAD, LEVEL_UP, GAME_OVER };
        public:


          ~SnakeGame()
          virtual void initiliaze()
          virtual void render()
          virtual void update()
          virtual void process_events()
          virtual bool game_over()
    };




#endif
