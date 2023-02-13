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
  kErrorCode_MemoryNode_Null = -10,
  kErrorCode_File = -20,
  kErrorCode_ZeroSize = -30,
  kErrorCode_Data_Null = -40,

  kErrorCode_Vector_Null = -50,
  kErrorCode_Vector_Full = -60,
  kErrorCode_NonFuction = -70,

  kErrorCode_VectorC_Null = -80,
  kErrorCode_VectorMH_Null = -90,

  kErrorCode_List_Null = -100,
  kErrorCode_List_Full = -110,
  kErrorCode_List_Empty = -120,

  kErrorCode_Stack_Null = -130,
  kErrorCode_Stack_Full = -140,

  kErrorCode_Queue_Null = -150,
  kErrorCode_Queue_Full = -160,
  kErrorCode_Queue_Empty = -170,

  kErrorCode_Logger_Null = -180,
  KErrorCode_Logger_Empty = -190,
  
  KErrorCode_No_Memory = -200,


} ErrorCode;




#endif // __COMMON_DEF_H__