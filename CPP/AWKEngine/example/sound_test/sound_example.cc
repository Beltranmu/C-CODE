/*==============================================================================
Crossfading, Layering and Branching Techniques Development
Copyright (c), Javier Benito Abolafio & Ricardo Beltrán Muriel, 2021.
==============================================================================*/
#include <fmod.hpp>
#include <common.h>
#include <main.h>
#include <sound_manager.h>

#include "GLFW/glfw3.h"

int AWK::awakenedMain(int argc, char **argv){

  
/*
* S -> DO
* E -> DO S / RE B
* D -> RE
* R -> RE S / MI B
* F -> MI
* ------
* G -> FA
* Y -> FA S / SOL B
* H -> SOL
* U -> SOL S/ LA B
* J -> LA
* I -> LA S/ SI B
* K -> SI
*/
  AWK::SoundManager SM;

  SM.init();
  SM.loadSound("../../data/sounds/andalucia.wav");
  SM.loadSound("../../data/sounds/op.wav");

  SM.loadSound("../../data/sounds/LAYER_A_0.ogg");
  SM.loadSound("../../data/sounds/LAYER_A_1.ogg");
  SM.loadSound("../../data/sounds/LAYER_A_2.ogg");

  SM.initBranching(5);
  int sound_id[3] = { 2,3,4 };
  SoundManager::Layering layer_1;
  SM.play();
  glfwInit();
  printf("Mode : Crossfading\n");
  printf("Keys : Q\n");

  printf("Change Mode: 1-2-3-4\n");
  printf("1 -> Crossfading\n");
  printf("2 -> Layering\n");
  printf("3 -> Branching\n");
  printf("4 -> Piano\n");

  SM.mode = 1;
  while(1){

    Common_Update();
    SM.musicInput();
    SM.update(glfwGetTime());

  }

  SM.freeSM();

  return 0;
}