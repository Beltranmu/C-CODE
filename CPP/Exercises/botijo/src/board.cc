/*
* Antekeland 2077
* Author: Javier Benito Abolafio & Ricardo Beltrán Muriel
* Mail: benitoab@esat-alumni.com & beltranmu@esat-alumni.com
* University Development @ESAT
*/

#include "board.h"
#include <stdlib.h>



// Checks the left cell to a given one
unsigned char CheckSingleNeighbour(unsigned char pos, const signed char desp){
  
  unsigned char resul = pos + desp;
    
  resul %= kBoardSize;
  
  return resul;
  
}

// Returns the number of neighbours in the same state as a given cell
unsigned char CheckNeighbours(Tile map[kBoardSize][kBoardSize], unsigned char row, unsigned char col, unsigned char state){
    
  unsigned char num_neighbours = 0;
  
  if(map[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, -1)].state_ == state){ ++num_neighbours; }   // Upper-Left
  if(map[CheckSingleNeighbour(row, -1)][col].state_ == state){ ++num_neighbours; }                             // Upper
  if(map[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, +1)].state_ == state){ ++num_neighbours; }   // Upper-Right
  if(map[row][CheckSingleNeighbour(col, -1)].state_ == state){ ++num_neighbours; }                             // Left
  if(map[row][CheckSingleNeighbour(col, +1)].state_ == state){ ++num_neighbours; }                             // Right
  if(map[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, -1)].state_ == state){ ++num_neighbours; }   // Lower-Left
  if(map[CheckSingleNeighbour(row, +1)][col].state_ == state){ ++num_neighbours; }                             // Lower
  if(map[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, +1)].state_ == state){ ++num_neighbours; }   // Lower-Right
  
  return num_neighbours;
  
}

unsigned char CheckNeighboursType(Tile map[kBoardSize][kBoardSize], unsigned char row, unsigned char col, unsigned char state, unsigned char check_type){
    
  unsigned char num_neighbours = 0;
  
  if(map[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, -1)].state_ == state &&
     map[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, -1)].type_ != check_type){ ++num_neighbours; }   // Upper-Left

  if(map[CheckSingleNeighbour(row, -1)][col].state_ == state &&
     map[CheckSingleNeighbour(row, -1)][col].type_ != check_type){ ++num_neighbours; }                             // Upper

  if(map[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, +1)].state_ == state &&
     map[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, +1)].type_ != check_type){ ++num_neighbours; }   // Upper-Right

  if(map[row][CheckSingleNeighbour(col, -1)].state_ == state &&
     map[row][CheckSingleNeighbour(col, -1)].type_ != check_type){ ++num_neighbours; }                             // Left

  if(map[row][CheckSingleNeighbour(col, +1)].state_ == state &&
     map[row][CheckSingleNeighbour(col, +1)].type_ != check_type){ ++num_neighbours; }                             // Right

  if(map[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, -1)].state_ == state &&
     map[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, -1)].type_ != check_type){ ++num_neighbours; }   // Lower-Left

  if(map[CheckSingleNeighbour(row, +1)][col].state_ == state &&
     map[CheckSingleNeighbour(row, +1)][col].type_ != check_type){ ++num_neighbours; }                             // Lower

  if(map[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, +1)].state_ == state &&
     map[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, +1)].type_ != check_type){ ++num_neighbours; }   // Lower-Right

  
  return num_neighbours;
  
}

unsigned char CheckCrossNeighbours(Tile map[kBoardSize][kBoardSize], unsigned char row, unsigned char col, unsigned char state){

  unsigned char num_neighbours = 0;

  if(map[CheckSingleNeighbour(row, -1)][col].state_ == state){ ++num_neighbours; }                             // Upper
  if(map[row][CheckSingleNeighbour(col, -1)].state_ == state){ ++num_neighbours; }                             // Left
  if(map[row][CheckSingleNeighbour(col, +1)].state_ == state){ ++num_neighbours; }                             // Right
  if(map[CheckSingleNeighbour(row, +1)][col].state_ == state){ ++num_neighbours; }                             // Lower

  return num_neighbours;

}

void SwapCells(Tile map[kBoardSize][kBoardSize],
               unsigned char row1, unsigned char col1, 
               unsigned char state1, 
               unsigned char row2, unsigned char col2, 
               unsigned char state2){

  map[row1][col1].state_ = state2;
  map[row2][col2].state_ = state1;
    
}

signed char TemporalSwap(Tile map[kBoardSize][kBoardSize], 
                         unsigned char row1,unsigned char col1,
                         unsigned char state1,
                         unsigned char row2,unsigned char col2, 
                         unsigned char state2){
  
  signed char tmp_gain;
  
  SwapCells(map, row1, col1, state1, row2, col2, state2);
  
  tmp_gain = CheckNeighbours(map, row2, col2, map[row2][col2].state_);  
  
  SwapCells(map,row1, col1, state2, row2, col2, state1);
  
  return tmp_gain;
  
}

