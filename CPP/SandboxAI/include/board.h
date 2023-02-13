/**
 * @brief Board , Artificial intelligence demo , ESAT - 2022
 * @file board.h
 * @author Ricardo Beltr�n Muriel <ricardobeltrandev@gmail.com>
 */



#ifndef __BOARD_H__
#define __BOARD_H__ 1

#include <stdint.h>
#include "SFML/Graphics.hpp"
#include "agent.h"
#include "astar.h"

const int kBoardMaxSize = 128;    ///<  Max size of the board.
const int kBoardMinSize = 16;     ///<  Min size of the board.
const int kBoardMaxUnits = 10;    ///<  Max number of agents in the board.


//! TileType Enum.
     /*! Type of the cells of the board. */
enum TileType {
  //Non-Walkable
  kTileType_Void = 0, /*!< Void cell. */ 
  kTileType_Wall,     /*!< Wall cell. */ 

  //Walkable
  kTileType_Normal,   /*!< Normal cell. */ 
  kTileType_Door,     /*!< Door cell. */ 
  kTileType_End,      /*!< MAX */ 
};

//! Cell struct.
/*! Type of the cells of the board. */
struct Cell{
  int value;    /*!< Type of the cell cell. */ 
  int height;   /*!< Height of the cell. */ 
};



//! Board class.
     /*! Represent a board, where units can move . */
class Board
{
  public:

    Board();
    ~Board();
   

    /**
      *@brief Create the board.
      *@param int width = kBoardMaxSize/4. Width of the board. default value kBoardMaxSize/4.
      *@param int height = kBoardMaxSize/4. Width of the board. default value kBoardMaxSize/4.
     */
    void initBoard(int width = kBoardMaxSize/4, int height = kBoardMaxSize / 4);

    /**
      *@brief Inits the Units of the board.
    */
    void initUnits();
    
    /**
      *@brief calculate the row and column of a cell from its id.
      *@param int *row . Variable to return the row
      *@param int *col . Variable to return the column
      *@param int id. Id of the cell.
    */
    void index2RowCol(int *row, int *col, int id);

    /**
     *@brief calculate the row and column of a cell from its id.
     *@param int row . Row of the cell
     *@param int col . Column of the cell
     *@param int *id. Variable to return the cell id.
   */
    void rowcol2Index(int  row, int  col, int *id);

    /**
     *@brief return the cell giving its row and column
     *@param int row . Variable to return the row
     *@param int col . Variable to return the column
     *@return the cell of that row and col.
   */
    Cell& cell(int row, int col);
    /**
     *@brief return the id of the cell to the north of the giving one
     *@param int id. Id of the cell you want to have as reference
     *@return the id cell 
   */
    int north(int id);
    /**
    *@brief return the id of the cell to the south of the giving one
    *@param int id. Id of the cell you want to have as reference
    *@return the id cell
    */
    int south(int id);
    /**
    *@brief return the id of the cell to the west of the giving one
    *@param int id. Id of the cell you want to have as reference
    *@return the id cell
    */
    int west(int id);
    /**
    *@brief return the id of the cell to the east of the giving one
    *@param int id. Id of the cell you want to have as reference
    *@return the id cell
    */
    int east(int id);

    /**
    *@brief return the id of the cell that is walkable
    *@return the id cell
    */
    int randomWalkableCell();

    //Return true if the type of the cell is the same that the one you give

    /**
    *@brief Return true if the type of the cell is the same that the one you give
    *@param int id. Id of the cell you want to have as reference.
    *@param int cell_type. Type of cell you want to compare.
    *@return true or false;
    */
    uint8_t cellType(int id, int cell_type);

    /**
    *@brief check if the cell you are going to move is possible to move to
    *@param int id_end_cell. The id cell to compare.
    *@return true if is possible to move and false otherwise
    */
    bool checkUnitMovement(int id_end_cell);
    /**
    *@brief check if the next movement is possible and move the agent.
    *@param Agent* a. Agent to move.
    *@param int id_end_cell. The id cell to compare.
    */
    void checkAndMove(Agent* a, int id_end_cell);

