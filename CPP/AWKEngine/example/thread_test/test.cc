

#include <stdio.h>
#include <window.h>
#include <engine.h>

#include "main.h"
#include <displaylist.h>
#include <gtx/transform.hpp>
#include "glm.hpp"

AWK::Engine engine;


int AWK::awakenedMain(int argc, char **argv) {

  glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
  glm::mat4 trans = glm::mat4(1.0f);
  trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));

  vec = trans * vec;
  printf("Vec(%f,%f,%f)", vec.x, vec.y, vec.z);



  return 0;
}


