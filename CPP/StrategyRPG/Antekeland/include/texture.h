/**
  * @file main.cc
  * @brief Main file of the game.
  * @details This is the core file. It gathers all classes of the game to make it possible.
  * @author Ricardo Beltrán Muriel <beltranmu@esat-alumni.com> 
  * @version alfa 1.0
  * @date Dec-2020
  * @copyright ESAT
  */


#ifndef __TEXTURE_H__
#define __TEXTURE_H__ 1

#include "SDL.h"
#include <stdint.h>

class Texture{
  public: 
    static const int32_t kMaxTextures = 100; ///@var max num of textures for the game
    
    //Methods 
    ~Texture();
    /**
     *@brief load a new texture
     *@param const char* img_path, the path of the texture
     *@param SDL_Renderer* r, SDL renderer
     *@return 0 if all ok, 1 not okay
    */ 
    int loadTexture(const char* img_path, SDL_Renderer* r);
    
    //void loadTexture(int32_t width, int32_t height, uint8_t* data );
    int32_t width();
    int32_t height();
    
    //Factory
    /**
     *@brief Create a new texture
     *@param const char* img_path, the path of the texture
     *@param SDL_Renderer* r, SDL renderer
     *@return the texture just created
    */ 
    static Texture* CreateTexture(const char* img_path,
                                  SDL_Renderer* r);
    /**
     *@brief Create a new texture, but empty
     *@return the texture just created
    */                              
    static Texture* CreateEmptyTexture();
  
  //Atributes
  SDL_Texture* texture_; ///@var the texture
  
  private:
    static int32_t total_textures_; ///@var total of texture already crated
    Texture();

};

#endif  // __TEXTURE_H__