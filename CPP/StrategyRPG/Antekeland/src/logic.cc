/**
 * Antekeland 2020
 * Author: Javier Benito Abolafio
 * Mail: benitoab@esat-alumni.com
 * University Development @ESAT
 */

#include "logic.h"

Logic::Logic(){

  enabled_ = 1; // 0 = Not walkable, 1 = walkable
  enter_ = 0; // 0 = Not entereable, 1 = allows to enter

}

Logic::~Logic(){
}

void Logic::init(){

  enabled_ = 1; // 0 = Not walkable, 1 = walkable
  enter_ = 0; // 0 = Not entereable, 1 = Cave, 2 = Door, 3 = Chest

}