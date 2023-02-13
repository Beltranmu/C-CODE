/**
  * @file main.cc
  * @brief Main file of the game.
  * @details This is the core file. It gathers all classes of the game to make it possible.
  * @author Javier Benito Abolafio <benitoab@esat-alumni.com> 
  * @version alfa 1.0
  * @date Ded-2020
  * @copyright ESAT
  */
 



#ifndef __CUSTOMIZATION_H__
#define __CUSTOMIZATION_H__ 1

#include "SDL.h"
#include "SDL_image.h"
#include "gamemanager.h"
#include "texture.h"
#include "imgui_internal.h"
#include <string>

static ImFont* newFont; ///@var the font used
static unsigned char id_[13]; ///@var the id of each cosmetic
static SDL_Surface* images_[41]; ///@al the images need to load the textures
//static SDL_Texture* textures_[41];


void ImGuiSDLProcessEvent(SDL_Event* e);
/**
 *@brief init the custimization values
*/
void InitCustomization();
/**
 *@brief Change the texture of the character and free the old one
 *@param SDL_Surface** img. The surface of the texture
 *@param char* dir. the path of the new texture.
 *@param unsigned char* id. the current id of the cosmetic
 *@param int32_t skin_id. the new id of the cosmetic
 *@param SDL_Texture** texture .the texture
 *@param SDL_Renderer* ren, SDl rendeder
*/
void SetImage(SDL_Surface** img, char* dir, unsigned char* id, 
              int32_t skin_id, SDL_Texture** texture, 
              SDL_Renderer* ren);
/**
 *@brief Draw the preview of the character 
 *@param SDL_Renderer* ren, SDL renderer
 *@param Character c. The character you want to draw
 */
void DrawCharacter(SDL_Renderer* ren, Character c);
/**
 *@brief Draw the imgui window of character sutomization
 *@param Character *c. The current character
*/
void CustomizeCharacter(Character *c);
/**
 *@brief Draw the IMgui window
*/
void DrawCustomization();
/**
 *@brief Load all the characters
 *@param SDL_Renderer* ren. SDL renderer to load textures
*/
void LoadCharacter(SDL_Renderer* ren);
/**
 *@brief Do all the thing that need to quit the customization
*/
void QuitCustomization();

#endif  // __CUSTOMIZATION_H__