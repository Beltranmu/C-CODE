/*
* Antekeland 2077
* Author: Javier Benito Abolafio & Ricardo Beltrán Muriel
* Mail: benitoab@esat-alumni.com & beltranmu@esat-alumni.com
* University Development @ESAT
*/

#include "label.h"
#include <string.h>
#include <stdio.h>

Label::Label(){
  text_ = nullptr;
  font_ = nullptr;
  color_ = {255,255,255,255};
  size_ = 12; 
  dst_rect_ = {0,0,100,size_};
  
  
}


Label::~Label(){
  if(nullptr != font_){
    TTF_CloseFont(font_);
    font_ = nullptr;
  }
  if(nullptr!= text_){
    free(text_);
    text_ = nullptr;
  }
}


void Label::initFont(const char* font_paht, uint16_t size){
  
  font_ = TTF_OpenFont(font_paht,size);
  
  if(!font_){
    printf("Error to open this font: %s\n -%s", font_paht,TTF_GetError());
    // poner la de serie.
  }
  
}

void Label::changeText(const char* t){
  if(nullptr != text_){
    free(text_);
  }

  if(nullptr != t){
    int t_size = strlen(t);
    text_  = (char*)malloc(sizeof(char)*t_size+1);
    memcpy(text_,t,t_size+1);
  }
}

void Label::changeColor(const SDL_Color c){
  color_ = c;
}

void Label::init(const char* font_path, uint16_t size, 
                 const SDL_Color c, const char* t,
                 const SDL_Rect r){
    
  changeText(t);
  initFont(font_path,size);   
  color_ = c; 
  dst_rect_ = r;   
}


void Label::setPosition(const SDL_Rect* r){
  dst_rect_ = *r;
}


void Label::draw(SDL_Renderer* render){
  
  SDL_Surface* text_surface;
  text_surface=TTF_RenderUTF8_Solid(font_,text_,color_);
  if(!text_surface){
    //handle error here, perhaps print TTF_GetError at least
    printf("Adios texto :D\n");
  } 
  else {
    
    //SDL_BlitSurface(text_surface,NULL,sur,&dst_rect_);
    SDL_Texture* text_texture;
   
    text_texture = SDL_CreateTextureFromSurface(render, 
                                                text_surface);
    
    if(text_texture == NULL){
      printf("Adios texto :D\n");
    }      
                                                
    SDL_RenderCopy(render, text_texture, NULL, &dst_rect_);                                           
    //perhaps we can reuse it, but I assume not for simplicity.
    SDL_FreeSurface(text_surface);
    SDL_DestroyTexture(text_texture);
  }
}