signed char CalculateGain(Tile map[kBoardSize][kBoardSize], 
                          unsigned char row1, unsigned char col1, 
                          unsigned char row2, unsigned char col2){
  
  signed char orig_gain;
  signed char new_gain;
  
  orig_gain = CheckNeighbours(map, row2, col2, map[row2][col2].state_);
  
  new_gain = TemporalSwap(map,row2, col2, map[row2][col2].state_,
                          row1, col1, map[row1][col1].state_);
  
  return new_gain - orig_gain;
  
}

void EraseTile(Tile map[kBoardSize][kBoardSize], unsigned char row, unsigned char col, unsigned char state){

  if(CheckNeighbours(map, row, col, state) <= 3 || CheckNeighbours(map, row, col, state) == 0){

    map[row][col].state_ = 0;
    map[row][col].type_ = 0;

  }

}

void PickCell(Tile map [kBoardSize][kBoardSize], unsigned char original_row, 
              unsigned char original_col,unsigned char orig_gain){
  
  signed char horizontal_movement;
  signed char vertical_movement;
  
  unsigned char next_row;
  unsigned char next_col;
  
  signed char new_gain;
  signed char new_gain2;
  
  do{

    horizontal_movement = rand()%(kSearchRange + 1);
    
    if(kSearchRange - horizontal_movement == 0){
    
      vertical_movement = 0;
    
    }else{
      
      vertical_movement = rand()%(kSearchRange - horizontal_movement);
      
    }
    
    if(rand()%2 == 0){
      
      horizontal_movement *= -1;
      
    }
    
    if(rand()%2 == 0){
      
      vertical_movement *= -1;
      
    }
    
    next_row = CheckSingleNeighbour(original_row, vertical_movement);
    next_col = CheckSingleNeighbour(original_col, horizontal_movement);
    
  }while(horizontal_movement == 0 && vertical_movement == 0); 
  
  if(8 != CheckNeighbours(map, next_row, next_col, map[next_row][next_col].state_) &&
                          map[next_row][next_col].state_ != map[original_row][original_col].state_){
    
    // Gain in the new cell position
    new_gain = TemporalSwap(map, original_row, original_col, map[original_row][original_col].state_,
                            next_row, next_col, map[next_row][next_col].state_);
      
    new_gain -= orig_gain;

    if(0 == map[next_row][next_col].state_){

      if(new_gain >= 0 || (new_gain == -1 && rand()%kChanceToMove == 21)){
        
        SwapCells(map,original_row, original_col, map[original_row][original_col].state_,
                  next_row, next_col,
                  map[next_row][next_col].state_);
                  
      }
      
    }else{

      new_gain2 = CalculateGain(map, original_row, original_col, next_row, next_col);
      
      new_gain += new_gain2;
      
      if(new_gain >= 0 || ((new_gain == -1 || new_gain == -2) && rand()%kChanceToMove == 21)){
        
        SwapCells(map,original_row, original_col, map[original_row][original_col].state_,
                  next_row, next_col,
                  map[next_row][next_col].state_);
                  
      }
      
    }
    
  }
  
}

