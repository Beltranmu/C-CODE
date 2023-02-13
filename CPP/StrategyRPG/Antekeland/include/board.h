/**
  * @file main.cc
  * @brief Main file of the game.
  * @details This is the core file. It gathers all classes of the game to make it possible.
  * @author Javier Benito Abolafio <benitoab@esat-alumni.com>
  * @version alfa 1.0
  * @date Ded-2020
  * @copyright ESAT
  */
 



#ifndef __BOARD_H__
#define __BOARD_H__ 1

#include "tile.h"


class Board{  
  public:
    //Constant of procedural generation
    static const unsigned char kBoardSize = 64; ///@var size of the procedural board
    const unsigned char kNState = 8;            ///@var number of biomes of the board
    const float kCellconcentration = 0.55f;     ///@var the conectration of all biomes 

    //Methods
    /**
     *@brief init the layer 1 of the graphic board
    */
    void initLayer1();
    /**
     *@brief init the layer 2 of the graphic board
    */    
    void initLayer2();
    /**
     *@brief udate the origin position of the map
    */  
    void update0Position();
     /**
     *@brief reset the origin position of the map
    */ 
    void reset0Position();
    //void move0Position(SDL_Event* e);
    /**
     *@brief dra the map
    */ 
    void drawMap(SDL_Renderer* renderer);
      
    //Atributes

    Tile map_[kBoardSize][kBoardSize]; ///@var arry of tiles of the map
    static int32_t x_origin_;         ///@var pos x of the origin of the map 
    static int32_t y_origin_;         ///@var pos y of the origin of the map 


};

#endif  // __BOARD_H__