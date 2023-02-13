/**
 * @brief B_Loader , Artificial intelligence demo , ESAT - 2022
 * @file b_loader.h
 * @author Ricardo Beltr�n Muriel <ricardobeltrandev@gmail.com>
 */




#ifndef __B_LOADER_H__
#define __B_LOADER_H__ 1

#include "SFML/Graphics.hpp"
#include "board.h"

 //! Error Type Enum.
      /*! Type of the cells of the board. */
typedef enum {
  kErrorCode_SRCNullPointer,       /*!< File name null. */
  kErrorCode_DataNullPointer,      /*!< Board null. */
  kErrorCode_File,                 /*!< Error to read file. */
  kErrorCode_OKAY                  /*!< OK. */ 
} ErrorCode;

/**
 *@brief load a board read from a image.
 *@param Board* board where to load all the information.
 *@param const char* filename. path of the image to read to load the logical board.
 *@return error code kErrorCode_SRCNullPointer if the filename is nullptr, kErrorCode_DataNullPointer if the board is nullptr,
 * kErrorCode_File if there is a problem reading the file.kErrorCode_OKAY -> Everything okay.
 */

static ErrorCode BoardFromImage(Board* board, const char* filename)  {

  if (nullptr == filename) return kErrorCode_SRCNullPointer;
  if (nullptr == board) return kErrorCode_DataNullPointer;

  sf::Image boardImage;
 
  if (!boardImage.loadFromFile(filename)) return kErrorCode_File;

  sf::Vector2u imageSize = boardImage.getSize();
  sf::Color pixelColor(0);

  int t_width = imageSize.x;
  int t_height = imageSize.y;

  board->initBoard(t_width, t_height);

  for (int col = 0; col < t_width; ++col) {
    for (int row = 0; row < t_height; ++row) {

      pixelColor = boardImage.getPixel(col, row);     

      //White
      if (pixelColor == sf::Color::White){
        board->cell(row, col).value = kTileType_Normal;
      }
      //Black
      else if (pixelColor == sf::Color::Black) {
        board->cell(row, col).value = kTileType_Wall;
      }
      else {
        board->cell(row, col).value = kTileType_Wall;
      }
    }
  }
  board->initUnits();
  return kErrorCode_OKAY;
}

#endif //__B_LOADER_H__