void ChangeTileType(Tile map[kBoardSize][kBoardSize], unsigned char row, unsigned char col, unsigned char state){

  if(state == 1 || state == 2 || state == 3 || state == 5){

    switch(CheckCrossNeighbours(map, row, col, state)){

      case 4:

        if(map[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, -1)].state_ != state){ map[row][col].type_ = 11; }   // Upper-Left
        if(map[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, +1)].state_ != state){ map[row][col].type_ = 12; }   // Upper-Right
        if(map[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, -1)].state_ != state){ map[row][col].type_ = 10; }   // Lower-Left
        if(map[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, +1)].state_ != state){ map[row][col].type_ = 9; }   // Lower-Right

      break;

      case 3:

      if(map[CheckSingleNeighbour(row, -1)][col].state_ != state){ map[row][col].type_ = 2; }                             // Upper
      if(map[row][CheckSingleNeighbour(col, -1)].state_ != state){ map[row][col].type_ = 8; }                             // Left
      if(map[row][CheckSingleNeighbour(col, +1)].state_ != state){ map[row][col].type_ = 4; }                             // Right
      if(map[CheckSingleNeighbour(row, +1)][col].state_ != state){ map[row][col].type_ = 6; }                             // Lower

      break;

      case 2:

      if(map[CheckSingleNeighbour(row, -1)][col].state_ != state &&
         map[row][CheckSingleNeighbour(col, -1)].state_ != state){ map[row][col].type_ = 1; }                             // Upper & Left
      if(map[CheckSingleNeighbour(row, -1)][col].state_ != state &&
         map[row][CheckSingleNeighbour(col, +1)].state_ != state){ map[row][col].type_ = 3; }                             // Upper & Right

      if(map[CheckSingleNeighbour(row, +1)][col].state_ != state &&
         map[row][CheckSingleNeighbour(col, -1)].state_ != state){ map[row][col].type_ = 7; }                             // Lower & Left
      if(map[CheckSingleNeighbour(row, +1)][col].state_ != state &&
         map[row][CheckSingleNeighbour(col, +1)].state_ != state){ map[row][col].type_ = 5; }                             // Lower & Right

      break;

    }

  }

   // Snow
  if(state == 3){

    // Stairs
    if(map[row][col].type_ == 6 && map[row][CheckSingleNeighbour(col, -1)].type_ != 13 &&
       map[row][CheckSingleNeighbour(col, +1)].type_ != 13){
      
      map[row][col].type_ = 13;
      
    }

    // Cave Stairs
    if(map[row][col].type_ == 0 && rand()%50 == 7 &&
       map[CheckSingleNeighbour(row, +1)][col].type_ != 13){ map[row][col].type_ = 14; }  

    // Snowy Rock
    if(map[row][col].type_ == 0 && map[CheckSingleNeighbour(row, +1)][col].type_ != 13 &&
       rand()%10 == 7){ map[row][col].type_ = 15; }

    if(CheckNeighbours(map, row, col, state) == 8 && 
       CheckNeighboursType(map, row, col, state, map[row][col].type_) == 0 &&
       rand()%10 == 7){
     
      map[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, -1)].type_ = 16;
      map[CheckSingleNeighbour(row, -1)][col].type_ = 17;
      map[row][CheckSingleNeighbour(col, -1)].type_ = 18;
      map[row][col].type_ = 19;
         
    }
       
  }

  // Deep Water
  if(state == 1){

    if(CheckNeighbours(map, row, col, state) == 8 && 
       CheckNeighboursType(map, row, col, state, 0) == 0 &&
       rand()%10 == 7){
     
      map[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, -1)].type_ = 13;
      map[CheckSingleNeighbour(row, -1)][col].type_ = 14;
      map[row][CheckSingleNeighbour(col, -1)].type_ = 15;
      map[row][col].type_ = 16;
         
    }

  }

  // Grass
  if(state == 4){

    if(rand()%50 == 7){ map[row][col].type_ = 2+(rand()%4); }

  }

  // Lava
  if(state == 5){

    if(CheckNeighbours(map, row, col, state) == 8 && 
       CheckNeighboursType(map, row, col, state, 0) == 0 &&
       rand()%10 == 7){
     
      map[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, -1)].type_ = 14;
      map[CheckSingleNeighbour(row, -1)][col].type_ = 15;
      map[row][CheckSingleNeighbour(col, -1)].type_ = 16;
      map[row][col].type_ = 17;
         
    }

    if(rand()%10 == 7 && map[row][col].type_ == 0){

      map[row][col].type_ = 13;

    }

  }

}

void SelectCasilla(Tile map[kBoardSize][kBoardSize]){
  
  const unsigned int max_repeats = kBoardSize*kBoardSize;
  const unsigned int max_steps = 1024;
  unsigned int repeats = 0;
  unsigned char rand_row;
  unsigned char rand_col;
  unsigned char gain;
  
  while(repeats < max_repeats * max_steps){
    
    rand_row = rand()%kBoardSize;
    rand_col = rand()%kBoardSize;
    
    gain = CheckNeighbours(map, rand_row, rand_col, map[rand_row][rand_col].state_);
    
      if(gain != 8 && map[rand_row][rand_col].state_ != 0){       
        PickCell(map,rand_row, rand_col, gain);
      
      }

    ++repeats;
    
  }

  for(int n = 0; n < 2; ++n){

    for(int i = 0; i < kBoardSize; ++i){

      for(int j = 0; j < kBoardSize; ++j){

        EraseTile(map, i, j, map[i][j].state_);

      }

    }

  }

  for(int n = 0; n < 1; ++n){

    for(int i = 0; i < kBoardSize; ++i){

      for(int j = 0; j < kBoardSize; ++j){

        ChangeTileType(map, i, j, map[i][j].state_);

      }

    }

  }
  
}


void Board:: initMap(const int window_w, const int window_h){
  int x = window_w/kBoardSize * (kBoardSize/kViewSize);
  int y = window_h/kBoardSize * (kBoardSize/kViewSize);
  
  for(int r = 0; r < kBoardSize; ++r){
    for(int c = 0; c < kBoardSize; ++c){
      
      map_[r][c].info_.x = c * x;
      map_[r][c].info_.y = r * y;
      map_[r][c].info_.w = x;
      map_[r][c].info_.h = y;
      map_[r][c].type_ = 0;
      
      if(rand()%100 <= kCellconcentration * 100){
        
        map_[r][c].state_ = rand()%7+1;
      }
      else{
        map_[r][c].state_ = 0;
      }
    }
  }
  
  SelectCasilla(map_);
}

void Board::drawMap(SDL_Renderer* renderer){
  
  SDL_Rect rect_subsprite;
  
  for(int r = 0; r < kBoardSize; ++r){
    for(int c = 0; c < kBoardSize; ++c){
      
      rect_subsprite.x = map_[r][c].type_ * 64;
      rect_subsprite.y = map_[r][c].state_ * 64;
      rect_subsprite.w = 64;
      rect_subsprite.h = 64;
      
      SDL_RenderCopy(renderer, map_sprite_.texture_, 
                    &rect_subsprite, &map_[r][c].info_);
    }
  }
  
  
}

