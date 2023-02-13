

#include <stdio.h>

#include <engine.h>

#include "main.h"
#include <displayList.h>
#include "minitrace.h"
#include <material.h>
#include <gtx/transform.hpp>





int AWK::awakenedMain(int argc, char* argv[]){
  AWK::Engine engine;
  engine.init(argc, argv);

  //Init Triangle
  

  
  /*-------SCHED----------*/
  engine.startRenderLoop([&](){

    MTR_BEGIN("main", "logic");


   
 
    
    MTR_END("main", "logic");

  });

  return 0;
}

