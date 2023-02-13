#include "agent.h"
#include <stdio.h>
#include <stdlib.h>
#include <board.h>

  Agent::Agent(){

    currentForwardX = 1;
    currentForwardY = 0;

    currentPos = 0;
    i_movement = 0;
    i_counterMov = 0;
    next_mov = 0;
    index_movementArray = 0;
    currentTarget = 0;
    speed = 1;
    been_chased = false;

    agentSelected = false;

    
  }

  Agent::~Agent(){

  }

  void Agent::init(){

    for(int i = 0; i < numMovements ; ++i){
      movementArray[i] = kPatternMovement_None;
      movementCounterArray[i] = 0;
    }
    //Include at least different patterns
    addMovement2Patern(Agent::PatternMovement::kPatternMovement_Forward, 5);
    addMovement2Patern(Agent::PatternMovement::kPatternMovement_TurnLeft90, 1);
    addMovement2Patern(Agent::PatternMovement::kPatternMovement_Wait, 1);
    movementType = kMovement_Pattern;
    i_movement = 0;
    distanceToCatch = 8;
  }



  int Agent::randomMov(bool* canIMove)
  {
    *canIMove = true;
    return rand() % 4; 
  }

  int Agent::patternMov(bool *canIMove){
    //Should I move?

    if (movementArray[i_movement] == kPatternMovement_None) {
      i_movement = 0;
    }
      
    switch (movementArray[i_movement])
    {
      case kPatternMovement_Forward:
        *canIMove = true;  break;

      case kPatternMovement_Turn180:
        currentForwardX *= -1;
        currentForwardY *= -1;
      break;

      case kPatternMovement_TurnLeft90:
        if(currentForwardY == 0){
          currentForwardY = -1 * currentForwardX;
          currentForwardX = 0;
        }else{
          currentForwardX = currentForwardY;
          currentForwardY = 0;
        }
      break;

      case kPatternMovement_TurnRight90:
        if (currentForwardY == 0) {
          currentForwardY = currentForwardX;
          currentForwardX = 0;
        }
        else {
          currentForwardX = -1 * currentForwardY;
          currentForwardY = 0;
        }
      break;
    }
    
    
    ++i_counterMov;
    if (i_counterMov >= movementCounterArray[i_movement]) {
      i_movement++;
      i_movement &= 63;
      i_counterMov = 0;
    }


    //Translate the movement
    next_mov = 0;
    if (currentForwardX == 0 && currentForwardY == 1) { next_mov = 1; }   // South
    if (currentForwardX == 1 && currentForwardY == 0) { next_mov = 2; }   // Right
    if (currentForwardX == -1 && currentForwardY == 0) { next_mov = 3; }  // Left

    
    return next_mov;
  }

  int Agent::pacManMovement(bool* canIMove , Board* board){
    if (currentTarget == currentPos) {
      return-1;
    }
    *canIMove = true;
    float euclideanDistance = 99999.f;
    int dst_desp = 0;
    // South
    if (currentForwardX == 0 && currentForwardY == 1) {
      if (board->checkUnitMovement(board->south(currentPos))) {

        euclideanDistance = board->euclidianDistance(currentTarget, board->south(currentPos));
        dst_desp = 1;
      }
      if (board->checkUnitMovement(board->east(currentPos))) {

        if (euclideanDistance > board->euclidianDistance(currentTarget, board->east(currentPos))) {
          euclideanDistance = board->euclidianDistance(currentTarget, board->east(currentPos));

          currentForwardX = 1;
          currentForwardY = 0;
          dst_desp = 3;
        }

      }
      if (board->checkUnitMovement(board->west(currentPos))) {

        if (euclideanDistance > board->euclidianDistance(currentTarget, board->west(currentPos))) {
          euclideanDistance = board->euclidianDistance(currentTarget, board->west(currentPos));

          currentForwardX = -1;
          currentForwardY = 0;
          dst_desp = 2;
        }

      }
      //Update Position
      return dst_desp;
    }

    // North
    if (currentForwardX == 0 && currentForwardY == -1) {
      if (board->checkUnitMovement(board->north(currentPos))) {

        euclideanDistance = board->euclidianDistance(currentTarget, board->north(currentPos));
        dst_desp = 0;
      }
      if (board->checkUnitMovement(board->east(currentPos))) {

        if (euclideanDistance > board->euclidianDistance(currentTarget, board->east(currentPos))) {
          euclideanDistance = board->euclidianDistance(currentTarget, board->east(currentPos));

          currentForwardX = 1;
          currentForwardY = 0;
          dst_desp = 3;
        }

      }
      if (board->checkUnitMovement(board->west(currentPos))) {

        if (euclideanDistance > board->euclidianDistance(currentTarget, board->west(currentPos))) {
          euclideanDistance = board->euclidianDistance(currentTarget, board->west(currentPos));

          currentForwardX = -1;
          currentForwardY = 0;
          dst_desp = 2;
        }

      }
      return dst_desp;
    }

    // East
    if (currentForwardX == 1 && currentForwardY == 0) {
      if (board->checkUnitMovement(board->east(currentPos))) {

        euclideanDistance = board->euclidianDistance(currentTarget, board->east(currentPos));
        dst_desp = 3;
      }
      if (board->checkUnitMovement(board->south(currentPos))) {

        if (euclideanDistance > board->euclidianDistance(currentTarget, board->south(currentPos))) {
          euclideanDistance = board->euclidianDistance(currentTarget, board->south(currentPos));

          currentForwardX = 0;
          currentForwardY = 1;
          dst_desp = 1;
        }

      }
      if (board->checkUnitMovement(board->north(currentPos))) {

        if (euclideanDistance > board->euclidianDistance(currentTarget, board->north(currentPos))) {
          euclideanDistance = board->euclidianDistance(currentTarget, board->north(currentPos));

          currentForwardX = 0;
          currentForwardY = -1;
          dst_desp = 0;
        }
      }
      return dst_desp;
    }

    // West
    if (currentForwardX == -1 && currentForwardY == 0) {
      if (board->checkUnitMovement(board->west(currentPos))) {

        euclideanDistance = board->euclidianDistance(currentTarget, board->west(currentPos));
        dst_desp = 2;
      }
      if (board->checkUnitMovement(board->south(currentPos))) {

        if (euclideanDistance > board->euclidianDistance(currentTarget, board->south(currentPos))) {
          euclideanDistance = board->euclidianDistance(currentTarget, board->south(currentPos));

          currentForwardX = 0;
          currentForwardY = 1;
          dst_desp = 1;
        }

      }
      if (board->checkUnitMovement(board->north(currentPos))) {
        if (euclideanDistance > board->euclidianDistance(currentTarget, board->north(currentPos))) {
          euclideanDistance = board->euclidianDistance(currentTarget, board->north(currentPos));

          currentForwardX = 0;
          currentForwardY = -1;
          dst_desp = 0;
        }

      }
    }
    return dst_desp;
  }


