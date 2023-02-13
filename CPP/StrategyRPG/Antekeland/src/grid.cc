/**
 * Antekeland 2077
 * Author: Javier Benito Abolafio & Ricardo Beltrán Muriel
 * Mail: benitoab@esat-alumni.com & beltranmu@esat-alumni.com
 * University Development @ESAT
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "grid.h"

// Inits Layers 1 & Layer 2
void CreateBoard(){

  GameManager& gM = GameManager::Instantiate();
  
  gM.layer1_.initLayer1();
  gM.layer2_.initLayer2();
  
}

// Inits Logic boards
void InitLogic(){

  GameManager& gM = GameManager::Instantiate();

  for(int i = 0; i < Board::kBoardSize; ++i){

    for(int j = 0; j < Board::kBoardSize; ++j){

      gM.board_[i][j].init();
      gM.units_[i][j].init();

    }
  }
}

// Checks one cell depending on the pos and desp
unsigned char CheckSingleNeighbour(unsigned char pos, const int desp){
  
  unsigned char resul = pos + (unsigned char)desp;
    
  resul %= Board::kBoardSize;
  
  return resul;
  
}

// Checks a cell state
/*unsigned char CheckState(const Tile layer[Board::kBoardSize][Board::kBoardSize], const unsigned char row,
                         const unsigned char col, const unsigned char state){

  return (layer[CheckSingleNeighbour(row, i)][CheckSingleNeighbour(col, j)].state_ == state);

}

// Checks a cell state and type
unsigned char CheckStateAndEnable(const Tile layer[Board::kBoardSize][Board::kBoardSize], const unsigned char row,
                                  const unsigned char col, const unsigned char state, const unsigned char check_enable){

  return (layer[CheckSingleNeighbour(row, i)][CheckSingleNeighbour(col, j)].state_ != state ||
          layer[CheckSingleNeighbour(row, i)][CheckSingleNeighbour(col, j)].enabled_ != check_enable);

}*/

/// @brief Check Matrix n*m >= 2x2
unsigned char MatrixNxM(const Tile layer[Board::kBoardSize][Board::kBoardSize],
                        const unsigned char row, const unsigned char col, const unsigned char n,
                        const unsigned char m, const unsigned char state){

  unsigned char neighbours = 0;

  int start_row = -1*(n/2);
  int end_row = n/2 - 1 + n%2;

  int start_col = -1*(m/2);
  int end_col = m/2 - 1 + m%2;

  for(int i = start_row; i <= end_row; ++i){

    for(int j = start_col; j <= end_col; ++j){

      if(i != 0 || j != 0){

        if(layer[CheckSingleNeighbour(row, i)][CheckSingleNeighbour(col, j)].state_ == state){ ++ neighbours; }

      }

    }

  }

  return neighbours;

}

/// @brief Returns the number of neighbours in the same state as a given cell
unsigned char CheckNeighbours(const Tile layer[Board::kBoardSize][Board::kBoardSize],
                              const unsigned char row, const unsigned char col, const unsigned char n,
                              const unsigned char m, const unsigned char state){
    
  return MatrixNxM(layer, row, col, n, m, state);
  
}

/// @brief Returns the number of neighbours in different state or type
unsigned char CheckNeighboursType(const Tile layer[Board::kBoardSize][Board::kBoardSize],
                                  unsigned char row, unsigned char col, const unsigned char n,
                                  const unsigned char m, unsigned char state,
                                  unsigned char check_type){
    
  unsigned char num_neighbours = 0;

  int start_row = -1*(n/2);
  int end_row = n/2 - 1 + n%2;

  int start_col = -1*(m/2);
  int end_col = m/2 - 1 + m%2;

  for(int i = start_row; i <= end_row; ++i){

    for(int j = start_col; j <= end_col; ++j){

      if(i != 0 || j != 0){

        if(layer[CheckSingleNeighbour(row, i)][CheckSingleNeighbour(col, j)].state_ != state ||
           layer[CheckSingleNeighbour(row, i)][CheckSingleNeighbour(col, j)].enabled_ != check_type){ ++num_neighbours; }

      }

    }

  }
  
  return num_neighbours;
  
}

/// @brief Checks those cells that form an X from a given one
unsigned char CheckCrossNeighbours(const Tile layer[Board::kBoardSize][Board::kBoardSize],
                                   unsigned char row, unsigned char col, unsigned char state){

  unsigned char num_neighbours = 0;

  if(layer[CheckSingleNeighbour(row, -1)][col].state_ == state){ ++num_neighbours; }                             // Upper
  if(layer[row][CheckSingleNeighbour(col, -1)].state_ == state){ ++num_neighbours; }                             // Left
  if(layer[row][CheckSingleNeighbour(col, +1)].state_ == state){ ++num_neighbours; }                             // Right
  if(layer[CheckSingleNeighbour(row, +1)][col].state_ == state){ ++num_neighbours; }                             // Lower

  return num_neighbours;

}

/// @brief Changes the position between two cells
void SwapCells(Tile layer[Board::kBoardSize][Board::kBoardSize],
               unsigned char row1, unsigned char col1, unsigned char state1,
               unsigned char row2, unsigned char col2, unsigned char state2){

  layer[row1][col1].state_ = state2;
  layer[row2][col2].state_ = state1;
    
}

signed char TemporalSwap(Tile layer[Board::kBoardSize][Board::kBoardSize],
                         unsigned char row1, unsigned char col1, unsigned char state1,
                         unsigned char row2, unsigned char col2, unsigned char state2){
  
  signed char tmp_gain;
  
  SwapCells(layer, row1, col1, state1, row2, col2, state2);
  
  tmp_gain = CheckNeighbours(layer, row2, col2, 3, 3, layer[row2][col2].state_);  
  
  SwapCells(layer, row1, col1, state2, row2, col2, state1);
  
  return tmp_gain;
  
}

signed char CalculateGain(Tile layer[Board::kBoardSize][Board::kBoardSize],
                          unsigned char row1, unsigned char col1,
                          unsigned char row2, unsigned char col2){
  
  signed char orig_gain;
  signed char new_gain;
  
  orig_gain = CheckNeighbours(layer, row2, col2, 3, 3, layer[row2][col2].state_);
  
  new_gain = TemporalSwap(layer, row2, col2, layer[row2][col2].state_,
                          row1, col1, layer[row1][col1].state_);
  
  return new_gain - orig_gain;
  
}

