
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

#include "SDL_image.h"

#include "customization.h"
#include "gamemanager.h"

/** @brief Process mouse events for the ImGuiSDL binding.
 *
 * @param Pointer to an SDL_Event captured this frame.
 */
 
void ImGuiSDLProcessEvent(SDL_Event* e) {
  if (e == nullptr) return ;

  ImGuiIO& io = ImGui::GetIO();
  
  GameManager& gM = GameManager::Instantiate();

  int wheel = 0;

  // SDL_StartTextInput();

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
    
    if(e->type == SDL_TEXTINPUT){
      io.AddInputCharactersUTF8(e->text.text);
      
    }
    
  }

  // SDL_StopTextInput();

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

void InitCustomization(){

 //GameManager& gM = GameManager::Instantiate();
  
  for(int i = 0; i < 13; ++i){

    id_[i] = 255;
    if(i < 11){
      //textures_[i] = nullptr;
    }

  }
  


}

/** @brief Loads a new image into the image-array */
void SetImage(SDL_Surface** img, char* dir, unsigned char* id, 
              int32_t skin_id, SDL_Texture** texture, 
              SDL_Renderer* ren){

  if(*img != nullptr){ SDL_FreeSurface(*img); }

  *img = IMG_Load(dir);
  *id = skin_id;


  if(*texture != nullptr){ SDL_DestroyTexture(*texture); }
  *texture = SDL_CreateTextureFromSurface(ren, *img);
  
}

