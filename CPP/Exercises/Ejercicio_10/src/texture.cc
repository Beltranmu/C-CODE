/// @author Javier Benito Abolafio <benitoab@esat-alumni.com>
/// Texture Class implementation

uint8_t Texture::total_textures = 0;

#include "texture.h"

Texture::Texture(){
  name_ = nullptr;
}

Texture::~Texture(){
  SDL_DestroyTexture(texture_);
  SDL_DestroySurface(surface_);
}

void Texture::init(const char* name) {

  if(nullptr == name) return;

  surface_ = IMG_Load(name);

}

void Texture::load(SDL_Renderer* renderer) {

  texture_ = SDL_CreateTextureFromSurface(renderer, surface_);

}

Texture* Texture::CreateTexture(const char* name){

  if(Texture::total_textures < kMaxTextures){
    Texture* p_texture = new Texture();
    p_texture->init(name);
    return p_texture;
  }else{
    return nullptr;
  }
  
}