    /**
    *@brief check if the agent is close to the an agent.
    *@param uint32_t playerToSearch. ID of the player to check
    *@param Agent* e. Agent you want to change their state to chase.
    */
    void checkForPlayer(uint32_t playerToSearch, Agent* e); ///> true if there is a player near otherwise false
    /**
   *@brief kill a unit.
   *@param int target_idx. id of the unit to kill.
   */
    void killUnit(int target_idx);


    /**
   *@brief Move the movement of all agents
   */
    void unitMovement();

    /**
    *@brief Move randomly all the agents that have random move activated .
    */
    void randomMove();
    /**
    *@brief Calculate the pac man move. Calculate the direction of the next move.
    *@param int32_t org_cell. Current cell of the agent
    *@param int32_t target_cell. Target cell of the agent
    *@param int32_t* fwX. Forward.x of the component.
    *@param int32_t* fwY. Forward.y of the component.
    *@return -1 if the agent won't move otherweise yes.
    */
    int pacmanMovement(int32_t org_cell, int32_t target_cell, int32_t* fwX, int32_t* fwY);

    /**
    *@brief draw the logical board
    *@param sf::RenderWindow* window. SFML window.
    */
    void drawLBoard(sf::RenderWindow* window);
    /**
    *@brief draw the logical board
    *@param sf::RenderWindow* window. SFML window.
    *@param int selected_cell. target row to draw.
    *@param  int mode. Mode of draw, in path finding mode, agent are not shown.
    */
    void drawBoard(sf::RenderWindow* window, int selected_cell, int mode);



    /**
    *@brief Calculate the manhattan distance. Between two cells
    *@param int32_t origin_cell. First cell
    *@param int32_t dst_cell. Second cell.
    *@param  The distance.
    */
    uint32_t manhantanDistance(int32_t origin_cell, int32_t dst_cell);
    /**
    *@brief Calculate the euclidean distance. Between two cells
    *@param int32_t origin_cell. First cell
    *@param int32_t dst_cell. Second cell.
    *@param  The distance.
    */
    float euclidianDistance(int32_t origin_cell, int32_t dst_cell);
    /**
    *@brief Calculate the chebyshov distance. Between two cells
    *@param int32_t origin_cell. First cell
    *@param int32_t dst_cell. Second cell.
    *@param  The distance.
    */
    uint32_t chebyshovDistance(int32_t origin_cell, int32_t dst_cell);


    /**
    *@brief Get the agent id of the agent clicked
    *@param sf::RenderWindow& window. SFML window
    *@return  the id of the agent.
    */
    int32_t getAgent(sf::RenderWindow& window);

    Cell cell_[kBoardMaxSize * kBoardMaxSize];    ///< Array of each cell of the board
    Agent units_[kBoardMaxSize];                  ///< Array of the units

    int32_t treasureLocation;                     ///< ID of the cell where is the treasure.

    Astar aPath_;                                 ///< Component of A* to calculate the path

    int width_;                                   ///< Width of the board
    int height_;                                  ///< Height of the board

    int width_tile_;                              ///< width of each tile
    int height_tile_;                             ///< height of each tile
    int desp_x_tile_;                             ///< displacement X of each tile
    int desp_y_tile_;                             ///< displacement Y of each tile



    int32_t targetRowI;                           ///< Row of the init target cell 
    int32_t targetColI;                           ///< Column of the init target cell 
    int32_t targetRowD;                           ///< Row of the destination target cell 
    int32_t targetColD;                           ///< Column of the destination target cell 

    bool drawLogical;                             ///< true to draw the logical draw. otherwise not


    sf::Texture mt;                               ///< map texture
    sf::Sprite board_sprite;                      ///< map sprite

    sf::Texture agent_t_;                          ///< agent texture
    sf::Sprite agent_s_;                           ///< agent sprite
    sf::Texture agent_t2_;                         ///< agent 2 texture
    sf::Sprite agent_not_selected_;                ///< agent 2 sprite

    sf::Texture player_t;                          ///< player texture
    sf::Sprite player_s;                           ///< player sprite

    sf::Texture goal_t;                            ///< goal tile texture
    sf::Sprite goal_s;                             ///< goal tile sprite

    sf::Texture start_t;                           ///< start tile texture
    sf::Sprite start_s;                            ///< start tile sprite

  protected:
   
  private:
};

#endif //__BOARD_H__