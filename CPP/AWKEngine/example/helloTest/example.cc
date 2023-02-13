#include <main.h>
#include <engine.h>
#include "minitrace.h"
#include <displaylist.h>
#include <material.h>
#include <gtx/transform.hpp>
#include <ecs.h>

int AWK::awakenedMain(int argc, char** argv) {

  AWK::Engine engine;
  engine.init(argc, argv);




  engine.startRenderLoop([&]() {

    MTR_BEGIN("main", "Logic");

    engine.update();

    MTR_END("main", "Logic");

  });

  return 0;
}