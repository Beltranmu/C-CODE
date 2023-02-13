/**
  * @file main.cc
  * @brief Main file of the game.
  * @details This is the core file. It gathers all classes of the game to make it possible.
  * @author Ricardo Beltrán Muriel <beltranmu@esat-alumni.com> 
  * @version alfa 1.0
  * @date Dec-2020
  * @copyright ESAT
  */

 
#ifndef __LABEL_H__
#define __LABEL_H__ 1


#include "SDL_ttf.h"
#include "entity.h"

class Label: public Entity{
  
  
  public:
  //Methods
  
  Label();
  ~Label();
  /**
   *@brief init the text
   *@param const char* font_path, the path of the font
   *@param uint16_t size. the size of the new text
   *@param const SDL_Color c, the color of the text
   *@param const char* t, the mensage
   *@param const SDL_Rect r, Where you want to put the text
  */
  void init(const char* font_path, uint16_t size, 
            const SDL_Color c, const char* t,
            const SDL_Rect r);
   
  /**
   *@brief init the font of the text 
   *@param const char* font_path, the path of the font
   *@param uint16_t size. the size of the new text
  */   
  void initFont(const char* font_path, 
                const uint16_t size);

  /**
   *@brief change the mensage of the text
   *@param const char* t, the mensage
  */  
  void changeText(const char* t);
  
  /**
   *@brief change the position of the text
   *@param const SDL_Rect r, Where you want to put the text
  */
  void setPosition(const SDL_Rect* r);
  
  /**
   *@brief init the position and the size of the box where the text is going to be
   *@param const int x, position x
   *@param const int y, position y
   *@param const int w, width
   *@param const int h, height
  */
  
  void initRect(const int x, const int y,
                const int w, const int h);
  
  /**
   *@brief Draw the text
   *@param  SDL_Renderer* render. SDL renderer
  */
  
  void draw(SDL_Renderer* render) override;
  /**
   *@brief Change the color of the text
   *@param  const SDL_Color c. the new color
  */
  void changeColor(const SDL_Color c);
  
  // Atributes
  
  char* text_;      ///@var the text 
  TTF_Font* font_;  ///@var the font of the text
  SDL_Color color_; ///@var the color of the text
  uint16_t size_;   ///@var the size of the text
    
  
  
  
};

#endif