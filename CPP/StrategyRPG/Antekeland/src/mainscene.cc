#include "mainscene.h"
#include "gamemanager.h"
#include "grid.h"


MainScene::MainScene(){
  

  
}

MainScene::~MainScene(){

}



void MainScene::init(){

  GameManager& gM = GameManager::Instantiate();
  SDL_Color black = {0,0,0,50};
  
  gM.combat_.initCombat(gM.player_[0]);
  gM.combat_.current_char_ = &gM.player_[0];
  
  gM.over_world_scene_ = 1;
 
  gM.ui_rects_[0].dst_rect_.x = gM.kBoardWidth;
  gM.ui_rects_[0].dst_rect_.y = 0;
  gM.ui_rects_[0].dst_rect_.w = gM.kWindowWidth - gM.kBoardWidth;
  gM.ui_rects_[0].dst_rect_.h = gM.kWindowHeight;
  
  gM.ui_rects_[1].dst_rect_.x = 0;
  gM.ui_rects_[1].dst_rect_.y = gM.kBoardHeight;
  gM.ui_rects_[1].dst_rect_.w = gM.kWindowWidth;
  gM.ui_rects_[1].dst_rect_.h = gM.kWindowHeight - gM.kBoardHeight;
  
  gM.ui_rects_[0].fill_color_ = black;
  gM.ui_rects_[1].fill_color_ = black;
  

  
  
  // introduce all entities to draw
  
  for(int i = 0; i< Board::kBoardSize; ++i){
    for(int j = 0; j< Board::kBoardSize; ++j){    
      ent_list.push_back(&gM.layer1_.map_[i][j]); 
    }
  }
  
  
   
  if(gM.first_time_overworld_ == 1){ 
 
    int x = 0, y =0;
    do{
      x = rand()%Board::kBoardSize, y = rand()%Board::kBoardSize;
    }while(gM.layer1_.map_[y][x].type_ !=0 
      || gM.layer1_.map_[y][x].state_ !=0);
    gM.first_time_overworld_ = 0; 
    gM.player_[4].dst_rect_.x = x;
    gM.player_[4].dst_rect_.y = y;
    // gM.player_[4].dst_rect_.x = gM.kViewSize/2;
    // gM.player_[4].dst_rect_.y = gM.kViewSize/2;
    gM.player_[4].dst_rect_.w = gM.layer1_.map_[0][0].dst_rect_.w ;
    gM.player_[4].dst_rect_.h = gM.layer1_.map_[0][0].dst_rect_.h ;
    // Board::x_origin_ = gM.player_[4].dst_rect_.w * (x-(gM.kViewSize/2));
    // Board::y_origin_ = gM.player_[4].dst_rect_.h * (y-(gM.kViewSize/2));
    Board::x_origin_ = gM.player_[4].dst_rect_.w * ((gM.kViewSize/2)-x);
    Board::y_origin_ = gM.player_[4].dst_rect_.h * ((gM.kViewSize/2)-y);
  }
  
  
  ent_list.push_back(&gM.player_[4]);
  
  
  

  for(int i = 0; i<Board::kBoardSize; ++i){
    for(int j = 0; j<Board::kBoardSize; ++j){
      
      ent_list.push_back(&gM.layer2_.map_[i][j]);
    }
  }
  ent_list.push_back (&gM.ui_rects_[0]);
  ent_list.push_back (&gM.ui_rects_[1]);
  /*for(int i= 0; i<8; ++i){
    ent_list.push_back(&gM.combat_.stats_text_[i]);
  }
  for(int i = 0; i<6; ++i){
    ent_list.push_back(&gM.combat_.stats_rect_[i]);
  }*/

  // Menu Buttons
  SDL_Color button_color = {163, 143, 138, 60};
  SDL_Color text_color = {0, 0, 0, 255};
  char* font_path = "../data/fonts/BreathFire.ttf";
  int font_size = 25;
    // Save Game
  mainscene_button_[0].dst_rect_ = {720, 150, 200, 60};
  mainscene_button_[0].fill_color_ = button_color;
  
    // Inventory
  mainscene_button_[1].dst_rect_ = {720, 300, 200, 60};
  mainscene_button_[1].fill_color_ = button_color;

    // Quit Game
  mainscene_button_[2].dst_rect_ = {720, 450, 200, 60};
  mainscene_button_[2].fill_color_ = button_color;
  
  SDL_Rect dst_rect = {730, 160, 180, 40};
  mainscene_text_[0].init(font_path, (uint16_t)font_size, text_color, "Save Game", dst_rect);
  
  dst_rect = {730, 310, 180, 40};
  mainscene_text_[1].init(font_path, (uint16_t)font_size, text_color, "Inventory", dst_rect);
  
  dst_rect = {730, 460, 180, 40};
  mainscene_text_[2].init(font_path, (uint16_t)font_size, text_color, "Quit Game", dst_rect);

  ent_list.push_back(&mainscene_button_[0]);
  ent_list.push_back(&mainscene_button_[1]);
  ent_list.push_back(&mainscene_button_[2]);

  ent_list.push_back(&mainscene_text_[0]);  
  ent_list.push_back(&mainscene_text_[1]);  
  ent_list.push_back(&mainscene_text_[2]);

}