/* Modified*/ // aux_layer may not be used in this function
void EraseTile(Tile layer[Board::kBoardSize][Board::kBoardSize],
               Tile aux_layer[Board::kBoardSize][Board::kBoardSize],
               unsigned char row, unsigned char col, unsigned char state){

  if(CheckNeighbours(layer, row, col, 3, 3, state) <= 3 ||
     CheckNeighbours(layer, row, col, 3, 3, state) == 0){

    layer[row][col].state_ = 0;
    layer[row][col].type_ = 0;

    aux_layer[row][col].enabled_ = 1;
    
  }

}

void PickCell(Tile layer[Board::kBoardSize][Board::kBoardSize],
              const unsigned char original_row, const unsigned char original_col,
              const unsigned char orig_gain){
  
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
  
  if(8 != CheckNeighbours(layer, next_row, next_col, 3, 3, layer[next_row][next_col].state_) &&
                          layer[next_row][next_col].state_ != layer[original_row][original_col].state_){
    
    // Gain in the new cell position
    new_gain = TemporalSwap(layer, original_row, original_col,
                            layer[original_row][original_col].state_,
                            next_row, next_col, layer[next_row][next_col].state_);
      
    new_gain -= orig_gain;

    if(0 == layer[next_row][next_col].state_){

      if(new_gain >= 0 || (new_gain == -1 && rand()%kChanceToMove == 21)){
        
        SwapCells(layer, original_row, original_col, layer[original_row][original_col].state_,
                  next_row, next_col, layer[next_row][next_col].state_);
                  
      }
      
    }else{

      new_gain2 = CalculateGain(layer, original_row, original_col, next_row, next_col);
      
      new_gain += new_gain2;
      
      if(new_gain >= 0 || ((new_gain == -1 || new_gain == -2) && rand()%kChanceToMove == 21)){
        
        SwapCells(layer, original_row, original_col,
                  layer[original_row][original_col].state_,
                  next_row, next_col, layer[next_row][next_col].state_);
                  
      }
      
    }
    
  }
  
}

