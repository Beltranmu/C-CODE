/**
  * @file main.cc
  * @brief Main file of the game.
  * @details This is the core file. It gathers all classes of the game to make it possible.
  * @author Javier Benito Abolafio <benitoab@esat-alumni.com>
  * @version alfa 1.0
  * @date Ded-2020
  * @copyright ESAT
  */
 



#ifndef __GAME_H__
#define __GAME_H__

#include "SDL.h"


class Scene;

class Game {
  public:   
  
    static const int32_t kNScenes = 5; ///@var number of scenes
  
    //Methods
    Game();    
    ~Game(); 
    
    /**
     * @brief load a new scene
     * @param the index of the new scene to load
     * @return the index of the previous scene
     */
    int loadScene(int n_scene);
    
    /**
     * @brief load a new scene
     * @param the index of the new scene to load
     * @return the index of the previous scene
     */
     
    /**
     * @brief contains all the fuction of the main loop of the game
     */
    void mainGame();
     /**
     * @brief contains all the input fuction of the game
     */
    void input();
    /**
     * @brief contains all the update fuction of the game
     */
    void update();
    /**
     * @brief contains all the draw fuction of the game
     */
    void draw();
    /**
     * @brief contains all the init fuction of the game
     */
    int init();
    /**
     * @brief contains all the quit fuction of the game
     */
    void quit();    

    //void ImGuiSDLProcessEvent1(SDL_Event* e);
    
    // Attributes
    SDL_Window *win_; ///@var SDL window
    SDL_Renderer *ren_; ///@var SDL rendeder
    Scene* current_scene_[kNScenes]; ///@var array of scenes
    int32_t current_id_scene_;       ///@var index that tell you the current active scene
    //int32_t previous_id_scene_;
    
  
    int quit_;                          ///@var 1 == exit the game 0 == doesn't
    double current_time_;               ///@var Use to chathc the time and control the fps
    double last_time_;                  ///@var Use to chathc the time and control the fps
    const unsigned char fps = 60;       ///@var fps of the game
    


};

#endif  // __GAME_H__