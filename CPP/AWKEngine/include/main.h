
/************************************************
 * Main EntryPoint
 * Developed by Ricardo BM & Javier BA
 * Graphic Engine Development
 * ESAT - 2021/2022
 ***********************************************/

#ifndef __MAIN_H__
#define __MAIN_H__ 1

#include <stdint.h>
namespace AWK {
  //! EntryPoint for Windows
    /*!
      \param argc. Number of initial params.
      \param argv. The params.
      \return 0 everything okay. Otherwise, there are problems
    */
  int awakenedMain(int argc, char **argv);
 
}

#endif // !__MAIN_H__