void DrawCharacter(SDL_Renderer* ren, Character c){


  GameManager& gM = GameManager::Instantiate();
  SDL_Rect fsrc_rect = {0,128,64,64};
  SDL_Rect fdest_rect = {310,637,64,64};

  SDL_Rect bsrc_rect = {0,0,64,64};
  SDL_Rect bdest_rect = {490,637,64,64};

  SDL_Rect lsrc_rect = {0,64,64,64};
  SDL_Rect ldest_rect = {666,637,64,64};

  SDL_Rect rsrc_rect = {0,192,64,64};
  SDL_Rect rdest_rect = {840,637,64,64};
  
  // Body Path
  std::string tmp_dir = "../data/skins/body/male/body$.png";

  // Set Gender
  if(c.skin_id_.gender == 2){

    tmp_dir.replace(tmp_dir.find("male"), 4, "female");

  }

  // Set Body  
  tmp_dir.replace(tmp_dir.find("$"), 1, std::to_string(c.skin_id_.skin)); 
  char* skin_dir = (char*)malloc(tmp_dir.length());
  strcpy(skin_dir, tmp_dir.c_str());  
  if(c.skin_id_.skin != id_[1]){
    SetImage(&images_[11 * c.char_id_ + 0], skin_dir, &id_[1], c.skin_id_.skin, &gM.textures_[11 * c.char_id_ + 0]->texture_, ren);    
  }

  if(c.skin_id_.gender != id_[0]){
    SetImage(&images_[11 * c.char_id_ + 0], skin_dir, &id_[1], c.skin_id_.skin, &gM.textures_[11 * c.char_id_ + 0]->texture_, ren);
    id_[0] = c.skin_id_.gender;
  }

  // Set Hair && Color
  std::string tmp_hair_color = "../data/skins/hair/hairs/hair ($)/color (%).png";
  tmp_hair_color.replace(tmp_hair_color.find("$"), 1, std::to_string(c.skin_id_.hair));
  tmp_hair_color.replace(tmp_hair_color.find("%"), 1, std::to_string(c.skin_id_.hair_color));
  char* haircolor_dir = (char*)malloc(tmp_hair_color.length());
  strcpy(haircolor_dir, tmp_hair_color.c_str());
  if(c.skin_id_.hair_color != id_[3]){
    SetImage(&images_[11 * c.char_id_ + 1], haircolor_dir, &id_[3], c.skin_id_.hair_color, &gM.textures_[11 * c.char_id_ + 1]->texture_, ren);
  }
  if(c.skin_id_.hair != id_[2]){
    SetImage(&images_[11 * c.char_id_ + 1], haircolor_dir, &id_[2], c.skin_id_.hair, &gM.textures_[11 * c.char_id_ + 1]->texture_, ren);
  }

  // Set Eyes Color
  std::string tmp_eyes = "../data/skins/body/male/eyes/$.png";
  tmp_eyes.replace(tmp_eyes.find("$"), 1, std::to_string(c.skin_id_.eyes));
  char* eyes_dir = (char*)malloc(tmp_eyes.length());
  strcpy(eyes_dir, tmp_eyes.c_str());
  if(c.skin_id_.eyes != id_[4]){    
    SetImage(&images_[11 * c.char_id_ + 2], eyes_dir, &id_[4], c.skin_id_.eyes, &gM.textures_[11 * c.char_id_ + 2]->texture_, ren);
  } 

  // Set Nose
  std::string tmp_nose = "../data/skins/body/male/nose/$-%.png";
  tmp_nose.replace(tmp_nose.find("$"), 1, std::to_string(c.skin_id_.skin));
  tmp_nose.replace(tmp_nose.find("%"), 1, std::to_string(c.skin_id_.nose));
  char* nose_dir = (char*)malloc(tmp_nose.length());
  strcpy(nose_dir, tmp_nose.c_str());
  if(c.skin_id_.nose != id_[5]){
    SetImage(&images_[11 * c.char_id_ + 3], nose_dir, &id_[5], c.skin_id_.nose, &gM.textures_[11 * c.char_id_ + 3]->texture_, ren);
  } 

  // Set Ears
  std::string tmp_ears = "../data/skins/body/male/ears/$-%.png";
  tmp_ears.replace(tmp_ears.find("$"), 1, std::to_string(c.skin_id_.skin));
  tmp_ears.replace(tmp_ears.find("%"), 1, std::to_string(c.skin_id_.ears));
  char* ears_dir = (char*)malloc(tmp_ears.length());
  strcpy(ears_dir, tmp_ears.c_str());
  if(c.skin_id_.ears != id_[6]){
    SetImage(&images_[11 * c.char_id_ + 4], ears_dir, &id_[6], c.skin_id_.ears, &gM.textures_[11 * c.char_id_ + 4]->texture_, ren);
  } 

  // Set Beard
  std::string tmp_beard = "../data/skins/facial/beard/color ($).png";
  tmp_beard.replace(tmp_beard.find("$"), 1, std::to_string(c.skin_id_.beard));
  char* beard_dir = (char*)malloc(tmp_beard.length());
  strcpy(beard_dir, tmp_beard.c_str());
  if(c.skin_id_.beard != id_[7]){
    SetImage(&images_[11 * c.char_id_ + 5], beard_dir, &id_[7], c.skin_id_.beard, &gM.textures_[11 * c.char_id_ + 5]->texture_, ren);
  }

  // Set Mustache
  std::string tmp_mustache = "../data/skins/facial/mustache ($).png";
  tmp_mustache.replace(tmp_mustache.find("$"), 1, std::to_string(c.skin_id_.mustache));
  char* mustache_dir = (char*)malloc(tmp_mustache.length());
  strcpy(mustache_dir, tmp_mustache.c_str());
  if(c.skin_id_.mustache != id_[8]){
    SetImage(&images_[11 * c.char_id_ + 6], mustache_dir, &id_[8], c.skin_id_.mustache, &gM.textures_[11 * c.char_id_ + 6]->texture_, ren);
  }

  // Set Mustache Color
  std::string tmp_mustachecolor = "../data/skins/facial/mustaches/mustache ($)/color (%).png";
  tmp_mustachecolor.replace(tmp_mustachecolor.find("$"), 1, std::to_string(c.skin_id_.mustache));
  tmp_mustachecolor.replace(tmp_mustachecolor.find("%"), 1, std::to_string(c.skin_id_.mustache_color));  
  char* mcolor_dir = (char*)malloc(tmp_mustachecolor.length());
  strcpy(mcolor_dir, tmp_mustachecolor.c_str());
  if(c.skin_id_.mustache_color != id_[9]){
    SetImage(&images_[11 * c.char_id_ + 7], mcolor_dir, &id_[9], c.skin_id_.mustache_color, &gM.textures_[11 * c.char_id_ + 7]->texture_, ren);
  }

  // Set Torso
  std::string tmp_torso = "../data/skins/torso/clothes/piece ($).png";
  tmp_torso.replace(tmp_torso.find("$"), 1, std::to_string(c.skin_id_.torso));
  char* torso_dir = (char*)malloc(tmp_torso.length());
  strcpy(torso_dir, tmp_torso.c_str());
  if(c.skin_id_.torso != id_[10]){
    SetImage(&images_[11 * c.char_id_ + 8], torso_dir, &id_[10], c.skin_id_.torso, &gM.textures_[11 * c.char_id_ + 8]->texture_, ren);
  }

  // Set Legs
  std::string tmp_legs = "../data/skins/legs/clothes/piece ($).png";
  tmp_legs.replace(tmp_legs.find("$"), 1, std::to_string(c.skin_id_.legs));
  // std::cout << tmp_legs << "\n";
  char* legs_dir = (char*)malloc(tmp_legs.length());
  strcpy(legs_dir, tmp_legs.c_str());
  if(c.skin_id_.legs != id_[11]){
    SetImage(&images_[11 * c.char_id_ + 9], legs_dir, &id_[11], c.skin_id_.legs, &gM.textures_[11 * c.char_id_ + 9]->texture_, ren);
  }

  // Set Cape
  std::string tmp_cape = "../data/skins/torso/back/capes/piece ($).png";
  tmp_cape.replace(tmp_cape.find("$"), 1, std::to_string(c.skin_id_.cape));
  char* cape_dir = (char*)malloc(tmp_cape.length());
  strcpy(cape_dir, tmp_cape.c_str());
  if(c.skin_id_.cape != id_[12]){
    SetImage(&images_[11 * c.char_id_ + 10], cape_dir, &id_[12], c.skin_id_.cape, &gM.textures_[11 * c.char_id_ + 10]->texture_, ren);
  }
  
  // Four Views
  for(int i = 0; i < 11; ++i){
    SDL_RenderCopy(ren, gM.textures_[11 * c.char_id_ + i]->texture_, &fsrc_rect, &fdest_rect);
    SDL_RenderCopy(ren, gM.textures_[11 * c.char_id_ + i]->texture_, &bsrc_rect, &bdest_rect);
    SDL_RenderCopy(ren, gM.textures_[11 * c.char_id_ + i]->texture_, &lsrc_rect, &ldest_rect);
    SDL_RenderCopy(ren, gM.textures_[11 * c.char_id_ + i]->texture_, &rsrc_rect, &rdest_rect);
  }
}

