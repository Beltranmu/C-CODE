#include "gamemanager.h"
#include <stdio.h>
#include "imgui.h"
/*
* Antekeland 2077
* Author: Javier Benito Abolafio & Ricardo Beltrán Muriel
* Mail: benitoab@esat-alumni.com & beltranmu@esat-alumni.com
* University Development @ESAT
*/

#include "imgui_sdl.h"
#include "SDL_image.h"

int GameManager::init(){
  const char* imagePath = "../data/hello.png";
  const char* map_path = "../data/resources/tileset.png";
  
 
  // DataBase
  
  
  //SDL
  if (SDL_Init(SDL_INIT_VIDEO) != 0){
		printf("SDL_Init Error: %s\n", SDL_GetError());
		return 1;
	}

  win_ = SDL_CreateWindow("Antekeland_2077", 100, 100, kWindowWidth, kWindowHeight, SDL_WINDOW_SHOWN);
  if (win_ == nullptr){
    //std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return 1;
  }

  ren_ = SDL_CreateRenderer(win_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (ren_ == nullptr){
    SDL_DestroyWindow(win_);
    // std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return 1;
  }
  
  //SDL_IMG
  int flags= IMG_INIT_PNG;
  IMG_Init(flags);
  
  
  //ImGui
  //ImGui::CreateContext();
	//ImGuiSDL::Initialize(ren_, kWindowWidth, kWindowHeight);
  if(sprite_.loadTexture(imagePath, ren_) == 1){
    SDL_DestroyRenderer(ren_);
    SDL_DestroyWindow(win_);
    // std::cout << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return 1;
  }
  
  if(board_.map_sprite_.loadTexture(map_path, ren_) == 1){
    SDL_DestroyRenderer(ren_);
    SDL_DestroyWindow(win_);
    // std::cout << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return 1;
  }
   
  quit_ = 0;
  
  board_.initMap(kWindowWidth, kWindowHeight);
  
  return 0;
}

void GameManager::quit(){
  
  //ImGuiSDL::Deinitialize();
  
  //Resto off the other quit functions
  // Clean up our objects and quit
  
  
  SDL_DestroyTexture(sprite_.texture_);
  SDL_DestroyTexture(board_.map_sprite_.texture_);
  SDL_DestroyRenderer(ren_);
  SDL_DestroyWindow(win_);
  // Cleanup
  //ImGui::DestroyContext();
  
  IMG_Quit();
  SDL_Quit();
  printf("Me he cerrado perfe. Adioh");
  
}

//It will be modified in a future
/** @brief Process mouse events for the ImGuiSDL binding.
 *
 * @param Pointer to an SDL_Event captured this frame.
 */


void GameManager::ImGuiSDLProcessEvent1(SDL_Event* e) {
  if (e == nullptr) return;

  ImGuiIO& io = ImGui::GetIO();

  int wheel = 0;

  while (SDL_PollEvent(e))
  {
    if (e->type == SDL_WINDOWEVENT)
    {
      if (e->window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
      {
        io.DisplaySize.x = static_cast<float>(e->window.data1);
        io.DisplaySize.y = static_cast<float>(e->window.data2);
      }
    }
    else if (e->type == SDL_MOUSEWHEEL)
    {
      wheel = e->wheel.y;
    }
  }

  int mouseX, mouseY;
  const int buttons = SDL_GetMouseState(&mouseX, &mouseY);

  // Setup low-level inputs (e.g. on Win32, GetKeyboardState(), or 
  // write to those fields from your Windows message loop handlers, etc.)

  io.DeltaTime = 1.0f / 60.0f;
  io.MousePos = ImVec2(static_cast<float>(mouseX), static_cast<float>(mouseY));
  io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
  io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);
  io.MouseWheel = static_cast<float>(wheel);
}

void GameManager::input(){
  
  SDL_Event event;
  while (SDL_PollEvent(&event))
  {
    //ImGuiSDLProcessEvent1(&event);
    if (event.key.keysym.sym == SDLK_ESCAPE || event.type == SDL_QUIT ) {
      quit_ = true;
    }
    if (event.type == SDL_WINDOWEVENT && 
      event.window.event == SDL_WINDOWEVENT_CLOSE && 
      event.window.windowID == SDL_GetWindowID(win_)) {
      quit_ = true;
    }
    // Add the rest of input events here: keyboard, gamepad, etc...
  }
  
}

void GameManager::draw(){
  
  // Start the Dear ImGui frame
  //ImGui::NewFrame();

  //ImGui Window
  //ImGui::ShowDemoWindow();

  //First clear the renderer
  SDL_RenderClear(ren_);

  //Draw the texture
  board_.drawMap(ren_);
  //SDL_RenderCopy(ren_, sprite_.texture_, NULL, NULL);

  //ImGui
  //ImGui::Render();
  //ImGuiSDL::Render(ImGui::GetDrawData());

  //Update the screen
  SDL_RenderPresent(ren_);
  
}


void GameManager::game(){
  
  init();
  
  while(!quit_)
  {
    
    input();
    //update();
    draw();
    
  
  }

  
  quit();
  
}