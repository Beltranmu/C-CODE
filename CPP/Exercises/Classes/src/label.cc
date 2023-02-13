/// @author Javier Benito Abolafio <benitoab@esat-alumni.com>
/// Label Class implementation

#include "label.h"

void initFont(const char* src_font, uint8_t size);
uint8_t initText(const char* t);
void setColor(SDL_Color c);
void setPosition(SDL_Rect r);

Label::Label(){

  message_ = nullptr;
  font_ = nullptr;
  color_ = {255, 255, 255, 255};
  size_ = 12;
  dst_rect_ = {0, 0, 100, size_};

}

Label::Label(const Label& l){
}

Label::~Label(){

  if(nullptr != font_){

    TTF_CloseFont(font_);
    font_ = nullptr;

  }

  if(nullptr != message_){

    free(message_);
    message_ = nullptr;

  }

}

void Label::init(const char* src_font, const uint8_t size,
                 const char* msg, SDL_Color color,
                 SDL_Rect dst_rect){

  initFont(src_font, size);
  initText(msg);
  setColor(color);
  setPosition(dst_rect);

}

void Label::setColor(SDL_Color c){

  color_ = c;

}

void Label::setPosition(SDL_Rect r){

  dst_rect_ = r;

}

void Label::initFont(const char* src_font, uint8_t size){

  font_ = TTF_OpenFont(src_font, size);

  if(!font_){
    printf("TTF_OpenFont: %s\n", TTF_GetError());
  }else{
    printf("Font loaded successfully\n");
  }
}

uint8_t Label::initText(const char* t){

  if(nullptr != message_){

    free(message_);

  }

  if(nullptr == t){

    return 1;

  }else{

    uint8_t temp_size = strlen(t);
    message_ = (char*)malloc(sizeof(char)*(temp_size+1));
    memcpy(message_, t, temp_size+1);

  }

  return 0;
  
}

void Label::drawText(SDL_Renderer* ren){

  SDL_Surface* text_surface;
  if(!(text_surface = TTF_RenderUTF8_Solid(font_, message_, color_))){

    printf("Error: %s\n", TTF_GetError());

  }else{

    SDL_Texture* text_texture;
    text_texture = SDL_CreateTextureFromSurface(ren, text_surface);

    SDL_RenderCopy(ren, text_texture, NULL, &dst_rect_);

  }

}