void CustomizeCharacter(Character *c){

  GameManager& gM = GameManager::Instantiate();
  char* button_text = "Next Character"; 
  
  const char* profession[] = {"Warrior", "Archer", "Sorceress", "Berserker", "Valkyrie", "Dark Knight",
                              "Healer", "Guardian", "Hashashin", "Mage"};

  // Start the Dear ImGui frame
  ImGui::NewFrame();
  // ImGui::SetNextWindowPos(ImVec2(gM.kWindowWidth/2, gM.kWindowHeight/2), 0, ImVec2(0.5, 0.5));
  ImGui::SetNextWindowSize(ImVec2(gM.kImGuiWidth/2, 320));
  ImGui::SetNextWindowPos(ImVec2(58, 51));

  ImGui::Begin("Create Character", NULL, gM.window_flags);
  ImGui::Indent(25.0f);
  ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Character Name");
  
  ImGui::InputText("", c->name_, 128);
  ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Choose Class");
  ImGui::ListBox("", &c->profession_, profession, IM_ARRAYSIZE(profession), 10);  
  ImGui::End();

  /** @brief Stats box */
  if(0 <= c->profession_ && c->profession_ < 10){
    ImGui::SetNextWindowPos(ImVec2(58, 380));
    ImGui::SetNextWindowSize(ImVec2(gM.kImGuiWidth/2, 170));

    ImGui::Begin("Stats", NULL, gM.window_flags);
    ImGui::BeginColumns("Attributes", 4, ImGuiColumnsFlags_NoBorder | ImGuiColumnsFlags_NoResize);
    ImGui::SetColumnWidth(0, 50.0f);
    ImGui::SetColumnWidth(1, 60.0f);
    ImGui::SetColumnWidth(2, 90.0f);

    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f),"HP");
    ImGui::Dummy(ImVec2(0.0f, 5.0f));
    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f),"Mana");
    ImGui::Dummy(ImVec2(0.0f, 5.0f));
    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f),"AD");
    ImGui::Dummy(ImVec2(0.0f, 5.0f));
    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f),"AP");

    ImGui::NextColumn();
    char aux_text[50];
    sprintf (aux_text, "%d", 
           gM.data_base_.p_[c->profession_]->hp);
    ImGui::Text(aux_text);      
    ImGui::Dummy(ImVec2(0.0f, 5.0f));
    sprintf (aux_text, "%d", 
           gM.data_base_.p_[c->profession_]->mana);
    ImGui::Text(aux_text);      
    ImGui::Dummy(ImVec2(0.0f, 5.0f));
    sprintf (aux_text, "%d", 
           gM.data_base_.p_[c->profession_]->physical_att);
    ImGui::Text(aux_text);      
    ImGui::Dummy(ImVec2(0.0f, 5.0f));
    sprintf (aux_text, "%d", 
           gM.data_base_.p_[c->profession_]->magic_att);
    ImGui::Text(aux_text);      
    
    

    ImGui::NextColumn();

    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Armor");
    ImGui::Dummy(ImVec2(0.0f, 5.0f));
    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Magig Res");
    ImGui::Dummy(ImVec2(0.0f, 5.0f));
    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Movement");
    ImGui::Dummy(ImVec2(0.0f, 5.0f));
    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Mana Regen");  
    ImGui::Dummy(ImVec2(0.0f, 5.0f));
    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Crit. Chance");

    ImGui::NextColumn();
    
      sprintf (aux_text, "%d", 
           gM.data_base_.p_[c->profession_]->armor);
    ImGui::Text(aux_text);      
    ImGui::Dummy(ImVec2(0.0f, 5.0f));
    sprintf (aux_text, "%d", 
           gM.data_base_.p_[c->profession_]->magic_resist);
    ImGui::Text(aux_text);      
    ImGui::Dummy(ImVec2(0.0f, 5.0f));
    sprintf (aux_text, "%d", 
           gM.data_base_.p_[c->profession_]->movement);
    ImGui::Text(aux_text);      
    ImGui::Dummy(ImVec2(0.0f, 5.0f));
    sprintf (aux_text, "%d", 
           gM.data_base_.p_[c->profession_]->mana_regen);
    ImGui::Text(aux_text);      
    ImGui::Dummy(ImVec2(0.0f, 5.0f)); 
    sprintf (aux_text, "%d", 
           gM.data_base_.p_[c->profession_]->crit_chance);
    ImGui::Text(aux_text);      
   
    
    ImGui::EndColumns();
    ImGui::End();
  }

  ImGui::SetNextWindowSize(ImVec2(gM.kImGuiWidth, gM.kImGuiHeight));
  ImGui::Begin("Antekeland", NULL, gM.window_flags);

  // Gender
  ImGui::BeginColumns("Character Creation", 2, ImGuiColumnsFlags_NoResize);  
  ImGui::Indent(25.0f);
  // ImGui::Columns(2, NULL, true);
  ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Select Gender");  
  ImGui::Text("\n");
  ImGui::SameLine(20);
  ImGui::RadioButton("Male", &c->skin_id_.gender, 1); ImGui::SameLine();
  ImGui::RadioButton("Female", &c->skin_id_.gender, 2);
  ImGui::Text("\n");

  // Skins
  ImGui::TextColored(ImVec4(0.79f, 0.63f, 0.96f, 1.0f), "Select Skin");
  if (ImGui::ArrowButton("##leftskin", ImGuiDir_Left)) {
    --c->skin_id_.skin;
    if(c->skin_id_.skin <= 0){ c->skin_id_.skin = 7; c->skin_id_.skin%=8; }
  }
  ImGui::SameLine();
  ImGui::Text("Skin %d", c->skin_id_.skin);
  ImGui::SameLine();
  if(ImGui::ArrowButton("##rightskin", ImGuiDir_Right)) { c->skin_id_.skin%=7; ++c->skin_id_.skin; }
  ImGui::Text("\n");

  // Hair
  ImGui::TextColored(ImVec4(0.79f, 0.63f, 0.96f, 1.0f), "Select Hair");
  if (ImGui::ArrowButton("##lefthair", ImGuiDir_Left)) {
    --c->skin_id_.hair;
    if(c->skin_id_.hair < 0){ c->skin_id_.hair = 30; c->skin_id_.hair%=31; }
  }
  ImGui::SameLine();
  ImGui::Text("Hair %d", c->skin_id_.hair);
  ImGui::SameLine();
  if(ImGui::ArrowButton("##righthair", ImGuiDir_Right)) { ++c->skin_id_.hair%=31; }
  ImGui::Text("\n");

  // Hair Color
  ImGui::TextColored(ImVec4(0.79f, 0.63f, 0.96f, 1.0f), "Select Hair Color");
  if (ImGui::ArrowButton("##leftcolor", ImGuiDir_Left)) {
    --c->skin_id_.hair_color;
    if(c->skin_id_.hair_color <= 0){ c->skin_id_.hair_color = 29; c->skin_id_.hair_color%=30; }
  }
  ImGui::SameLine();
  ImGui::Text("Hair Color %d", c->skin_id_.hair_color);
  ImGui::SameLine();
  if(ImGui::ArrowButton("##rightcolor", ImGuiDir_Right)) { c->skin_id_.hair_color%=29; ++c->skin_id_.hair_color; }
  ImGui::Text("\n");

  // Ears Type
  ImGui::TextColored(ImVec4(0.79f, 0.63f, 0.96f, 1.0f), "Select Ears");
  if (ImGui::ArrowButton("##leftear", ImGuiDir_Left)) {
    --c->skin_id_.ears;
    if(c->skin_id_.ears < 0){ c->skin_id_.ears = 2; c->skin_id_.ears%=3; }
  }
  ImGui::SameLine();
  ImGui::Text("Ears %d", c->skin_id_.ears);
  ImGui::SameLine();
  if(ImGui::ArrowButton("##rightear", ImGuiDir_Right)) { ++c->skin_id_.ears%=3; }
  ImGui::Text("\n");

  // Nose Type
  ImGui::TextColored(ImVec4(0.79f, 0.63f, 0.96f, 1.0f), "Select Nose");
  if (ImGui::ArrowButton("##leftnose", ImGuiDir_Left)) {
    --c->skin_id_.nose;
    if(c->skin_id_.nose < 0){ c->skin_id_.nose = 3; c->skin_id_.nose%=4; }
  }
  ImGui::SameLine();
  ImGui::Text("Nose %d", c->skin_id_.nose);
  ImGui::SameLine();
  if(ImGui::ArrowButton("##rightnose", ImGuiDir_Right)) { ++c->skin_id_.nose%=4; }
  ImGui::Text("\n");

  // Eyes Color
  ImGui::TextColored(ImVec4(0.79f, 0.63f, 0.96f, 1.0f), "Select Eyes Color");
  if (ImGui::ArrowButton("##lefteye", ImGuiDir_Left)) {
    --c->skin_id_.eyes;
    if(c->skin_id_.eyes < 0){ c->skin_id_.eyes = 8; c->skin_id_.eyes%=9; }
  }
  ImGui::SameLine();
  ImGui::Text("Eyes Color %d", c->skin_id_.eyes);
  ImGui::SameLine();
  if(ImGui::ArrowButton("##righteye", ImGuiDir_Right)) { ++c->skin_id_.eyes%=9; }

  ImGui::NextColumn();
  ImGui::Indent(35.0f);
  // Beard
  ImGui::TextColored(ImVec4(0.79f, 0.63f, 0.96f, 1.0f), "Select Beard");
  if (ImGui::ArrowButton("##leftbeard", ImGuiDir_Left)) {
    --c->skin_id_.beard;
    if(c->skin_id_.beard < 0){ c->skin_id_.beard = 29; c->skin_id_.beard%=30; }
  }
  ImGui::SameLine();
  ImGui::Text("Beard %d", c->skin_id_.beard);
  ImGui::SameLine();
  if(ImGui::ArrowButton("##rightbeard", ImGuiDir_Right)) { ++c->skin_id_.beard%=30; }
  ImGui::Text("\n");

  // Mustache
  ImGui::TextColored(ImVec4(0.79f, 0.63f, 0.96f, 1.0f), "Select Mustache");
  if (ImGui::ArrowButton("##leftmustache", ImGuiDir_Left)) {
    --c->skin_id_.mustache;
    if(c->skin_id_.mustache < 0){ c->skin_id_.mustache = 4; c->skin_id_.mustache%=5; }
  }
  ImGui::SameLine();
  ImGui::Text("Mustache %d", c->skin_id_.mustache);
  ImGui::SameLine();
  if(ImGui::ArrowButton("##rightmustache", ImGuiDir_Right)) { ++c->skin_id_.mustache%=5; }
  ImGui::Text("\n");

  // Mustache Color
  ImGui::TextColored(ImVec4(0.79f, 0.63f, 0.96f, 1.0f), "Select Mustache Color");
  if (ImGui::ArrowButton("##leftmustachecolor", ImGuiDir_Left)) {
    --c->skin_id_.mustache_color;
    if(c->skin_id_.mustache_color < 0){ c->skin_id_.mustache_color = 29; c->skin_id_.mustache_color%=30; }
  }
  ImGui::SameLine();
  ImGui::Text("Mustache %d", c->skin_id_.mustache_color);
  ImGui::SameLine();
  if(ImGui::ArrowButton("##rightmustachecolor", ImGuiDir_Right)) { ++c->skin_id_.mustache_color%=30; }
  ImGui::Text("\n");

  // Torso
  ImGui::TextColored(ImVec4(0.79f, 0.63f, 0.96f, 1.0f), "Select Top Clothes");
  if (ImGui::ArrowButton("##lefttorso", ImGuiDir_Left)) {
    --c->skin_id_.torso;
    if(c->skin_id_.torso < 0){ c->skin_id_.torso = 38; c->skin_id_.torso%=39; }
  }
  ImGui::SameLine();
  ImGui::Text("Torso %d", c->skin_id_.torso);
  ImGui::SameLine();
  if(ImGui::ArrowButton("##righttorso", ImGuiDir_Right)) { ++c->skin_id_.torso%=39; }
  ImGui::Text("\n");

  // Cape
  ImGui::TextColored(ImVec4(0.79f, 0.63f, 0.96f, 1.0f), "Select Cape");
  if (ImGui::ArrowButton("##leftcape", ImGuiDir_Left)) {
    --c->skin_id_.cape;
    if(c->skin_id_.cape < 0){ c->skin_id_.cape = 23; c->skin_id_.cape%=24; }
  }
  ImGui::SameLine();
  ImGui::Text("Cape %d", c->skin_id_.cape);
  ImGui::SameLine();
  if(ImGui::ArrowButton("##rightcape", ImGuiDir_Right)) { ++c->skin_id_.cape%=24; }
  ImGui::Text("\n");

  // Legs
  ImGui::TextColored(ImVec4(0.79f, 0.63f, 0.96f, 1.0f), "Select Pants");
  if (ImGui::ArrowButton("##leftleg", ImGuiDir_Left)) {
    --c->skin_id_.legs;
    if(c->skin_id_.legs < 0){ c->skin_id_.legs = 10; c->skin_id_.legs%=11; }
  }
  ImGui::SameLine();
  ImGui::Text("Pants %d", c->skin_id_.legs);
  ImGui::SameLine();
  if(ImGui::ArrowButton("##rightleg", ImGuiDir_Right)) { ++c->skin_id_.legs%=11; }
  ImGui::Text("\n");

  // Randomize
  if(ImGui::Button("Randomize Character")){

    c->skin_id_.gender = 1+(rand()%2);
    c->skin_id_.skin = 1+ (rand()%7);
    c->skin_id_.hair = rand()%31;
    c->skin_id_.hair_color = rand()%30;
    c->skin_id_.ears = rand()%3;
    c->skin_id_.nose = rand()%4;
    c->skin_id_.eyes = rand()%9;
    c->skin_id_.beard = rand()%30;
    c->skin_id_.mustache = rand()%5;
    c->skin_id_.mustache_color = 1+(rand()%29);
    c->skin_id_.torso = rand()%39;
    c->skin_id_.cape = rand()%24;
    c->skin_id_.legs = rand()%11;

  } 
    
  if(gM.current_edit_ == 3){
    button_text = "Continue";
    ImGui::Indent(40.0f);
  }else{
    ImGui::Indent(20.0f);
  }
  if(ImGui::Button(button_text)){

    SDL_Rect tmp_rect = {0, 0, 64, 64};    
   
    for(int i = 0; i < 6; ++i){
       c->skin_[i].initSprite(*(gM.textures_[11 * c->char_id_ + i]), &tmp_rect, &tmp_rect);
    }
    for(int i = 0; i<13; ++i){
      
      printf("N:%d v:%d\n",i,id_[i]);
    }
    
    //printf("\n\n");
    
    gM.player_[gM.current_edit_].skin_id_.gender = id_[0];
    gM.player_[gM.current_edit_].skin_id_.skin = id_[1];
    gM.player_[gM.current_edit_].skin_id_.hair = id_[2];
    gM.player_[gM.current_edit_].skin_id_.hair_color = id_[3];
    gM.player_[gM.current_edit_].skin_id_.eyes = id_[4];
    gM.player_[gM.current_edit_].skin_id_.ears = id_[5];
    gM.player_[gM.current_edit_].skin_id_.nose = id_[6];
    gM.player_[gM.current_edit_].skin_id_.beard = id_[7];
    gM.player_[gM.current_edit_].skin_id_.mustache = id_[8];
    gM.player_[gM.current_edit_].skin_id_.mustache_color = id_[9];
    gM.player_[gM.current_edit_].skin_id_.torso = id_[10];
    gM.player_[gM.current_edit_].skin_id_.cape = id_[11];
    gM.player_[gM.current_edit_].skin_id_.legs = id_[12];/*
    c->skin_id_.gender = id_[0];
    c->skin_id_.skin = id_[1];
    c->skin_id_.hair = id_[2];
    c->skin_id_.hair_color = id_[3];
    c->skin_id_.eyes = id_[4];
    c->skin_id_.ears = id_[5];
    c->skin_id_.nose = id_[6];
    c->skin_id_.beard = id_[7];
    c->skin_id_.mustache = id_[8];
    c->skin_id_.mustache_color = id_[9];
    c->skin_id_.torso = id_[10];
    c->skin_id_.cape = id_[11];
    c->skin_id_.legs = id_[12];*/
    
    // Gender + Skin
    // c->skin_[0].initSprite(textures_[11 * c->char_id_ + 0], tmp_rect, tmp_rect);
    // Hair + Color
    // c->skin_[1].initSprite(textures_[11 * c->char_id_ + 1], tmp_rect, tmp_rect);
    // Eyes
    // c->skin_[2].initSprite(textures_[11 * c->char_id_ + 2], tmp_rect, tmp_rect);
    // Nose
    // c->skin_[3].initSprite(textures_[11 * c->char_id_ + 3], tmp_rect, tmp_rect);
    // Ears
    // c->skin_[4].initSprite(textures_[11 * c->char_id_ + 4], tmp_rect, tmp_rect);
    // Beard
    // c->skin_[5].initSprite(textures_[11 * c->char_id_ + 5], tmp_rect, tmp_rect);
    // Mustache + Color
    c->skin_[6].initSprite(*gM.textures_[11 * c->char_id_ + 7], &tmp_rect, &tmp_rect);

    // Torso
    c->outfit_[0].initSprite(*gM.textures_[11 * c->char_id_ + 8], &tmp_rect, &tmp_rect);
    // Legs
    c->outfit_[1].initSprite(*gM.textures_[11 * c->char_id_ + 9], &tmp_rect, &tmp_rect);
    // Cape
    c->outfit_[2].initSprite(*gM.textures_[11 * c->char_id_ + 10], &tmp_rect, &tmp_rect);
    printf("%d",gM.current_edit_);
    c->init(c->profession_,gM.current_edit_); 

    ++gM.current_edit_;
    InitCustomization();
  }

  ImGui::EndColumns();  

  ImGui::End();

  ImGui::SetNextWindowSize(ImVec2(150, 150));
  ImGui::Begin("Front View", NULL, gM.window_flags);
  ImGui::End();

  ImGui::SetNextWindowSize(ImVec2(150, 150));
  ImGui::Begin("Back View", NULL, gM.window_flags);
  ImGui::End();

  ImGui::SetNextWindowSize(ImVec2(150, 150));
  ImGui::Begin("Left Side View", NULL, gM.window_flags);
  ImGui::End();

  ImGui::SetNextWindowSize(ImVec2(150, 150));
  ImGui::Begin("Right Side View", NULL, gM.window_flags);
  ImGui::End();

}

