// adt_queue.h
// Escuela Superior de Arte y Tecnologia
// Algoritmos & Inteligencia Artificial
// ESAT 2020-2021
//

#ifndef __ADT_QUEUE_H__
#define __ADT_QUEUE_H__

#include "adt_list.h"


class Queue
{
public:
  Queue();
  Queue(Queue &copyQueue);
  Queue& operator= (const Queue & queue);
  ~Queue();

  /**
   * @brief Create an empty container for the queue.
   * @param capacity determines the capacity the container.
   * @return - if the queue to be initiated is NULL.
   - kErrorCode_ZeroSize when the capacity is <= 0.
             - KErrorCode_No_Memory when there is no memory available for the nodes of the list.
             - kErrorCode_Ok Everething was done correctly.
  */
  s16 init(u16 capacity);

  /**
   * @brief Soft-resets each queue's MemoryNode.
   * @return - kErrorCode_Queue_Null when the queue to be softreset is NULL.
             - kErrorCode_List_Null when the storage to be soft-reset is NULL or the first node of it is NULL.
             - kErrorCode_MemoryNode_Null if the first node is NULL.
             - kErrorCode_List_Empty if the list is empty.
             - kErrorCode_Ok Everething was done correctly. 
  */
  s16 softReset();

  /**
   * @brief Resets each queue's MemoryNode.
   * @return - kErrorCode_Queue_Null when the queue to be softreset is NULL.
             - kErrorCode_List_Null when the storage to be soft-reset is NULL or the first node of it is NULL.
             - kErrorCode_MemoryNode_Null if the first node is NULL.
             - kErrorCode_List_Empty if the list is empty.
             - kErrorCode_Ok Everething was done correctly. 
  */
  s16 reset();

  /**
   * @brief Modifies the queue's length.
   * @param new_size determines the new size for the queue.
   * @return - kErrorCode_Queue_Null when the queue to be resized is NULL.
             - kErrorCode_List_Null if the storage of the queue is NULL. 
             - kErrorCode_ZeroSize if the new size is less or equal 0.
             - kErrorCode_Ok Everething was done correctly.
  */
  s16 resize(u16 new_size);


  // State queries
  /**
   * @brief Returns the amount of elementes to store in the queue.
   * @return the maximum number of nodes of the queue.
   */
  u16 capacity ();

  /**
   * @brief Returns the length of the queue.
   * @return number of nodes that are used in the queue.
  */
  u16 length();

  /**
   * @brief Checks whether the queue is empty or not.
   * @return true == emtpy, otherwise == false.
   */
  bool isEmpty();

  /**
   * @brief Checks whether the queue is full or not.
   * @return true == full, otherwise == false.
   */
  bool isFull();

  // Data queries

  /**
   * @brief Returns the first data of the queue
   * @return the data of the first node in the queue. Return NULL if: the queue is empty, the storage of the queue is empty.
   */
  void* front();

  /**
   * @brief Returns the last  data of the queue
   * @return the data of the first node in the queue. Return NULL if: the queue is empty, the storage of the queue is empty.
   */
  void* back();

  // Insertion  
  /**
   * @brief Inserts data in the last node of the queue.
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
  s16 enqueue(void *data, u16 bytes);


  // Extraction
  /**
   * @brief Extracts the first data of the queue.
   * @return the data of that node. Return NULL if: the queue is NULL, the storage of the queue is NULL.
   */
  void* dequeue();


  // Miscellaneous
  /**
   * @brief Concats the information of two queue.
   * @param *queue_src queue to concat.
   * @return - kErrorCode_Queue_Null One of the two or both queue to concat is NULL.
             - kErrorCode_List_Null One of the two or both queue's storage to concat is NULL.
             - kErrorCode_MemoryNode_Null if the firs node of list is NULL. 
             - KErrorCode_No_Memory if there is no memory available for a new block of MemoryNode.
             - kErrorCode_Ok Everething was done correctly.
   */
  s16 concat(Queue *queue_src);

  /**
   * @brief Prints the information of the entire vector.
   */

  void print(); // Prints the features and content of the vector


  protected:

  List * storage_;  ///@var *storage_ pointer to a List that will contain the datas.
  private:

};




#endif //__ADT_VECTOR_H__*/
