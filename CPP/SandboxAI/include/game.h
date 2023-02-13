/**
 * @brief Game Class, Artificial intelligence demo , ESAT - 2022
 * @file game.h
 * @author Ricardo Beltr�n Muriel <ricardobeltrandev@gmail.com>
 * demo video Sand Box: https://youtu.be/Yjhppwc-JRI Voronoi:https://youtu.be/ssdKqyDNLeA Pathfinding: https://youtu.be/ACN9w0g3cu4
 */



#ifndef __GAME_H__
#define __GAME_H__ 1

#include <stdint.h>
#include "SFML/Graphics.hpp"
#include "board.h"
#include "voronoi.h"

  /**
    *@brief Game class with the main loop of the game, and the controller of the demo
  */

class Game
{
  public:
    Game();
    ~Game();

    /**
    *@brief Function to call in the c main to open the demo.
    */
    void mainLoop();

    //! GameModeType Enum.
      /*! Each of the different demos. */
    enum GameModeType{
      NORMAL_TYPE = 0,     /*!< Sandbox mode, with agent with different types of movements. */ 
      VORONOI_TYPE,        /*!< Voronoi mode, You can create a voronoi distribution with the points you want. */ 
      PATHFINDING_TYPE,    /*!< Sandbox mode, calculate the path finding using three different types of distances. */ 
      MAX_TYPE           
    };


  protected:   

    /**
    *@brief init the screen and all the default values for the demo.
    *@param uint32_t w_width, width of the window to open.
    *@param uint32_t w_height, height of the window to open.
    */
    void init(uint32_t w_width, uint32_t w_height);

    /**
    *@brief  where the inputs are caught.
    */
    void input();

    /**
    *@brief  update of our demo
    *@param float delta_time, time of each frame.
    */
    void update(float delta_time);

    /**
    *@brief  Collect all the imgui code of the demo.
    */
    void ImguiGame();
    /**
    *@brief  Collect all the imgui code of the sandbox mode.
    */
    void ImguiSandBox();
    /**
    *@brief  Collect all the imgui code of the voronoi mode.
    */
    void ImguiVoronoi();
    /**
    *@brief  Collect all the imgui code of the Path finding mode.
    */
    void ImguiPathFinding();

    /**
    *@brief  Collect all the draw functions of the demo.
    */
    void draw();

    /**
    *@brief  make all the necessary to close the demo properly.
    */
    void end();
    
    Voronoi voronoi;          ///< Used to calculate and the Voronoi distribution.
    bool voronoiInitialized;  ///< Used to know if all voronoi data are initialized.
    uint32_t numberVPoint;    ///< How many cells/sites/points is going to be to calculate voronoi.

    //FrameControl
    //! FrameControl strut.
    /*! collect all the deferent variables to have a control of deferent updates of the game. */
    struct FramesPerModule {
      int main_game;            /*!< FPS of the main update. */ 
      int world;                /*!< FPS of the world update. */
      int input_;               /*!< FPS of the input . */
      int draw_;                /*!< FPS of the draw. */
      int ai;                   /*!< FPS of the AI update. */
      float second_per_frame;   /*!< How many second per frame  will have our application. */
    };
    int imguifps;               ///<  Counter to show on imgui how many frames are on the demo. 

    FramesPerModule fps;        ///<  Controls of fps of the game.
    GameModeType gmType;        ///<  Controls the gamemode of the demo. 
    int intgmType;              ///<  Controls the gamemode of the demo,but used on the imgui::intslider. 
    //Window
    sf::RenderWindow w_;        ///<  SFML window.
    int32_t w_width_;           ///<  With of the window.
    int32_t w_height_;          ///<  Height of the window.

    //Events
    sf::Event events_;          ///<  SFML events.

    //Texture
    
    //Board
    Board board_;              ///<  Board of the sandbox and path finding modes.
    sf::Texture map_texture_;  ///<  Texture of the board.
    sf::Sprite map_sprite_;    ///<  Sprite of the board.
      
    float delta_time_;         ///<  Delta time.


    // ImGUI
    int32_t selectedAgentID;  ///<  ID of the selected agent you want to modify.
    int32_t targetRow;        ///<  Row target of the agent.
    int32_t targetCol;        ///<  Column target of the agent.



    int32_t possibleNextTarget;             ///<  New possible target for the agents.
    bool startAddPattern;                   ///<  true -> Show the start pattern options otherwise not.
    int32_t number_movement;                ///<  Number of movement you want to add to the agent.
    Agent::PatternMovement movement_type;;  ///<  Type of movement you want to add to the agent.
 
  private:

};

#endif //__GAME_H__