// adt_queue.h
// Escuela Superior de Arte y Tecnologia
// Algoritmos & Inteligencia Artificial
// ESAT 2020-2021
//

#ifndef __ADT_QUEUE_H__
#define __ADT_QUEUE_H__

#include "adt_list.h"

/**
 * @struct ADT Queue
 * @var *storage_ pointer to a List that will contain the data.
 * @var *ops_ pointer to Queue Functions.
 */
typedef struct adt_queue_s {
  List * storage_;
	struct queue_ops_s *ops_;
} Queue;

/**
 * @struct Queue_Ops_s.
 * @brief Contains all the *ops functions.
 */
struct queue_ops_s {
  
  /**
   * @brief Destroys the queue and its data.
   * @param *queue to be destroyed.
   * @return - kErrorCode_Queue_Null when the queue to be destroyed is NULL.
             - kErrorCode_List_Null when the queue's storage is NULL. Or if it is returned at the storage destruction.
             - kErrorCode_MemoryNode_Null When storage destroy function returned it.
             - kErrorCode_Ok Everething was done correctly.
   */

	s16 (*destroy)(Queue *queue);

  /**
   * @brief Soft-resets each queue's MemoryNode.
   * @param *queue to be soft-reset.
   * @return - kErrorCode_Queue_Null when the queue to be softreset is NULL.
             - kErrorCode_List_Null when the storage to be soft-reset is NULL or the first node of it is NULL.
             - kErrorCode_MemoryNode_Null if the first node is NULL.
             - kErrorCode_List_Empty if the list is empty.
             - kErrorCode_Ok Everething was done correctly. 
   */
  s16 (*softReset)(Queue *queue);

  /**
   * @brief Resets each queue's MemoryNode.
   * @param *queue to be reset.
   * @return - kErrorCode_Queue_Null when the queue to be softreset is NULL.
             - kErrorCode_List_Null when the storage to be soft-reset is NULL or the first node of it is NULL.
             - kErrorCode_MemoryNode_Null if the first node is NULL.
             - kErrorCode_List_Empty if the list is empty.
             - kErrorCode_Ok Everething was done correctly. 
   */
  s16 (*reset)(Queue *queue);
  
  /**
   * @brief Modifies the queue's length.
   * @param *queue to apply the resize.
   * @param new_size determines the new size for the queue.
   * @return - kErrorCode_Queue_Null when the queue to be resized is NULL.
             - kErrorCode_List_Null if the storage of the queue is NULL. 
             - kErrorCode_ZeroSize if the new size is less or equal 0.
             - kErrorCode_Ok Everething was done correctly.
   */
	s16 (*resize)(Queue *queue, u16 new_size);


	// State queries

  /**
   * @brief Returns the amount of elementes to store in the queue.
   * @param *queue to get its capacity.
   * @return the maximum number of nodes of the queue.
   */
	u16 (*capacity)(Queue *queue);

  /**
   * @brief Returns the length of the queue.
   * @param *queue to get its length.
   * @return number of nodes that are used in the queue.
   */
	u16 (*length)(Queue *queue);

  /**
   * @brief Checks whether the queue is empty or not.
   * @param *queue to check
   * @return true == emtpy, otherwise == false.
   */
	bool (*isEmpty)(Queue *queue);

  /**
   * @brief Checks whether the queue is full or not.
   * @param *queue to check
   * @return true == full, otherwise == false.
   */
	bool (*isFull)(Queue *queue);

	// Data queries
  
  /**
   * @brief Returns the data of the first node of the queue.
   * @param *queue receives the queue.
   * @return the data of the first node in the queue. Return NULL if: the queue is empty, the storage of the queue is empty.
   */
  void* (*front)(Queue *queue);

  /**
   * @brief returns the data of the last node of the queue.
   * @param *queue receives the queue.
   * @return the data of the last node in the queue. Return NULL if: the queue is empty, the storage of the queue is empty.
   */
  void* (*back)(Queue *queue);

	// Insertion  


  /**
   * @brief Inserts data in the last node of the queue.
   * @param *queue in which you want to insert the data.
   * @param *data data to insert.
   * @param bytes size of the data.
   * @return - kErrorCode_Queue_Null when the queue is NULL.
             - kErrorCode_Queue_Full when the queue is full.
             - kErrorCode_List_Null when the storage of the queue is NULL.
             - kErrorCode_Data_Null if the data to insert is NULL.
             - kErrorCode_List_Full if the list is full.
             - KErrorCode_No_Memory if there is no memory available for a new node.
             - kErrorCode_ZeroSize if size of the data to insert is 0.
             - kErrorCode_Ok Everething was done correctly.
   */
  s16 (*enqueue)(Queue *queue, void *data, u16 bytes);

  
	// Extraction

  /**
   * @brief Extracts the data of the first node of the queue that is not null.
   * @param *queue to extract the data from.
   * @return the data of that node. Return NULL if: the queue is NULL, the storage of the queue is NULL.
   */
	void* (*dequeue)(Queue *queue);


	// Miscellaneous
  
  /**
   * @brief Concats the information of two queues.
   * @param *queue First queue to concat and where the result is going to be.
   * @param *queue_src Second queue to concat.
   * @return - kErrorCode_Queue_Null One of the two or both queue to concat is NULL.
             - kErrorCode_List_Null One of the two or both queue's storage to concat is NULL.
             - kErrorCode_MemoryNode_Null if the firs node of list is NULL. 
             - KErrorCode_No_Memory if there is no memory available for a new block of MemoryNode.
             - kErrorCode_Ok Everething was done correctly.
   */
	s16 (*concat)(Queue *queue, Queue *queue_src);

  /**
   * @brief Prints the information of the entire queue.
   * @param *queue to print its information.
   */
   
	void (*print)(Queue * queue); // Prints the features and content of the queue
};

/**
 * @brief Careates a new empty queue
 * @param capacity determines the capacity of the new queue
 * @return the empty queue. Null if there is no memory available for the queue.
 */
Queue* QUEUE_create(u16 capacity);
#endif //__ADT_QUEUE_H__
