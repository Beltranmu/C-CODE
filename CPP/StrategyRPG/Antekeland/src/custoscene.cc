#include "custoscene.h"
#include "gamemanager.h"
#include "customization.h"
CustoScene::CustoScene(){


}

CustoScene::~CustoScene(){
  
}

void CustoScene::init(){
  GameManager& gM = GameManager::Instantiate();
  
  //BG 
  RBM::Transform2 aux_tr = {{0.0f,0.0f},0.0f,{0.0f,0.0f}};
  RBM::Vec2 aux_v2 = {0.5f,0.0f};
  gM.bg_custo_.init(aux_tr,1,0,*gM.bg_texture_, &aux_v2);
  ent_list.push_back(&gM.bg_custo_);
  
  InitCustomization();
  
}

void CustoScene::input(SDL_Event* eve){}

void CustoScene::update(){
  
  GameManager& gM = GameManager::Instantiate();
  gM.bg_custo_.update(1);
  
  CustomizeCharacter(&gM.player_[gM.current_edit_]);
  
}

void CustoScene::drawImgui(SDL_Renderer* ren){
  GameManager& gM = GameManager::Instantiate();
  DrawCustomization();
  if(gM.current_edit_<4){
    DrawCharacter(ren, gM.player_[gM.current_edit_]);
  }
}