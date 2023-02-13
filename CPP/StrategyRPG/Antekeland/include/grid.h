/**
  * @file main.cc
  * @brief Main file of the game.
  * @details This is the core file. It gathers all classes of the game to make it possible.
  * @author Javier Benito Abolafio <benitoab@esat-alumni.com> 
  * @version alfa 1.0
  * @date Ded-2020
  * @copyright ESAT
  */
 
#ifndef __GRID_H__
#define __GRID_H__ 1

#include "gamemanager.h"

const int kChanceToMove = 721; ///@var the chance to change a tile in his generation
const unsigned char kSearchRange = (Board::Board::kBoardSize/4)*3;  ///@var the range for searching a random tile
static int repeats = 0; ///@var count the number of times that you complete a stage in porcedural generation

/**
 *@brief create and init the graphic board
*/
void CreateBoard(); 
/**
 *@brief init the logic board
*/
void InitLogic();
  /** 
   *@brief Tells you the next position after a desp of the board
   *@param unsigned char pos, your current position(x or y, but not both)
   *@param  const int desp how many tiles you want to move
   *@return the next position 
  */ 
unsigned char CheckSingleNeighbour(unsigned char pos, const int desp);

unsigned char MatrixNxM(const Tile layer[Board::kBoardSize][Board::kBoardSize],
                      const unsigned char row, const unsigned char col, const unsigned char n,
                      const unsigned char m, const unsigned char state);
unsigned char CheckNeighbours(const Tile layer[Board::kBoardSize][Board::kBoardSize],
                              const unsigned char row, const unsigned char col, const unsigned char n,
                              const unsigned char m, const unsigned char state);
unsigned char CheckNeighboursType(const Tile layer[Board::kBoardSize][Board::kBoardSize],
                                  unsigned char row, unsigned char col, const unsigned char n,
                                  const unsigned char m, unsigned char state,
                                  unsigned char check_type);
unsigned char CheckCrossNeighbours(const Tile layer[Board::kBoardSize][Board::kBoardSize],
                                   unsigned char row, unsigned char col, unsigned char state);
void SwapCells(Tile layer[Board::kBoardSize][Board::kBoardSize],
               unsigned char row1, unsigned char col1, unsigned char state1,
               unsigned char row2, unsigned char col2, unsigned char state2);
signed char TemporalSwap(Tile layer[Board::kBoardSize][Board::kBoardSize],
                         unsigned char row1, unsigned char col1, unsigned char state1,
                         unsigned char row2, unsigned char col2, unsigned char state2);
signed char CalculateGain(Tile layer[Board::kBoardSize][Board::kBoardSize],
                          unsigned char row1, unsigned char col1,
                          unsigned char row2, unsigned char col2);                                                                             
void EraseTile(Tile layer[Board::kBoardSize][Board::kBoardSize],
               Tile aux_layer[Board::kBoardSize][Board::kBoardSize],
               unsigned char row, unsigned char col, unsigned char state);
void PickCell(Tile layer[Board::kBoardSize][Board::kBoardSize],
              const unsigned char original_row, const unsigned char original_col,
              const unsigned char orig_gain);
void ChangeTileType(Tile layer[Board::kBoardSize][Board::kBoardSize],
                    Tile aux_layer[Board::kBoardSize][Board::kBoardSize],
                    unsigned char row, unsigned char col, unsigned char state,
                    unsigned char* ship, unsigned char* shop);
void CreateMap();                                                                                                                             

#endif  // __GRID_H__