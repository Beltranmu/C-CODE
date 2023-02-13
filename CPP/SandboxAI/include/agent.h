/**
 * @brief Agent , Artificial intelligence demo , ESAT - 2022
 * @file agent.h
 * @author Ricardo Beltr�n Muriel <ricardobeltrandev@gmail.com>
 */


#ifndef __AGENT_H__
#define __AGENT_H__ 1

#include <stdint.h>
class Board;

//! 
//! Class Agent.
     /*! Agent for the demo */
class Agent {

  public:

    //! movement type Enum.
     /*! Represent the different types of movement and agent can have. */
  typedef enum{
    kMovement_Random = 0,   /*!< Random. */ 
    kMovement_Pattern,      /*!< Pattern. */ 
    kMovement_Track,        /*!< Tracking. */ 
    kMovement_None,         /*!< None. */ 
  } Movement;

  //! PAtternMovement Enum.
     /*! Represent the types of movement that can be done in the pattern movement. */
  typedef enum{ 
    kPatternMovement_None = -1,       /*!< None. */
    kPatternMovement_Wait,            /*!< Wait. */
    kPatternMovement_Forward,         /*!< Go forward. */
    kPatternMovement_Turn180,         /*!< Turn 180. */
    kPatternMovement_TurnLeft90,      /*!< Turn left 90. */
    kPatternMovement_TurnRight90,     /*!< Turn right 90. */
  }PatternMovement;

    Agent();
    ~Agent();

    /**
    *@brief Init the agent.
    */
    void init();

    /**
    *@brief Move the agent.
    *@param Board* board. The board you want to move the agent in
    */
    void moveUnit(Board* board);

    /**
    *@brief Init the agent.
    *@param bool* canIMove. return true if the agent can move otherwise, false.
    *@return the next movement of an agent. 0-> North, 1-> South, 2-> Right, 3-> Left
    */
    int randomMov(bool* canIMove);

    /**
    *@brief Init the agent.
    *@param bool* canIMove. return true if the agent can move otherwise, false.
    *@return the next movement of an agent. 0-> North, 1-> South, 2-> Right, 3-> Left
    */
    int patternMov(bool* canIMove);

    /**
    *@brief Init the agent.
    *@param bool* canIMove. return true if the agent can move otherwise, false.
    *@param Board* board. The board you want to move the agent in
    *@return the next movement of an agent. 0-> North, 1-> South, 2-> Right, 3-> Left
    */
    int pacManMovement(bool* canIMove, Board* board);

    /**
    *@brief Add a movement to the pattern
    *@param   PatternMovement p_m type of movement.
    *@param    int32_t n_m amount of movement.
    */
    void addMovement2Patern(PatternMovement p_m, int32_t n_m);

    /**
    *@brief Clean the pattern of an agent.
    */
    void cleanMovementPatern();

    static const int32_t numMovements = 64;        ///< Max number of movement of the pattern
    PatternMovement movementArray[numMovements];   ///< Type of movement that an agent have in its pattern
    int32_t movementCounterArray[numMovements];    ///< Number of movement that an agent have in its pattern
    int32_t i_movement;                            ///< index of the movement
    int32_t i_counterMov;                          ///< counter of the number of movement an agent already has done
    int32_t next_mov;                              ///< Represent the next movement of an agent. 0-> North, 1-> South, 2-> Right, 3-> Left
                         

    int32_t currentForwardX;                       ///< The forward of the agent.
    int32_t currentForwardY;                       ///< The forward of the agent.
                                                   
    int32_t currentPos;                            ///< Cell id of the current position
    int32_t currentTarget;                         ///< Cell id of the current target
    Movement movementType;                         ///< Type of movement the agent has
    uint32_t distanceToCatch;                      ///< Distance of reaction 
    uint32_t speed;                                ///< Speed 
    bool been_chased;                              ///< If this agent is been chased 
                                                  
    int32_t index_movementArray;                   ///< Index of the movement for the pattern movement.
                                                   
    uint32_t agentID;                              ///< ID of the agent
    bool agentSelected;                            ///< true if this agent is selected otherwise false,

};

#endif //!__AGENT_H__