void Agent::addMovement2Patern(PatternMovement p_m, int32_t n_m){

  if(index_movementArray <64){
    movementArray[index_movementArray] = p_m;
    movementCounterArray[index_movementArray] = n_m;
    index_movementArray++;
  }
}

void Agent::cleanMovementPatern(){
  for (int i = 0; i < numMovements; ++i) {
    movementArray[i] = kPatternMovement_None;
    movementCounterArray[i] = 0;
  }
}

void Agent::moveUnit(Board* board) {

  int mov = 0;
  int next_tile = currentPos;
  bool will_move = false;

  if(agentID !=0){
    board->checkForPlayer(0, this);
  }

  switch (movementType) {
    case Agent::kMovement_Random: mov = randomMov(&will_move); break;;
    case Agent::kMovement_Pattern: mov = patternMov(&will_move); break;
    case Agent::kMovement_Track: mov = pacManMovement(&will_move, board);
      if (mov == -1) { will_move = false; }
      break;
  }

  if (will_move) {
    switch (mov) {
    case 0:
      next_tile = board->north(currentPos);
      break;
    case 1:
      next_tile = board->south(currentPos);
      break;
    case 2:
      next_tile = board->west(currentPos);
      break;
    case 3:
      next_tile = board->east(currentPos);
      break;
    }

    board->checkAndMove(this, next_tile);
  }
}
