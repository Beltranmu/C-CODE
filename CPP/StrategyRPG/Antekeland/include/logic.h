/**
  * @file main.cc
  * @brief Main file of the game.
  * @details This is the core file. It gathers all classes of the game to make it possible.
  * @author Javier Benito Abolafio <benitoab@esat-alumni.com>
  * @version alfa 1.0
  * @date Ded-2020
  * @copyright ESAT
  */
 

#ifndef __LOGIC_H__
#define __LOGIC_H__ 1

#include <stdint.h>

class Logic{
  public:
    // Methods
    Logic();
    ~Logic();
    /**
     *@brief Init the logic
     */
    void init();

    // Attributes
    uint8_t enabled_; ///@var used to know if you can walk == 1 or not
    uint8_t enter_; ///@var used to know if you are in enter of a cave /

};

#endif  // __LOGIC_H__