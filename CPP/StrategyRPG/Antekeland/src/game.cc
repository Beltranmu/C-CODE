/**
 * Antekeland 2020
 * Author: Javier Benito Abolafio
 * Mail: benitoab@esat-alumni.com
 * University Development @ESAT
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "imgui.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "game.h"
#include "gamemanager.h"
#include "grid.h"
#include "customization.h"
#include "menuscene.h"
#include "mainscene.h"
#include "custoscene.h"
#include "customization.h"
#include "combatscene.h"

#include "imgui.h"
#include "imgui_sdl.h"

Game::Game(){
  win_ = nullptr;
  ren_ = nullptr;
  for(int i = 0; i < Game::kNScenes; ++i){
     current_scene_[i] = nullptr;
  }

  //previous_scene_ = nullptr;
  quit_ = 0;
  current_time_ = 0;
  last_time_ = 0;
  current_id_scene_=0;
}

Game::~Game(){

  if(nullptr != win_){ SDL_DestroyWindow(win_); }
  if(nullptr != ren_){ SDL_DestroyRenderer(ren_); }
}

int Game::loadScene(int n_scene){
  
  int32_t last_scene = current_id_scene_;
  current_scene_[current_id_scene_]->quit();
  current_id_scene_ = n_scene;
  current_scene_[current_id_scene_]->init();
  
  return last_scene;
}


int Game::init(){
  //SDL
  if (SDL_Init(SDL_INIT_VIDEO) != 0){
		printf("SDL_Init Error: %s\n", SDL_GetError());
		return 1;
	}

  win_ = SDL_CreateWindow("Antekeland", 100, 100, GameManager::kWindowWidth,
                          GameManager::kWindowHeight, SDL_WINDOW_SHOWN);
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
  
  //SDL_TEXT
  if(TTF_Init() == -1){
    printf("Error al cargar SDL_ttf\n"); 
  }

  GameManager& gM = GameManager::Instantiate();
  //IMGUI
  gM.window_flags |= ImGuiWindowFlags_NoMove;
  gM.window_flags |= ImGuiWindowFlags_NoResize;
  gM.window_flags |= ImGuiWindowFlags_NoCollapse;

  ImGui::CreateContext();
  

  ImGuiIO& io = ImGui::GetIO();
  newFont = io.Fonts->AddFontFromFileTTF("../data/fonts/BreathFire.ttf", 16.0f);  
  io.Fonts->Build();
  
  ImGuiSDL::Initialize(ren_, gM.kWindowWidth, gM.kWindowHeight);	 
  
  time_t t = 1608586990;//time(NULL);
  
  
  gM.init(ren_);
  //CreateBoard();
  
  //DB
  char* p = "../data/database/antekeland.db";
  gM.data_base_.init();
  gM.data_base_.openDB(p); 
  // gM.data_base_.readGame();
  //gM.data_base_.readBoardData();
  gM.data_base_.readProfessionData();
  gM.data_base_.readAttacksData();
  gM.data_base_.loadData();

  //gM.data_base_.loadBoard();
 
  //gM.data_base_.closeDB();
  
  
  //NEW GAME
  //InitLogic();
  //CreateMap();

  /*gM.player_[0].init(0,0);  
  gM.player_[1].init(0,1);  
  gM.player_[2].init(0,2);  
  gM.player_[3].init(0,3); */ 
  
  gM.player_[0].init(0);  
  gM.player_[1].init(1);  
  gM.player_[2].init(2);  
  gM.player_[3].init(3);  
  gM.player_[4].init(0);  

  //current_scene_[0] = new Customization();
  current_scene_[3] = new MenuScene();
  current_scene_[3]->init();

  current_scene_[2] = new CombatScene();
  //current_scene_[2]->init();

  current_scene_[1] = new MainScene();
  //current_scene_[1]->init();

  current_scene_[0] = new CustoScene();
  //current_scene_[0]->init();

  
  current_id_scene_ = 3;

  return 0;
}

void Game::quit(){
  
  // Rest of the other quit functions
  // Clean up our objects and quit
  
  SDL_DestroyRenderer(ren_);
  SDL_DestroyWindow(win_);
  GameManager::Instantiate().data_base_.closeDB();
  
  // Cleanup
  QuitCustomization();
  IMG_Quit();
  SDL_Quit();
  TTF_Quit();
  
  printf("Me he cerrado perfe. Adioh");
  
}

void Game::input(){
  
  last_time_ = SDL_GetTicks();
  GameManager& gM = GameManager::Instantiate();
  
  SDL_Event event;
  while (SDL_PollEvent(&event))
  {
    ImGuiSDLProcessEvent(&event);
    if (event.type == SDL_QUIT ) {
      quit_ = true;      
    }
    if(event.key.keysym.sym == SDLK_8){ loadScene(0);}
    if(event.key.keysym.sym == SDLK_9){ loadScene(1);}
    if(event.key.keysym.sym == SDLK_0){ loadScene(2);}
    if(event.key.keysym.sym == SDLK_7){ loadScene(3);}
    // if(event.key.keysym.sym == SDLK_3){ ;}
    // if(event.key.keysym.sym == SDLK_4){ ;}
    // if(event.key.keysym.sym == SDLK_5){ ;}

    current_scene_[current_id_scene_]->input(&event);
  
    if (event.type == SDL_WINDOWEVENT && 
      event.window.event == SDL_WINDOWEVENT_CLOSE && 
      event.window.windowID == SDL_GetWindowID(win_)) {
      quit_ = true;
    }
    // Add the rest of input events here: keyboard, gamepad, etc...
  }

  // Load Scene from Customization
  if(gM.current_edit_ == 4){
    gM.player_[4].init(gM.player_[0].profession_, 4); 
    gM.player_[4].cpyCharacter(gM.player_[0]);
    loadScene(1);
    gM.current_edit_ = 0;
    SDL_SetWindowSize(win_, 1024, 640);
  }

  if(gM.new_game_ == 1){    
    loadScene(0);
    gM.new_game_ = 0;
  }

  if(gM.enter_cave_ == 1){
    loadScene(2);
    gM.enter_cave_ = 0;
  }
  //load game 
  if(gM.new_game_ == 2){
    loadScene(1);
    gM.new_game_ = 0;
    LoadCharacter(ren_);
    gM.player_[4].cpyCharacter(gM.player_[0]);
    SDL_SetWindowSize(win_, 1024, 640);
  }
  
}

void Game::update(){

  //GameManager& gM = GameManager::Instantiate();
 
  current_scene_[current_id_scene_]->update();

}

void Game::draw(){

  //GameManager& gM = GameManager::Instantiate();
  
  SDL_SetRenderDrawColor(ren_,222,208,158,87);
  SDL_RenderClear(ren_);

  current_scene_[current_id_scene_]->draw(ren_);
  current_scene_[current_id_scene_]->drawImgui(ren_);

  //Update the screen
  SDL_RenderPresent(ren_);
  
  do{
    current_time_ = SDL_GetTicks();
  }while((current_time_ - last_time_) <= (1000.0/fps));
}

void Game::mainGame(){
  
  init();

  while(!quit_){
    
    input();    

    update();   
 
    draw();
    
  }

  quit();
}