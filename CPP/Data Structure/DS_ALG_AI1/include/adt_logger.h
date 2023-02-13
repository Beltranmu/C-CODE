// adt_logger.h
// Escuela Superior de Arte y Tecnologia
// Algoritmos & Inteligencia Artificial
// ESAT 2020-2021
//

#ifndef __ADT_LOGGER_H__
#define __ADT_LOGGER_H__

#include "adt_queue.h"

/**
 * @struct ADT Logger
 * @var storage_ store all the information you want to save.
 * @var *ops_ pointer to Logger Functions.
 */
typedef struct adt_logger_s {
  Queue *storage_;
	struct logger_ops_s *ops_;
} Logger;

/**
 * @struct Logger_Ops_s.
 * @brief Contains all the *ops functions.
 */
struct logger_ops_s {

  /**
   * @brief Destroys the logger and its data.
   * @param *logger to be destroyed.
   * @return - kErrorCode_Logger_Null when the logger to be destroyed is NULL.
             - kErrorCode_Queue_Null when the storage of the logger is NULL.
             - kErrorCode_List_Null when the queue's storage is NULL. Or if it is returned at the storage destruction.
             - kErrorCode_MemoryNode_Null When storage destroy function returned it.
             - kErrorCode_Ok Everething was done correctly.
   */
  s16(*destroy)(Logger *logger);

  /**
 * @brief Introduce a new data on the Logger.
 * @param *logger in which you are to introduce the data .
 * @param *text to introduce in the logger .
 * @return - kErrorCode_Logger_Null when the logger is NULL.
           - kErrorCode_Queue_Null  when the storage is NULL.
           - kErrorCode_List_Null when the storage of the queue is NULL.
           - kErrorCode_Data_Null if the data to insert is NULL.
           - kErrorCode_ZeroSize if size of the data to insert is 0.
           - kErrorCode_Queue_Full when the queue is full.
           - kErrorCode_List_Full  if the list is full.
           - kErrorCode_Ok Everething was done correctly.
 */
  s16(*write)(Logger *logger, char* text);

  /**
    * @brief Introduce a new data on the Logger.
    * @param *logger to be flushed.
    * @return - kErrorCode_Logger_Null when the logger is NULL.
              - kErrorCode_Queue_Null when the storage is NULL.
              - KErrorCode_Logger_Empty if the logger is empty.
              - kErrorCode_Ok Everething was done correctly.
  */

  s16(*flush)(Logger *logger);


};
/**
 * @brief Careates a new empty looger
 * @param capacity determines the capacity of the new logger
 * @return the new logger. Null if there is no memory available for the queue.
 */
Logger* LOGGER_create(u16 capacity);
#endif //__ADT_LOGGER_H__