void DrawCustomization(){

  //ImGui
  ImGui::Render();  
  ImGuiSDL::Render(ImGui::GetDrawData());

}



void LoadCharacter(SDL_Renderer* ren){
  
  GameManager& gM = GameManager::Instantiate();
  SDL_Rect tmp_rect = {0, 0, 64, 64}; 
  // Body Path

  for(int i = 0; i<4; ++i){
    std::string tmp_dir = "../data/skins/body/male/body$.png";
    if(gM.player_[i].skin_id_.gender == 2){
      tmp_dir.replace(tmp_dir.find("male"), 4, "female");
    }
    
    // Set Body  
    tmp_dir.replace(tmp_dir.find("$"), 1, std::to_string(gM.player_[i].skin_id_.skin)); 
    char* skin_dir = (char*)malloc(tmp_dir.length());
    strcpy(skin_dir, tmp_dir.c_str());  
    gM.textures_[11 * gM.player_[i].char_id_ + 0]->loadTexture(skin_dir,ren);
    
   
    
    std::string tmp_hair_color = "../data/skins/hair/hairs/hair ($)/color (%).png";

    tmp_hair_color.replace(tmp_hair_color.find("$"), 1, std::to_string(gM.player_[i].skin_id_.hair));
    tmp_hair_color.replace(tmp_hair_color.find("%"), 1, std::to_string(gM.player_[i].skin_id_.hair_color));
    char* haircolor_dir = (char*)malloc(tmp_hair_color.length());
    strcpy(haircolor_dir, tmp_hair_color.c_str());
    gM.textures_[11 * gM.player_[i].char_id_ + 1]->loadTexture(haircolor_dir,ren);
  
    std::string tmp_eyes = "../data/skins/body/male/eyes/$.png";
    
    tmp_eyes.replace(tmp_eyes.find("$"), 1, std::to_string(gM.player_[i].skin_id_.eyes));
    char* eyes_dir = (char*)malloc(tmp_eyes.length());
    strcpy(eyes_dir, tmp_eyes.c_str());
    gM.textures_[11 * gM.player_[i].char_id_ + 2]->loadTexture(eyes_dir,ren);
    
    std::string tmp_nose = "../data/skins/body/male/nose/$-%.png";
    tmp_nose.replace(tmp_nose.find("$"), 1, std::to_string(gM.player_[i].skin_id_.skin));
    tmp_nose.replace(tmp_nose.find("%"), 1, std::to_string(gM.player_[i].skin_id_.nose));
    char* nose_dir = (char*)malloc(tmp_nose.length());
    strcpy(nose_dir, tmp_nose.c_str());
    
    gM.textures_[11 * gM.player_[i].char_id_ + 3]->loadTexture(nose_dir,ren);
      
    
    std::string tmp_ears = "../data/skins/body/male/ears/$-%.png";
    tmp_ears.replace(tmp_ears.find("$"), 1, std::to_string(gM.player_[i].skin_id_.skin));
    tmp_ears.replace(tmp_ears.find("%"), 1, std::to_string(gM.player_[i].skin_id_.ears));
    char* ears_dir = (char*)malloc(tmp_ears.length());
    strcpy(ears_dir, tmp_ears.c_str());
    
    gM.textures_[11 * gM.player_[i].char_id_ + 4]->loadTexture(ears_dir,ren);
    
    // Set Beard
    std::string tmp_beard = "../data/skins/facial/beard/color ($).png";
    tmp_beard.replace(tmp_beard.find("$"), 1, std::to_string(gM.player_[i].skin_id_.beard));
    char* beard_dir = (char*)malloc(tmp_beard.length());
    strcpy(beard_dir, tmp_beard.c_str());
    
    gM.textures_[11 * gM.player_[i].char_id_ + 5]->loadTexture(beard_dir,ren);
    
    
    // Set Mustache
    std::string tmp_mustache = "../data/skins/facial/mustache ($).png";
    tmp_mustache.replace(tmp_mustache.find("$"), 1, std::to_string(gM.player_[i].skin_id_.mustache));
    char* mustache_dir = (char*)malloc(tmp_mustache.length());
    strcpy(mustache_dir, tmp_mustache.c_str());
    
    gM.textures_[11 * gM.player_[i].char_id_ + 6]->loadTexture(mustache_dir,ren);
    
    
    
    // Set Mustache Color
    std::string tmp_mustachecolor = "../data/skins/facial/mustaches/mustache ($)/color (%).png";
    tmp_mustachecolor.replace(tmp_mustachecolor.find("$"), 1, std::to_string(gM.player_[i].skin_id_.mustache));
    tmp_mustachecolor.replace(tmp_mustachecolor.find("%"), 1, std::to_string(gM.player_[i].skin_id_.mustache_color));  
    char* mcolor_dir = (char*)malloc(tmp_mustachecolor.length());
    strcpy(mcolor_dir, tmp_mustachecolor.c_str());
    
    gM.textures_[11 * gM.player_[i].char_id_ + 7]->loadTexture(mcolor_dir,ren);
    
    // Set Torso
    std::string tmp_torso = "../data/skins/torso/clothes/piece ($).png";
    tmp_torso.replace(tmp_torso.find("$"), 1, std::to_string(gM.player_[i].skin_id_.torso));
    char* torso_dir = (char*)malloc(tmp_torso.length());
    strcpy(torso_dir, tmp_torso.c_str());
    
    gM.textures_[11 * gM.player_[i].char_id_ + 8]->loadTexture(torso_dir,ren);
    
    // Set Legs
    std::string tmp_legs = "../data/skins/legs/clothes/piece ($).png";
    tmp_legs.replace(tmp_legs.find("$"), 1, std::to_string(gM.player_[i].skin_id_.legs));
    // std::cout << tmp_legs << "\n";
    char* legs_dir = (char*)malloc(tmp_legs.length());
    strcpy(legs_dir, tmp_legs.c_str());
    
    gM.textures_[11 * gM.player_[i].char_id_ + 9]->loadTexture(legs_dir,ren);
    
    // Set Cape
    std::string tmp_cape = "../data/skins/torso/back/capes/piece ($).png";
    tmp_cape.replace(tmp_cape.find("$"), 1, std::to_string(gM.player_[i].skin_id_.cape));
    char* cape_dir = (char*)malloc(tmp_cape.length());
    strcpy(cape_dir, tmp_cape.c_str());
    
    gM.textures_[11 * gM.player_[i].char_id_ + 10]->loadTexture(cape_dir,ren);
    

    for(int j = 0; j < 6; ++j){
      gM.player_[i].skin_[j].initSprite(*(gM.textures_[11 * gM.player_[i].char_id_ + j]), &tmp_rect, &tmp_rect);
    }
    
    // Mustache + Color
    gM.player_[i].skin_[6].initSprite(*gM.textures_[11 * gM.player_[i].char_id_ + 7], &tmp_rect, &tmp_rect);

    // Torso
    gM.player_[i].outfit_[0].initSprite(*gM.textures_[11 * gM.player_[i].char_id_ + 8], &tmp_rect, &tmp_rect);
    // Legs
    gM.player_[i].outfit_[1].initSprite(*gM.textures_[11 * gM.player_[i].char_id_ + 9], &tmp_rect, &tmp_rect);
    // Cape
    gM.player_[i].outfit_[2].initSprite(*gM.textures_[11 * gM.player_[i].char_id_ + 10], &tmp_rect, &tmp_rect);
  
    gM.player_[i].init(gM.player_[i].profession_,i);
    
    for(int l = 1; l<gM.player_[i].level_ ; ++l){
      gM.player_[i].levelUp();
    }
  }

}

void QuitCustomization(){

  ImGuiSDL::Deinitialize();
  ImGui::DestroyContext();

}
