/// @author Javier Benito Abolafio <benitoab@esat-alumni.com>
/// Texture Class definition
#ifndef __TEXTURE_H__
#define __TEXTURE_H__ 1

#include "SDL.h"
#include "SDL_image.h"

#include <stdint.h>

class Texture {
  public:
    Texture();
    ~Texture();

    void init(const char* name);
    void load(SDL_Renderer* renderer);

    static Texture* CreateTexture(const char* name);

    SDL_Surface* surface_;
    SDL_Texture* texture_;
    static uint8_t total_textures;

  private:
    const uint8_t kMaxTextures;

};

#endif // __TEXTURE_H__