// Changes every tile to their new type
void ChangeTileType(Tile layer[Board::kBoardSize][Board::kBoardSize],
                    Tile aux_layer[Board::kBoardSize][Board::kBoardSize],
                    unsigned char row, unsigned char col, unsigned char state,
                    unsigned char* ship, unsigned char* shop){

  GameManager& gM = GameManager::Instantiate();                  

  if(state == 1 || state == 2 || state == 3 || state == 5){

    // Water/Snow/Lava Edges
    switch(CheckCrossNeighbours(layer, row, col, state)){
        
      case 4:
        
        if(layer[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, -1)].state_ != state){               // Upper-Left
          
          layer[row][col].type_ = 11;
          layer[row][col].enabled_ = 0;
          aux_layer[row][col].enabled_ = 0;

          gM.board_[row][col].enabled_ = 0;

        }

        if(layer[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, +1)].state_ != state){               // Upper-Right                        
          
          layer[row][col].type_ = 12;
          layer[row][col].enabled_ = 0;
          aux_layer[row][col].enabled_ = 0;

          gM.board_[row][col].enabled_ = 0;
        
        }   

        if(layer[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, -1)].state_ != state){               // Lower-Left
          
          layer[row][col].type_ = 10;
          layer[row][col].enabled_ = 0;
          aux_layer[row][col].enabled_ = 0;

          gM.board_[row][col].enabled_ = 0;
       
        }

        if(layer[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, +1)].state_ != state){               // Lower-Right
          
          layer[row][col].type_ = 9;
          layer[row][col].enabled_ = 0;
          aux_layer[row][col].enabled_ = 0;

          gM.board_[row][col].enabled_ = 0;
        
        }   

      break;

      case 3:

        if(layer[CheckSingleNeighbour(row, -1)][col].state_ != state){                                           // Upper
          
          layer[row][col].type_ = 2;
          layer[row][col].enabled_ = 0;
          aux_layer[row][col].enabled_ = 0;

          gM.board_[row][col].enabled_ = 0;
       
        }                             
        if(layer[row][CheckSingleNeighbour(col, -1)].state_ != state){                                           // Left
          
          layer[row][col].type_ = 8;
          layer[row][col].enabled_ = 0;
          aux_layer[row][col].enabled_ = 0;

          gM.board_[row][col].enabled_ = 0;
        
        }                             
        if(layer[row][CheckSingleNeighbour(col, +1)].state_ != state){                                           // Right
          
          layer[row][col].type_ = 4;
          layer[row][col].enabled_ = 0;
          aux_layer[row][col].enabled_ = 0;

          gM.board_[row][col].enabled_ = 0;
       
        }                             
        if(layer[CheckSingleNeighbour(row, +1)][col].state_ != state){                                           // Lower
          
          layer[row][col].type_ = 6;
          layer[row][col].enabled_ = 0;
          aux_layer[row][col].enabled_ = 0;

          gM.board_[row][col].enabled_ = 0;
       
        }                             

      break;

      case 2:

        if(layer[CheckSingleNeighbour(row, -1)][col].state_ != state &&                                          // Upper & Left
           layer[row][CheckSingleNeighbour(col, -1)].state_ != state){
            
            layer[row][col].type_ = 1;
            layer[row][col].enabled_ = 0;
            aux_layer[row][col].enabled_ = 0;

            gM.board_[row][col].enabled_ = 0;
        
        }

        if(layer[CheckSingleNeighbour(row, -1)][col].state_ != state &&                                          // Upper & Right
           layer[row][CheckSingleNeighbour(col, +1)].state_ != state){

            layer[row][col].type_ = 3;
            layer[row][col].enabled_ = 0;
            aux_layer[row][col].enabled_ = 0;

            gM.board_[row][col].enabled_ = 0;
        
        }                             

        if(layer[CheckSingleNeighbour(row, +1)][col].state_ != state &&                                          // Lower & Left
           layer[row][CheckSingleNeighbour(col, -1)].state_ != state){
            
            layer[row][col].type_ = 7;
            layer[row][col].enabled_ = 0;
            aux_layer[row][col].enabled_ = 0;

            gM.board_[row][col].enabled_ = 0;
        
        } 

        if(layer[CheckSingleNeighbour(row, +1)][col].state_ != state &&                                          // Lower & Right
           layer[row][CheckSingleNeighbour(col, +1)].state_ != state){
            
            layer[row][col].type_ = 5;
            layer[row][col].enabled_ = 0;
            aux_layer[row][col].enabled_ = 0;

            gM.board_[row][col].enabled_ = 0;
        
        }                             

      break;

    }

  }

  if(state == 2){
    gM.board_[row][col].enabled_ = 1;
  }

  // Normal Path
  if(state == 0){

    // Shop (only one)
    if(CheckNeighboursType(aux_layer, row, col, 4, 4, 0, 1) == 0 && *shop == 0){
      
      // Graphic
      aux_layer[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, -2)].type_ = 37;
      aux_layer[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, -1)].type_ = 38;
      aux_layer[CheckSingleNeighbour(row, -1)][col].type_ = 39;
      aux_layer[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, +1)].type_ = 40;

      aux_layer[row][CheckSingleNeighbour(col, -2)].type_ = 41;
      aux_layer[row][CheckSingleNeighbour(col, -1)].type_ = 42;
      aux_layer[row][col].type_ = 43;
      aux_layer[row][CheckSingleNeighbour(col, +1)].type_ = 44;

      aux_layer[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, -2)].type_ = 45;
      aux_layer[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, -1)].type_ = 46;
      aux_layer[CheckSingleNeighbour(row, +1)][col].type_ = 47;
      aux_layer[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, +1)].type_ = 48;

      aux_layer[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, -2)].enabled_ = 0;
      aux_layer[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, -1)].enabled_ = 0;
      aux_layer[CheckSingleNeighbour(row, -1)][col].enabled_ = 0;
      aux_layer[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, +1)].enabled_ = 0;
      aux_layer[row][CheckSingleNeighbour(col, -2)].enabled_ = 0;
      aux_layer[row][CheckSingleNeighbour(col, -1)].enabled_ = 0;
      aux_layer[row][col].enabled_ = 0;
      aux_layer[row][CheckSingleNeighbour(col, +1)].enabled_ = 0;
      aux_layer[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, -2)].enabled_ = 0;
      aux_layer[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, -1)].enabled_ = 0;
      aux_layer[CheckSingleNeighbour(row, +1)][col].enabled_ = 0;
      aux_layer[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, +1)].enabled_ = 0;

      // Logic
      gM.board_[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, -1)].enter_ = 2;

      gM.board_[row][CheckSingleNeighbour(col, -2)].enabled_ = 0;
      gM.board_[row][CheckSingleNeighbour(col, -1)].enabled_ = 0;
      gM.board_[row][col].enabled_ = 0;
      gM.board_[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, -2)].enabled_ = 0;
      gM.board_[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, -1)].enabled_ = 0;
      gM.board_[CheckSingleNeighbour(row, +1)][col].enabled_ = 0;

      *shop = 1;

    }
  
    // Single plant
    if(rand()%20 == 7 && aux_layer[row][col].enabled_ == 1){   

      layer[row][col].enabled_ = 1;
      aux_layer[row][col].enabled_ = 0;
      aux_layer[row][col].type_ = 2;
      
      gM.board_[row][col].enabled_ = 1;

    }

    // Wood & info panel
    if(rand()%70 == 21 && aux_layer[row][col].enabled_ == 1){
      
      layer[row][col].enabled_ = 0;
      aux_layer[row][col].enabled_ = 0;
      aux_layer[row][col].type_ = 3 + rand()%4;

      gM.board_[row][col].enabled_ = 0;

    }

    // Lights
    if(CheckNeighboursType(aux_layer, row, col, 2, 2, 0, 1) == 0 && rand()%100 == 7){
      
      layer[row][col].enabled_ = 0;
      layer[CheckSingleNeighbour(row, -1)][col].enabled_ = 1;
      aux_layer[row][col].enabled_ = 0;
      aux_layer[CheckSingleNeighbour(row, -1)][col].enabled_ = 0;
      aux_layer[row][col].type_ = 8;
      aux_layer[CheckSingleNeighbour(row, -1)][col].type_ = 7;

      gM.board_[row][col].enabled_ = 0;

    }

    // Tree
    if(CheckNeighboursType(aux_layer, row, col, 4, 4, 0, 1) == 0){
      
      layer[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, -1)].enabled_ = 1;
      layer[CheckSingleNeighbour(row, -1)][col].enabled_ = 1;
      layer[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, +1)].enabled_ = 1;
      layer[row][CheckSingleNeighbour(col, -1)].enabled_ = 1;
      layer[row][col].enabled_ = 1;
      layer[row][CheckSingleNeighbour(col, +1)].enabled_ = 1;
      layer[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, -1)].enabled_ = 0;
      layer[CheckSingleNeighbour(row, +1)][col].enabled_ = 0;
      layer[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, +1)].enabled_ = 1;

      aux_layer[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, -1)].enabled_ = 0;
      aux_layer[CheckSingleNeighbour(row, -1)][col].enabled_ = 0;
      aux_layer[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, +1)].enabled_ = 0;
      aux_layer[row][CheckSingleNeighbour(col, -1)].enabled_ = 0;
      aux_layer[row][col].enabled_ = 0;
      aux_layer[row][CheckSingleNeighbour(col, +1)].enabled_ = 0;
      aux_layer[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, -1)].enabled_ = 0;
      aux_layer[CheckSingleNeighbour(row, +1)][col].enabled_ = 0;
      aux_layer[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, +1)].enabled_ = 0;

      switch(rand()%4){

        case 0:

          // Graphic
          aux_layer[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, -1)].type_ = 9;
          aux_layer[CheckSingleNeighbour(row, -1)][col].type_ = 10;
          aux_layer[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, +1)].type_ = 11;
          aux_layer[row][CheckSingleNeighbour(col, -1)].type_ = 12;
          aux_layer[row][col].type_ = 13;
          aux_layer[row][CheckSingleNeighbour(col, +1)].type_ = 14;
          aux_layer[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, -1)].type_ = 15;
          aux_layer[CheckSingleNeighbour(row, +1)][col].type_ = 16;
          aux_layer[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, +1)].type_ = 17;

          // Logic          
          gM.board_[row][col].enabled_ = 0;                            // 13
          gM.board_[CheckSingleNeighbour(row, +1)][col].enabled_ = 0;  // 16

        break;
      
        case 1:

          // Graphic
          aux_layer[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, -1)].type_ = 18;
          aux_layer[CheckSingleNeighbour(row, -1)][col].type_ = 19;
          aux_layer[row][CheckSingleNeighbour(col, -1)].type_ = 20;
          aux_layer[row][col].type_ = 21;
          aux_layer[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, -1)].type_ = 22;
          aux_layer[CheckSingleNeighbour(row, +1)][col].type_ = 23;

          // Logic
          gM.board_[row][CheckSingleNeighbour(col, -1)].enabled_ = 0;  // 20
          gM.board_[row][col].enabled_ = 0;                            // 21  

        break;

        case 2:

          // Graphic
          aux_layer[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, -1)].type_ = 24;
          aux_layer[CheckSingleNeighbour(row, -1)][col].type_ = 25;
          aux_layer[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, +1)].type_ = 26;
          aux_layer[row][CheckSingleNeighbour(col, -1)].type_ = 27;
          aux_layer[row][col].type_ = 28;
          aux_layer[row][CheckSingleNeighbour(col, +1)].type_ = 29;
          aux_layer[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, -1)].type_ = 30;
          aux_layer[CheckSingleNeighbour(row, +1)][col].type_ = 31;
          aux_layer[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, +1)].type_ = 32;

          // Logic
          gM.board_[row][col].enabled_ = 0;                            // 28
          gM.board_[CheckSingleNeighbour(row, +1)][col].enabled_ = 0;  // 31

        break;

        case 3:     

          // Graphic   
          aux_layer[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, -1)].type_ = 33;
          aux_layer[CheckSingleNeighbour(row, -1)][col].type_ = 34;
          aux_layer[row][CheckSingleNeighbour(col, -1)].type_ = 35;
          aux_layer[row][col].type_ = 36;

          // Logic
          gM.board_[row][CheckSingleNeighbour(col, -1)].enabled_ = 0;
          gM.board_[row][col].enabled_ = 0;

        break;

      }

    }

  }

  // City
  // Water Fountain
  if(CheckNeighboursType(aux_layer, row, col, 3, 3, 8, 1) == 0 && rand()%20 == 7){

    layer[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, -1)].enabled_ = 0;
    layer[CheckSingleNeighbour(row, -1)][col].enabled_ = 0;
    layer[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, +1)].enabled_ = 0;
    layer[row][CheckSingleNeighbour(col, -1)].enabled_ = 0;
    layer[row][col].enabled_ = 0;
    layer[row][CheckSingleNeighbour(col, +1)].enabled_ = 0;

    // Graphic
    aux_layer[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, -1)].type_ = 23;
    aux_layer[CheckSingleNeighbour(row, -1)][col].type_ = 24;
    aux_layer[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, +1)].type_ = 25;
    aux_layer[row][CheckSingleNeighbour(col, -1)].type_ = 26;
    aux_layer[row][col].type_ = 27;
    aux_layer[row][CheckSingleNeighbour(col, +1)].type_ = 28;

    aux_layer[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, -1)].enabled_ = 0;
    aux_layer[CheckSingleNeighbour(row, -1)][col].enabled_ = 0;
    aux_layer[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, +1)].enabled_ = 0;
    aux_layer[row][CheckSingleNeighbour(col, -1)].enabled_ = 0;
    aux_layer[row][col].enabled_ = 0;
    aux_layer[row][CheckSingleNeighbour(col, +1)].enabled_ = 0;

    // Logic
    gM.board_[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, -1)].enabled_ = 0;  // 23
    gM.board_[CheckSingleNeighbour(row, -1)][col].enabled_ = 0;                            // 24
    gM.board_[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, +1)].enabled_ = 0;  // 25
    gM.board_[row][CheckSingleNeighbour(col, -1)].enabled_ = 0;                            // 26
    gM.board_[row][col].enabled_ = 0;                                                      // 27
    gM.board_[row][CheckSingleNeighbour(col, +1)].enabled_ = 0;                            // 28

  }

  if(rand()%10 == 7){

    switch(rand()%3){

      case 0: // Big house

        if(CheckNeighboursType(aux_layer, row, col, 3, 3, 8, 1) == 0){

          layer[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, -1)].enabled_ = 1;
          layer[CheckSingleNeighbour(row, -1)][col].enabled_ = 1;
          layer[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, +1)].enabled_ = 1;
          layer[row][CheckSingleNeighbour(col, -1)].enabled_ = 0;
          layer[row][col].enabled_ = 0;
          layer[row][CheckSingleNeighbour(col, +1)].enabled_ = 0;
          layer[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, -1)].enabled_ = 0;
          layer[CheckSingleNeighbour(row, +1)][col].enabled_ = 0;
          layer[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, +1)].enabled_ = 0;

          // Graphic
          aux_layer[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, -1)].type_ = 1;
          aux_layer[CheckSingleNeighbour(row, -1)][col].type_ = 2;
          aux_layer[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, +1)].type_ = 3;
          aux_layer[row][CheckSingleNeighbour(col, -1)].type_ = 4;
          aux_layer[row][col].type_ = 5;
          aux_layer[row][CheckSingleNeighbour(col, +1)].type_ = 6;
          aux_layer[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, -1)].type_ = 7;
          aux_layer[CheckSingleNeighbour(row, +1)][col].type_ = 8;
          aux_layer[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, +1)].type_ = 9;

          aux_layer[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, -1)].enabled_ = 0;
          aux_layer[CheckSingleNeighbour(row, -1)][col].enabled_ = 0;
          aux_layer[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, +1)].enabled_ = 0;
          aux_layer[row][CheckSingleNeighbour(col, -1)].enabled_ = 0;
          aux_layer[row][col].enabled_ = 0;
          aux_layer[row][CheckSingleNeighbour(col, +1)].enabled_ = 0;
          aux_layer[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, -1)].enabled_ = 0;
          aux_layer[CheckSingleNeighbour(row, +1)][col].enabled_ = 0;
          aux_layer[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, +1)].enabled_ = 0;

          // Logic
          gM.board_[row][CheckSingleNeighbour(col, -1)].enabled_ = 0;
          gM.board_[row][col].enabled_ = 0;
          gM.board_[row][CheckSingleNeighbour(col, +1)].enabled_ = 0;
          gM.board_[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, -1)].enabled_ = 0;
          gM.board_[CheckSingleNeighbour(row, +1)][col].enabled_ = 0;
          gM.board_[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, +1)].enabled_ = 0;
          
        }

      break;

      case 1: // 3x2 blue house

        if(CheckNeighboursType(aux_layer, row, col, 3, 2, 8, 1) == 0){
          
          layer[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, -1)].enabled_ = 1;
          layer[CheckSingleNeighbour(row, -1)][col].enabled_ = 1;
          layer[row][CheckSingleNeighbour(col, -1)].enabled_ = 0;
          layer[row][col].enabled_ = 0;
          layer[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, -1)].enabled_ = 0;
          layer[CheckSingleNeighbour(row, +1)][col].enabled_ = 0;

          // Graphic
          aux_layer[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, -1)].type_ = 10;
          aux_layer[CheckSingleNeighbour(row, -1)][col].type_ = 11;
          aux_layer[row][CheckSingleNeighbour(col, -1)].type_ = 12;
          aux_layer[row][col].type_ = 13;
          aux_layer[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, -1)].type_ = 14;
          aux_layer[CheckSingleNeighbour(row, +1)][col].type_ = 15;

          aux_layer[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, -1)].enabled_ = 0;
          aux_layer[CheckSingleNeighbour(row, -1)][col].enabled_ = 0;
          aux_layer[row][CheckSingleNeighbour(col, -1)].enabled_ = 0;
          aux_layer[row][col].enabled_ = 0;
          aux_layer[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, -1)].enabled_ = 0;
          aux_layer[CheckSingleNeighbour(row, +1)][col].enabled_ = 0;

          // Logic         
          gM.board_[row][CheckSingleNeighbour(col, -1)].enabled_ = 0;
          gM.board_[row][col].enabled_ = 0;
          gM.board_[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, -1)].enabled_ = 0;
          gM.board_[CheckSingleNeighbour(row, +1)][col].enabled_ = 0;

        }

      break;

      case 2: // 3x2 red house

        if(CheckNeighboursType(aux_layer, row, col, 3, 2, 8, 1) == 0){

          layer[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, -1)].enabled_ = 1;
          layer[CheckSingleNeighbour(row, -1)][col].enabled_ = 1;
          layer[row][CheckSingleNeighbour(col, -1)].enabled_ = 0;
          layer[row][col].enabled_ = 0;
          layer[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, -1)].enabled_ = 0;
          layer[CheckSingleNeighbour(row, +1)][col].enabled_ = 0;

          // Graphic
          aux_layer[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, -1)].type_ = 16;
          aux_layer[CheckSingleNeighbour(row, -1)][col].type_ = 17;
          aux_layer[row][CheckSingleNeighbour(col, -1)].type_ = 18;
          aux_layer[row][col].type_ = 19;
          aux_layer[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, -1)].type_ = 20;
          aux_layer[CheckSingleNeighbour(row, +1)][col].type_ = 21;

          aux_layer[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, -1)].enabled_ = 0;
          aux_layer[CheckSingleNeighbour(row, -1)][col].enabled_ = 0;
          aux_layer[row][CheckSingleNeighbour(col, -1)].enabled_ = 0;
          aux_layer[row][col].enabled_ = 0;
          aux_layer[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, -1)].enabled_ = 0;
          aux_layer[CheckSingleNeighbour(row, +1)][col].enabled_ = 0;

          // Logic
          gM.board_[row][CheckSingleNeighbour(col, -1)].enabled_ = 0;
          gM.board_[row][col].enabled_ = 0;
          gM.board_[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, -1)].enabled_ = 0;
          gM.board_[CheckSingleNeighbour(row, +1)][col].enabled_ = 0;

        }

      break;

    }

  }

  // Streetlights
  if(rand()%50 == 7 && CheckNeighboursType(aux_layer, row, col, 2, 2, 8, 1) == 0){

    layer[row][col].enabled_ = 0;
     
    // Graphic
    aux_layer[row][col].enabled_ = 0;
    aux_layer[row][col].type_ = 22;

    // Logic
    gM.board_[row][col].enabled_ = 0;

  }

  // Yellow Desert
  if(CheckNeighboursType(aux_layer, row, col, 3, 3, 7, 1) == 0){

    layer[row][col].enabled_ = 0;
    aux_layer[row][col].enabled_ = 0;
    aux_layer[row][col].type_ = 1+rand()%5;

    if(aux_layer[row][col].type_ == 4){ gM.board_[row][col].enter_ = 1; gM.board_[row][col].enabled_ = 0;}      // Sand stairs
    if(aux_layer[row][col].type_ != 0 && aux_layer[row][col].type_ != 4){   // Sand obstacles
      gM.board_[row][col].enabled_ = 0;
    }

  }

  // Red Desert
  if(state == 6){

    if(layer[row][col].type_ == 0 &&
       CheckNeighboursType(aux_layer, row, col, 4, 3, 6, 1) == 0){

      unsigned char rand_building = rand()%4;

      switch(rand_building){

        case 0: // 2x2

          // Graphic
          layer[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, -1)].type_ = 1;
          layer[CheckSingleNeighbour(row, -1)][col].type_ = 3;
          layer[row][CheckSingleNeighbour(col, -1)].type_ = 7;
          layer[row][col].type_ = 5;

          layer[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, -1)].enabled_ = 0;
          layer[CheckSingleNeighbour(row, -1)][col].enabled_ = 0;
          layer[row][CheckSingleNeighbour(col, -1)].enabled_ = 0;
          layer[row][col].enabled_ = 0;
          
          aux_layer[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, -1)].enabled_ = 0;
          aux_layer[CheckSingleNeighbour(row, -1)][col].enabled_ = 0;
          aux_layer[row][CheckSingleNeighbour(col, -1)].enabled_ = 0;
          aux_layer[row][col].enabled_ = 0;

          // Logic
          gM.board_[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, -1)].enabled_ = 0;
          gM.board_[CheckSingleNeighbour(row, -1)][col].enabled_ = 0;
          gM.board_[row][CheckSingleNeighbour(col, -1)].enabled_ = 0;
          gM.board_[row][col].enabled_ = 0;

        break;

        case 1:   // 2x3

          // Graphic
          layer[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, -1)].type_ = 1;
          layer[CheckSingleNeighbour(row, -1)][col].type_ = 2;
          layer[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, +1)].type_ = 3;
          layer[row][CheckSingleNeighbour(col, -1)].type_ = 7;
          layer[row][col].type_ = 6;
          layer[row][CheckSingleNeighbour(col, +1)].type_ = 5;

          layer[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, -1)].enabled_ = 0;
          layer[CheckSingleNeighbour(row, -1)][col].enabled_ = 0;
          layer[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, +1)].enabled_ = 0;
          layer[row][CheckSingleNeighbour(col, -1)].enabled_ = 0;
          layer[row][col].enabled_ = 0;
          layer[row][CheckSingleNeighbour(col, +1)].enabled_ = 0;

          aux_layer[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, -1)].enabled_ = 0;
          aux_layer[CheckSingleNeighbour(row, -1)][col].enabled_ = 0;
          aux_layer[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, +1)].enabled_ = 0;
          aux_layer[row][CheckSingleNeighbour(col, -1)].enabled_ = 0;
          aux_layer[row][col].enabled_ = 0;
          aux_layer[row][CheckSingleNeighbour(col, +1)].enabled_ = 0;

          // Logic
          gM.board_[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, -1)].enabled_ = 0;
          gM.board_[CheckSingleNeighbour(row, -1)][col].enabled_ = 0;
          gM.board_[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, +1)].enabled_ = 0;
          gM.board_[row][CheckSingleNeighbour(col, -1)].enabled_ = 0;
          gM.board_[row][col].enabled_ = 0;
          gM.board_[row][CheckSingleNeighbour(col, +1)].enabled_ = 0;

        break;

        case 2:   // 3x2

          // Graphic
          layer[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, -1)].type_ = 1;
          layer[CheckSingleNeighbour(row, -1)][col].type_ = 3;
          layer[row][CheckSingleNeighbour(col, -1)].type_ = 8;
          layer[row][col].type_ = 4;
          layer[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, -1)].type_ = 7;
          layer[CheckSingleNeighbour(row, +1)][col].type_ = 5;

          layer[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, -1)].enabled_ = 0;
          layer[CheckSingleNeighbour(row, -1)][col].enabled_ = 0;
          layer[row][CheckSingleNeighbour(col, -1)].enabled_ = 0;
          layer[row][col].enabled_ = 0;
          layer[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, -1)].enabled_ = 0;
          layer[CheckSingleNeighbour(row, +1)][col].enabled_ = 0;
          
          aux_layer[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, -1)].enabled_ = 0;
          aux_layer[CheckSingleNeighbour(row, -1)][col].enabled_ = 0;
          aux_layer[row][CheckSingleNeighbour(col, -1)].enabled_ = 0;
          aux_layer[row][col].enabled_ = 0;
          aux_layer[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, -1)].enabled_ = 0;
          aux_layer[CheckSingleNeighbour(row, +1)][col].enabled_ = 0;

          // Logic
          gM.board_[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, -1)].enabled_ = 0;
          gM.board_[CheckSingleNeighbour(row, -1)][col].enabled_ = 0;
          gM.board_[row][CheckSingleNeighbour(col, -1)].enabled_ = 0;
          gM.board_[row][col].enabled_ = 0;
          gM.board_[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, -1)].enabled_ = 0;
          gM.board_[CheckSingleNeighbour(row, +1)][col].enabled_ = 0;

        break;

        case 3:   // 3x3

          // Graphic
          layer[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, -1)].type_ = 1;
          layer[CheckSingleNeighbour(row, -1)][col].type_ = 2;
          layer[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, +1)].type_ = 3;
          layer[row][CheckSingleNeighbour(col, -1)].type_ = 8;
          layer[row][col].type_ = 10;
          layer[row][CheckSingleNeighbour(col, +1)].type_ = 4;
          layer[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, -1)].type_ = 7;
          layer[CheckSingleNeighbour(row, +1)][col].type_ = 6;
          layer[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, +1)].type_ = 5;

          layer[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, -1)].enabled_ = 0;
          layer[CheckSingleNeighbour(row, -1)][col].enabled_ = 0;
          layer[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, +1)].enabled_ = 0;
          layer[row][CheckSingleNeighbour(col, -1)].enabled_ = 0;
          layer[row][col].enabled_ = 0;
          layer[row][CheckSingleNeighbour(col, +1)].enabled_ = 0;
          layer[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, -1)].enabled_ = 0;
          layer[CheckSingleNeighbour(row, +1)][col].enabled_ = 0;
          layer[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, +1)].enabled_ = 0;

          aux_layer[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, -1)].enabled_ = 0;
          aux_layer[CheckSingleNeighbour(row, -1)][col].enabled_ = 0;
          aux_layer[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, +1)].enabled_ = 0;
          aux_layer[row][CheckSingleNeighbour(col, -1)].enabled_ = 0;
          aux_layer[row][col].enabled_ = 0;
          aux_layer[row][CheckSingleNeighbour(col, +1)].enabled_ = 0;
          aux_layer[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, -1)].enabled_ = 0;
          aux_layer[CheckSingleNeighbour(row, +1)][col].enabled_ = 0;
          aux_layer[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, +1)].enabled_ = 0;

          // Logic
          gM.board_[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, -1)].enabled_ = 0;
          gM.board_[CheckSingleNeighbour(row, -1)][col].enabled_ = 0;
          gM.board_[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, +1)].enabled_ = 0;
          gM.board_[row][CheckSingleNeighbour(col, -1)].enabled_ = 0;
          gM.board_[row][col].enabled_ = 1;
          gM.board_[row][CheckSingleNeighbour(col, +1)].enabled_ = 0;
          gM.board_[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, -1)].enabled_ = 0;
          gM.board_[CheckSingleNeighbour(row, +1)][col].enabled_ = 0;
          gM.board_[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, +1)].enabled_ = 0;

        break;

      }

    }

    // Stairs
    if(layer[row][col].type_ == 6 && layer[CheckSingleNeighbour(row, +1)][col].type_ == 0){

      layer[row][col].type_ = 9;
      layer[row][col].enabled_ = 1;

      gM.board_[row][col].enabled_ = 1;

    }

  }

  // Snow
  if(state == 3){

    // Stairs
    if(layer[row][col].type_ == 6 && layer[row][CheckSingleNeighbour(col, -1)].type_ != 13 &&
       layer[row][CheckSingleNeighbour(col, +1)].type_ != 13){
      
      layer[row][col].type_ = 13;
      layer[row][col].enabled_ = 1;

      gM.board_[row][col].enabled_ = 1;
      
    }

    // Cave Stairs
    if(layer[row][col].type_ == 0 && rand()%50 == 7 &&
       layer[CheckSingleNeighbour(row, +1)][col].type_ != 13){

      layer[row][col].enabled_ = 1;
      aux_layer[row][col].type_ = 14;
      aux_layer[row][col].enabled_ = 0;

      // Logic
      gM.board_[row][col].enabled_ = 0;
      gM.board_[row][col].enter_ = 1;
        
    }  

    // Snowy Rock
    if(CheckNeighboursType(aux_layer, row, col, 3, 3, 3, 1) == 0){
      
      layer[row][col].enabled_ = 0;
      aux_layer[row][col].type_ = 15;
      aux_layer[row][col].enabled_ = 0;

      // Logic
      gM.board_[row][col].enabled_ = 0;
      
    }

    // Cave
    if(layer[row][col].type_ == 0 && rand()%25 == 7 && aux_layer[row][col].enabled_ == 1 && layer[CheckSingleNeighbour(row, +1)][col].type_ != 13){
           
      layer[row][col].enabled_ = 1;
      aux_layer[row][col].type_ = 16;
      aux_layer[row][col].enabled_ = 0;

      // Logic
      gM.board_[row][col].enabled_ = 0;
      gM.board_[row][col].enter_ = 1;
         
    }
       
  }

  // Shallow Water
  if(CheckNeighboursType(aux_layer, row, col, 2, 2, 2, 1) == 0 && rand()%15 == 7){

    switch(rand()%2){

      case 0:

        layer[row][col].enabled_ = 0;
        aux_layer[row][col].type_ = 13;
        aux_layer[row][col].enabled_ = 0;

      break;

      case 1:

        layer[row][col].enabled_ = 0;
        aux_layer[row][col].type_ = 14;
        aux_layer[row][col].enabled_ = 0;

      break;

    }

    gM.board_[row][col].enabled_ = 0;    

  }

  // Deep Water
  if(state == 1){

    // Marine Cave
    if(CheckNeighboursType(aux_layer, row, col, 3, 3, 1, 1) == 0 && rand()%20 == 7){
     
      layer[row][col].enabled_ = 1;
      aux_layer[row][col].type_ = 13;
      aux_layer[row][col].enabled_ = 0;

      // Logic
      gM.board_[row][col].enabled_ = 0;
      gM.board_[row][col].enter_ = 1;
         
    }

    // Marine Rock
    if(aux_layer[row][col].enabled_ == 1 && rand()%20 == 7){
      
      aux_layer[row][col].type_ = 14;
      aux_layer[row][col].enabled_ = 0;
      layer[row][col].enabled_ = 0;

      // Logic
      gM.board_[row][col].enabled_ = 0;
      
    }

    // One Piece Ship
    if(CheckNeighboursType(aux_layer, row, col, 5, 5, 1, 1) == 0 && *ship == 0){

      aux_layer[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, -2)].type_ = 15;
      aux_layer[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, -1)].type_ = 16;
      aux_layer[CheckSingleNeighbour(row, -1)][col].type_ = 17;
      aux_layer[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, +1)].type_ = 18;

      aux_layer[row][CheckSingleNeighbour(col, -2)].type_ = 19;
      aux_layer[row][CheckSingleNeighbour(col, -1)].type_ = 20;
      aux_layer[row][col].type_ = 21;
      aux_layer[row][CheckSingleNeighbour(col, +1)].type_ = 22;

      aux_layer[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, -2)].type_ = 23;
      aux_layer[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, -1)].type_ = 24;
      aux_layer[CheckSingleNeighbour(row, +1)][col].type_ = 25;
      aux_layer[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, +1)].type_ = 26;

      aux_layer[CheckSingleNeighbour(row, +2)][CheckSingleNeighbour(col, -2)].type_ = 27;
      aux_layer[CheckSingleNeighbour(row, +2)][CheckSingleNeighbour(col, -1)].type_ = 28;
      aux_layer[CheckSingleNeighbour(row, +2)][col].type_ = 29;
      aux_layer[CheckSingleNeighbour(row, +2)][CheckSingleNeighbour(col, +1)].type_ = 30;

      layer[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, -1)].enabled_ = 0;
      layer[CheckSingleNeighbour(row, -1)][col].enabled_ = 0;
      layer[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, +1)].enabled_ = 0;
      layer[row][CheckSingleNeighbour(col, -1)].enabled_ = 0;
      layer[row][col].enabled_ = 0;
      layer[row][CheckSingleNeighbour(col, +1)].enabled_ = 0;
      layer[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, -1)].enabled_ = 0;
      layer[CheckSingleNeighbour(row, +1)][col].enabled_ = 0;
      layer[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, +1)].enabled_ = 0;

      // Disable Layer 2
      aux_layer[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, -2)].enabled_ = 0;
      aux_layer[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, -1)].enabled_ = 0;
      aux_layer[CheckSingleNeighbour(row, -1)][col].enabled_ = 0;
      aux_layer[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, +1)].enabled_ = 0;

      aux_layer[row][CheckSingleNeighbour(col, -2)].enabled_ = 0;
      aux_layer[row][CheckSingleNeighbour(col, -1)].enabled_ = 0;
      aux_layer[row][col].enabled_ = 0;
      aux_layer[row][CheckSingleNeighbour(col, +1)].enabled_ = 0;

      aux_layer[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, -2)].enabled_ = 0;
      aux_layer[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, -1)].enabled_ = 0;
      aux_layer[CheckSingleNeighbour(row, +1)][col].enabled_ = 0;
      aux_layer[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, +1)].enabled_ = 0;

      aux_layer[CheckSingleNeighbour(row, +2)][CheckSingleNeighbour(col, -2)].enabled_ = 0;
      aux_layer[CheckSingleNeighbour(row, +2)][CheckSingleNeighbour(col, -1)].enabled_ = 0;
      aux_layer[CheckSingleNeighbour(row, +2)][col].enabled_ = 0;
      aux_layer[CheckSingleNeighbour(row, +2)][CheckSingleNeighbour(col, +1)].enabled_ = 0;

      // Logic
      gM.board_[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, -2)].enabled_ = 0;
      gM.board_[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, -1)].enabled_ = 0;
      gM.board_[CheckSingleNeighbour(row, +1)][col].enabled_ = 0;
      gM.board_[CheckSingleNeighbour(row, +1)][CheckSingleNeighbour(col, +1)].enabled_ = 0;

      gM.board_[CheckSingleNeighbour(row, +2)][CheckSingleNeighbour(col, -2)].enabled_ = 0;
      gM.board_[CheckSingleNeighbour(row, +2)][CheckSingleNeighbour(col, -1)].enabled_ = 0;
      gM.board_[CheckSingleNeighbour(row, +2)][col].enabled_ = 0;
      gM.board_[CheckSingleNeighbour(row, +2)][CheckSingleNeighbour(col, +1)].enabled_ = 0;

      *ship = 1;

    }

  }

  // Grass
  if(state == 4){

    if(aux_layer[row][col].enabled_ == 1 && rand()%10 == 5){

      aux_layer[row][col].type_ = 2 + rand()%4;
      aux_layer[row][col].enabled_ = 0;
      layer[row][col].enabled_ = 0;

      // Logic
      gM.board_[row][col].enabled_ = 0;

    }

  }

  // Lava
  if(state == 5){

    if(CheckNeighboursType(aux_layer, row, col, 3, 3, 5, 1) == 0){

      aux_layer[row][col].type_ = 13+rand()%2;
      aux_layer[row][col].enabled_ = 0;
      layer[row][col].enabled_ = 0;

      // Logic
      gM.board_[row][col].enabled_ = 0;

    }

    // Lava Cave
    if(layer[row][col].type_ == 0 && rand()%25 == 7 && aux_layer[row][col].enabled_ == 1){
    
      layer[row][col].enabled_ = 1;
      aux_layer[row][col].type_ = 15;
      aux_layer[row][col].enabled_ = 0;

      // Logic
      gM.board_[row][col].enabled_ = 0;
      gM.board_[row][col].enter_ = 0;
         
    }

  }

  // Random Chest
  if(layer[row][col].type_ == 0 && rand()%1000 == 21 && aux_layer[row][col].enabled_ == 1){

    unsigned char rnd_chest = rand()%100;

    if(rnd_chest <= 50){

      aux_layer[row][col].state_ = 9;
      aux_layer[row][col].type_ = 0;
      layer[row][col].enabled_ = 0;
      aux_layer[row][col].enabled_ = 0;

    }

    if(51 <= rnd_chest && rnd_chest <= 75){

      aux_layer[row][col].state_ = 9;
      aux_layer[row][col].type_ = 4;
      layer[row][col].enabled_ = 0;
      aux_layer[row][col].enabled_ = 0;

    }

    if(76 <= rnd_chest && rnd_chest <= 90){

      aux_layer[row][col].state_ = 9;
      aux_layer[row][col].type_ = 8;
      layer[row][col].enabled_ = 0;
      aux_layer[row][col].enabled_ = 0;

    }

    if(91 <= rnd_chest && rnd_chest <= 100){

      aux_layer[row][col].state_ = 9;
      aux_layer[row][col].type_ = 12;
      layer[row][col].enabled_ = 0;
      aux_layer[row][col].enabled_ = 0;

    }

    // Logic
    gM.board_[row][col].enabled_ = 0; 
    gM.board_[row][col].enter_ = 3;

  }

}

