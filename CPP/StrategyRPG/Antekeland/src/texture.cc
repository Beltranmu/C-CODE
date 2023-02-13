/*
* Antekeland 2077
* Author: Javier Benito Abolafio & Ricardo Beltrán Muriel
* Mail: benitoab@esat-alumni.com & beltranmu@esat-alumni.com
* University Development @ESAT
*/

#include "texture.h"
#include "SDL_image.h"
#include <stdio.h>

int32_t Texture::total_textures_ = 0;

Texture::Texture(){
  ++total_textures_;
  texture_ = nullptr;
}

Texture::~Texture(){
  if(nullptr == texture_)
  SDL_DestroyTexture(texture_);
}

//Factory
Texture* Texture::CreateTexture(const char* img_path,
                                SDL_Renderer* r){   
    
  if(total_textures_< kMaxTextures){
    Texture* tmp_texture = new Texture();
    tmp_texture->loadTexture(img_path, r);
    return tmp_texture;
  }
  else{
    return nullptr;
  }
}

Texture* Texture::CreateEmptyTexture(){   
    
  if(total_textures_< kMaxTextures){
    Texture* tmp_texture = new Texture();
    return tmp_texture;
  }
  else{
    return nullptr;
  }
}

int Texture::loadTexture(const char* img_path, SDL_Renderer* r){

  if(nullptr == img_path){ 
    return 1;
  }
  
  if(texture_){
    SDL_DestroyTexture(texture_);
  }

 texture_= nullptr;
 SDL_Surface *img = IMG_Load(img_path);
  if (img == nullptr){
    return 1;
  }

  texture_ = SDL_CreateTextureFromSurface(r, img);
  SDL_FreeSurface(img);
  
  if (texture_ == nullptr){
    return 1;
  }

  return 0;
}
  
 /*
void Texture::loadTexture(int32_t width, int32_t height, uint8_t* data ){
  
  if(width<1 || height <1 || (NULL == data)){
    return;
  }
  
  if(texture_){
    SDL_DestroyTexture(texture_);
  }
  
  //TODO errors and load texture from memory
}
 
  */
int32_t Texture::width(){
  
  int32_t w, h;
  
  SDL_QueryTexture(texture_,NULL,NULL,&w,&h);
  
  return w;
}


int32_t Texture::height(){
  
  int32_t w, h;
  
  SDL_QueryTexture(texture_,NULL,NULL,&w,&h);
  
  return h;
}