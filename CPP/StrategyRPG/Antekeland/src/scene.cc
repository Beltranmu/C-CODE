#include "scene.h"

#include "entity.h"

Scene::Scene(){
  //ent_list = nullptr;
}

Scene::~Scene(){
  
 // if(nullptr != ent_list){
    ent_list.clear();
    //ent_list = nullptr;
  //}
}

void Scene::draw(SDL_Renderer* ren){
  
  for each(Entity* aux_ent in ent_list){
    aux_ent->draw(ren);
  }
  
}
void Scene::quit(){
    ent_list.clear();  
}