void CreateMap(){

  GameManager& gM = GameManager::Instantiate();
  
  signed int max_repeats = Board::kBoardSize*Board::kBoardSize;
  unsigned char rand_row;
  unsigned char rand_col;
  unsigned char gain;
  unsigned char sunny = 0;
  unsigned char shop = 0;
  
  while(repeats < (max_repeats * 1024)){
    
    rand_row = rand()%Board::kBoardSize;
    rand_col = rand()%Board::kBoardSize;
    
    gain = CheckNeighbours(gM.layer1_.map_, rand_row, rand_col,
                           3, 3, gM.layer1_.map_[rand_row][rand_col].state_);
    
      if(gain != 8 && gM.layer1_.map_[rand_row][rand_col].state_ != 0){ 

        PickCell(gM.layer1_.map_, rand_row, rand_col, gain);
      
      }

    ++repeats;
    
  }

  for(int n = 0; n < 5; ++n){

    for(unsigned char i = 0; i < Board::kBoardSize; ++i){

      for(unsigned char j = 0; j < Board::kBoardSize; ++j){

        EraseTile(gM.layer1_.map_, gM.layer2_.map_, i, j,
                  gM.layer1_.map_[i][j].state_);        

      }

    }

  }

  for(int i = 0; i < Board::kBoardSize; ++i){
    for(int j = 0; j < Board::kBoardSize; ++j){
      // printf("%d ", gM.layer2_.map_[i][j].enabled_);
      gM.layer2_.map_[i][j].state_ = gM.layer1_.map_[i][j].state_;
    }
    // printf("\n");
  }

  for(int n = 0; n < 1; ++n){

    for(unsigned char i = 0; i < Board::kBoardSize; ++i){

      for(unsigned char j = 0; j < Board::kBoardSize; ++j){        
        
        ChangeTileType(gM.layer1_.map_, gM.layer2_.map_, i, j,
                       gM.layer1_.map_[i][j].state_, &sunny, &shop);

      }

    }

  }
  
}

/*void DrawCharacter(SDL_Renderer *renderer){
 
  SDL_SetRenderDrawColor(renderer,0,255,0,255);
  
  SDL_Rect aux_rect = Character.info_body;
  
  aux_rect.x = Character.info_body.x * kWindowWidth/kNumCols + 4;
  aux_rect.y = Character.info_body.y * kWindowHeight/kNumRows + 4;
  
  SDL_RenderFillRect(renderer, &aux_rect);
  SDL_RenderDrawRect(renderer, &aux_rect);
  
}*/