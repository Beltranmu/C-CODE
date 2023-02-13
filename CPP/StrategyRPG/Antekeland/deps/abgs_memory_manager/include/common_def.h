// common_def.h
// Toni Barella, Ivan Sancho
// Algoritmos & Inteligencia Artificial
// ESAT 2020-2021
//

#ifndef __COMMON_DEF_H__
#define __COMMON_DEF_H__

#define VERBOSE_

typedef enum
{
  kErrorCode_Ok = 0,
  kErrorCode_Memory = -10,
  kErrorCode_File = -20,
  kErrorCode_ZeroSize = -30,
  kErrorCode_Data = -40,

  kErrorCode_Vector_Null = -50,
  kErrorCode_VectorFull = -60,
  kErrorCode_NonFuction = -70,

} ErrorCode;




#endif // __COMMON_DEF_H__