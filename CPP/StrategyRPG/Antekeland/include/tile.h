/**
  * @file main.cc
  * @brief Main file of the game.
  * @details This is the core file. It gathers all classes of the game to make it possible.
  * @author Javier Benito Abolafio <benitoab@esat-alumni.com>
  * @version alfa 1.0
  * @date Ded-2020
  * @copyright ESAT
  */
 


#ifndef __TILE_H__  
#define __TILE_H__ 1


#include "sprite.h"

class Tile : public Sprite {

  public:
    // Methods
    Tile();    
    ~Tile();
    /**
     *@brief Init a tile
     *@param const SDL_Rect* r, Where thi tile is
     *@param const uint8_t t, his type
     *@param const uint8_t s, his state
     *@param const uint8_t e, enabled or not
    */ 
    void init(const SDL_Rect* r, const uint8_t t, const uint8_t s, const int8_t e);
    /**
     *@brief Set the size and position of the board tiles
    */ 
    void initSubSprite(); // To be done when the map is already created
    
    /**
     *@brief Draw a tile and if it necesary it duplicate it to do spherical world
     *@param SDL_Renderer* ren, SDL renderer
    */ 
    void draw(SDL_Renderer* ren)override;
    
    //Atributes 
    uint8_t state_;   ///@var  state of the tile. Whichi biome is the tile
    uint8_t type_;    ///@var type of the tile. if it has a rock or some decoration.
    
    // Inherites SDL_Rect dst_rect_ from Entity
  
};

#endif  // __TILE_H__