void MainScene::quit(){
  GameManager& gM = GameManager::Instantiate();
  gM.over_world_scene_ = 0;
  Scene::quit();
}

void MainScene::input(SDL_Event* eve){
  
  GameManager::Instantiate().player_[4].movCharacter(eve);
  SDL_Point mouse_position = {eve->button.x, eve->button.y};
  SDL_Rect button_dst = {720, 150, 200, 60};
  SDL_Color text_color_white = {255,255,255,255};
  SDL_Color text_color_black = {0,0,0,255};

  // Save Game
  if(SDL_PointInRect(&mouse_position, &button_dst)){

    mainscene_text_[0].changeColor(text_color_black);
    
    if(eve->type == SDL_MOUSEBUTTONDOWN &&
     eve->button.button == SDL_BUTTON_LEFT){
      printf("SAVING...\n");
      GameManager::Instantiate().data_base_.saveCharacter();
      GameManager::Instantiate().data_base_.saveGameData();
      //GameManager::Instantiate().data_base_.saveBoardData();
      printf("SAVED\n");
    }
      
  }else{
    mainscene_text_[0].changeColor(text_color_white);
  }
   
  // Inventory
  button_dst = {730, 310, 180, 40};
  if(SDL_PointInRect(&mouse_position, &button_dst)){

    mainscene_text_[1].changeColor(text_color_black);
    
    if(eve->type == SDL_MOUSEBUTTONDOWN &&
     eve->button.button == SDL_BUTTON_LEFT){

      /* Do stuff */

    }
      
  }else{
    mainscene_text_[1].changeColor(text_color_white);
  }
  
  // Quit Game
  button_dst = {730, 460, 180, 40};
  if(SDL_PointInRect(&mouse_position, &button_dst)){

    mainscene_text_[2].changeColor(text_color_black);
    
    if(eve->type == SDL_MOUSEBUTTONDOWN &&
     eve->button.button == SDL_BUTTON_LEFT){

      /* Do stuff */

    }
      
  }else{
    mainscene_text_[2].changeColor(text_color_white);
  }
 
}

void MainScene::update(){
  GameManager& gM = GameManager::Instantiate();
  
  gM.layer1_.reset0Position();
  gM.layer2_.reset0Position();
  
  gM.layer1_.update0Position();
  gM.layer2_.update0Position();
  gM.combat_.updateStats();
  
  gM.player_[4].updateSpriteC();
  /*printf("\n\n\n\n");
  
  printf("%d\n",gM.player_[0].char_id_);
  printf("%d\n",gM.player_[0].xp_);
  printf("%d\n",gM.player_[0].level_);
  printf("%d\n\n",gM.player_[0].profession_);
              
  printf("%d\n",gM.player_[0].skin_id_.gender);
  printf("%d\n",gM.player_[0].skin_id_.skin);
  printf("%d\n",gM.player_[0].skin_id_.hair);
  printf("%d\n",gM.player_[0].skin_id_.hair_color);
  printf("%d\n",gM.player_[0].skin_id_.eyes);
  printf("%d\n",gM.player_[0].skin_id_.ears);
  printf("%d\n",gM.player_[0].skin_id_.nose);
  printf("%d\n",gM.player_[0].skin_id_.beard);
  printf("%d\n",gM.player_[0].skin_id_.mustache);
  printf("%d\n",gM.player_[0].skin_id_.mustache_color);
  printf("%d\n",gM.player_[0].skin_id_.torso);
  printf("%d\n",gM.player_[0].skin_id_.cape);
  printf("%d\n",gM.player_[0].skin_id_.legs);
  printf("%d\n",gM.player_[0].skin_id_.head);
  printf("%d\n",gM.player_[0].skin_id_.neck);
  printf("%d\n",gM.player_[0].skin_id_.belt);
  printf("%d\n",gM.player_[0].skin_id_.armor);
  printf("%d\n",gM.player_[0].skin_id_.back);
  printf("%d\n",gM.player_[0].skin_id_.bracelets);
  printf("%d\n",gM.player_[0].skin_id_.bracers);
  printf("%d\n",gM.player_[0].skin_id_.gloves);
  printf("%d\n",gM.player_[0].skin_id_.pants);
  printf("%d\n",gM.player_[0].skin_id_.feet);*/


}

void MainScene::drawImgui(SDL_Renderer* ren){
  
}
