/**
  * @file main.cc
  * @brief Main file of the game.
  * @details This is the core file. It gathers all classes of the game to make it possible.
  * @author Ricardo Beltrán Muriel <beltranmu@esat-alumni.com> 
  * @version alfa 1.0
  * @date Dec-2020
  * @copyright ESAT
  */

#ifndef __SPRITE_H__
#define __SPRITE_H__ 1

#include "entity.h"
#include "texture.h"

class Sprite : public Entity{
  
  public:  
    //Methods
    Sprite();
    ~Sprite();
    /**
     *@brief init a sprite wit a texture and position an snip
     *@param Texture& t, texture for the sprite
     *@param const SDL_Rect* position. where do you want to draw it
     *@param const SDL_Rect* snip. Snips the sprite off the texture
    */ 
    void initSprite(Texture& t,
                    const SDL_Rect* position,
                    const SDL_Rect* snip);
    
    int width();
    int height();
    /**
     *@brief init a sprite wit a texture and position an snip
     *@param Texture& t texture for the sprite
     *@param const SDL_Rect* position where do you want to draw it
     *@param const SDL_Rect* snip the snip fo the sprite
    */
    void set_texture(const Texture& tex);
    /**
     *@brief draw the sprite
     *@param SDL_Renderer* ren, SDL rendeder
    */
    void draw(SDL_Renderer* ren) override;
    
    //Atributes
    SDL_Rect snip_rect_;

  
    Texture* texture_;
 
};

#endif