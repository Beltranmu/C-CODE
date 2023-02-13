/**
 * @brief Astar Class, Artificial intelligence demo , ESAT - 2022
 * @file astar.h
 * @author Ricardo Beltr�n Muriel <ricardobeltrandev@gmail.com>
 */



#ifndef __ASTAR_H__
#define __ASTAR_H__ 1

#include <stdint.h>
#include <list>
#include <vector>

#include "SFML/Graphics.hpp"

class Board;

//! TypeDistanceUsedToCalculatePath Enum.
     /*! Each diferent distance to calculate A*. */
enum TypeDistanceUsedToCalculatePath{
  MANHATTAN_TYPE = 0,     /*!< Manhattan distance. */ 
  EUCLIDEAN_TYPE,         /*!< Euclidean distance. */ 
  CHEBYSHOV_TYPE,         /*!< Chebyshov distance. */ 
  MAX_TYPE
};


//! Cell struct.
     /*! Each cell of the board. It is used to store all the information of the cells to calculate A* */
struct ACell{
  uint32_t cellID;        /*!< ID of the cell. */ 
  uint32_t parentCellID;  /*!< Parent ID cell. Same cellID and parentCellID -> init cell */ 
  float score;            /*!< Total score of the cell H = F + G. */ 
  float g;                /*!< G score. */ 
};

//! Path struct.
     /*! Representation of a Path. Used to be draw an to store the path */
struct TPath{
  std::vector<ACell> path;        /*!< Vector of cells that sorted and are the path. */
  uint32_t origin;                /*!< ID of the origin cell. */
  uint32_t destination;           /*!< ID of the destination cell. */
  bool draw = false;              /*!< If you want to show the path. True is shown otherwise not */
  //bool diagonal = false;          /*!< . */
  TypeDistanceUsedToCalculatePath type;   /*!< the type of the distance it has been used to calculate the path. */
};


/**
  *@brief Astar class. used to calculate the A*.
*/
class Astar{
public:

  Astar();
  /**
    *@brief calculate the path with a origin and a destination.
    *@param Board* board. In which board you want to calculate the A*.
    *@param int initPostition. Id of the initial cell.
    *@param int int endPosition. Id of the destination cell.
    */
  void calculatePath(Board* board, int initPostition, int endPosition);

  std::list<ACell> openList;         ///<  Open list of the A* algorithm. 
  std::list<ACell> closeList;        ///< Closed list of the A* algorithm. 
  std::vector<TPath> currentPaths;   ///< Vector to store all the already calculated path.


  sf::Color pathColors[MAX_TYPE];   ///< Array of color. Store a different color from each type of distance. 

  bool manhattanD;              ///< True If the path you want to calculate is using manhattan distance, otherwise not. 
  bool euclideanD;              ///< True If the path you want to calculate is using euclidean distance, otherwise not. 
  bool chebyshovD;              ///< True If the path you want to calculate is using chebyshov distance, otherwise not. 
  //bool allowDiagonals;          ///< Array of color. Store a different color from each type of distance. 

};



#endif //__ASTAR_H__