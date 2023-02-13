// adt_list.h
// Escuela Superior de Arte y Tecnologia
// Algoritmos & Inteligencia Artificial
// ESAT 2020-2021
//

#ifndef __ADT_LIST_H__
#define __ADT_LIST_H__

#include"adt_memory_node.h"


class List{
public:
  List();
  List(List& copylist);
  List& operator= (const List& list);
  ~List();



  /**
   * @brief Careates a new empty list
   * @param capacity determines the capacity of the new list
   * @return - kErrorCode_List_Null the list to be initiated is NULL.
             - kErrorCode_ZeroSize when the capacity is <= 0.
             - KErrorCode_No_Memory when there is no memory available for the nodes of the list.
             - kErrorCode_Ok Everething was done correctly.
  */

  s16 init(u16 capacity);

  /**
   * @brief Soft-resets each node of the list
   * @return - kErrorCode_List_Null when the list to be soft-reset is NULL.
             - kErrorCode_MemoryNode_Null if the first node is NULL.
             - kErrorCode_List_Empty if the list is empty.
             - kErrorCode_Ok Everething was done correctly.
  */
  s16 softReset();


  /**
   * @brief Resets each node of the list
   * @return - kErrorCode_List_Null when the list to be reset is NULL.
             - kErrorCode_MemoryNode_Null if the first node is NULL.
             - kErrorCode_List_Empty if the list is empty.
             - kErrorCode_Ok Everething was done correctly.
  */
  s16 reset();

  /**
   * @brief Modifies the list's length.
   * @param new_size determines the new size for the list.
   * @return - kErrorCode_List_Null if the list to be resized is NULL.
             - kErrorCode_ZeroSize if the new size is less or equal 0.
             - kErrorCode_Ok Everething was done correctly.
   */
  s16 resize(u16 new_size);


  // State queries

  /**
   * @brief Returns the amount of elementes to store in the list.
   * @return the maximum number of nodes of the list.
   */
  u16 capacity();

  /**
   * @brief Returns the length of the list.
   * @return number of nodes that are used in the list.
   */
  u16 length();

  /**
   * @brief Checks whether the list is empty or not.
   * @return true == emtpy, otherwise == false.
   */
  bool isEmpty();

  /**
   * @brief Checks whether the list is full or not.
   * @return true == full, otherwise == false.
   */
  bool isFull();


  // Data queries
  /**
   * @brief Returns the data of the first node of the list.
   * @return the data of the first node in the list, NULL if the list is NULL, is empty or there is no nodes in the list.
   */
  void* first();

  /**
   * @brief returns the data of the last node of the list.
   * @return the data of the last node in the list, NULL if the list is NULL, is empty or there is no nodes in the list.
   */
  void* last();

  /**
   * @brief Returns the data of an spcecific node of the list.
   * @param position determines the position where the data will be read.
   * @return the data of an specific node, NULL if the list is NULL, is empty, there is no nodes in the list or position > list->length.
   */
  void* at(u16 position);


  // Insertion  
  /**
   * @brief Inserts data in the first node of the list.
   * @param *data data to insert.
   * @param bytes size of the data.
   * @return - kErrorCode_List_Null if the list is NULL.
             - kErrorCode_Data_Null if the data to insert is NULL.
             - kErrorCode_List_Full if the list is full.
             - KErrorCode_No_Memory if there is no memory available for a new node.
             - kErrorCode_ZeroSize if size of the data to insert is 0.
             - kErrorCode_Ok Everething was done correctly.
   */
  s16 insertFirst (void *data, u16 bytes);

  /**
   * @brief Inserts data in the last node of the list.
   * @param *vector in which you want to insert the data.
   * @param *data data to insert.
   * @param bytes size of the data.
   * @return - kErrorCode_List_Null if the list is NULL.
             - kErrorCode_Data_Null if the data to insert is NULL.
             - kErrorCode_List_Full if the list is full.
             - KErrorCode_No_Memory if there is no memory available for a new node.
             - kErrorCode_ZeroSize if size of the data to insert is 0.
             - kErrorCode_Ok Everething was done correctly.
   */
  s16 insertLast(void *data, u16 bytes);

  /**
   * @brief Inserts data in a specific node of the list.
   * @param *data data to insert.
   * @param bytes size of the data.
   * @param position determines the position where the data will be inserted.
   * @return - kErrorCode_List_Null if the list is NULL.
             - kErrorCode_Data_Null if the data to insert is NULL.
             - kErrorCode_List_Full if the list is full.
             - KErrorCode_No_Memory if there is no memory available for a new node.
             - kErrorCode_ZeroSize if size of the data to insert is 0.
             - kErrorCode_Ok Everething was done correctly.
   */
  s16 insertAt(void *data, u16 bytes, u16 position);


  // Extraction

  /**
   * @brief Extracts the data of the first node of the list that is not null.
   * @return the data of that node, or NULL if the list is NULL, is empty, or the first node is NULL.
   */
  void* extractFirst();

  /**
   * @brief Extracts the data of last node of the list that is not null.
   * @return the data of that node, or NULL if the list is NULL, is empty, or the first or the last node is NULL.
   */
  void* extractLast();

  /**
   * @brief Extracts the data of a specific node of the list if it is not null.
   * @param position determines the position where the data will be extracted.
   * @return the data of that node, or NULL if the list is NULL, is empty, the first or the last node is NULL or the position > list->length.
   */
  void* extractAt( u16 position);


  // Miscellaneous

  /**
   * @brief Concats the information of two list.
   * @param *vector_src Second list to concat.
   * @return - kErrorCode_List_Null if one of the two list param are NULL.
             - kErrorCode_MemoryNode_Null if the firs node of list is NULL. 
             - KErrorCode_No_Memory if there is no memory available for a new block of MemoryNode.
             - kErrorCode_Ok Everething was done correctly.
   */
  s16 concat(List *list_src);

  /**
   * @brief Calls to a function from all elements of the list
   * @param *callback function to apply on a node
   * @return - kErrorCode_List_Null if the list is NULL.
             - kErrorCode_MemoryNode_Null if the first element of the list is NULL.
             - kErrorCode_NonFuction when the callback is NULL.
             - kErrorCode_Ok Everething was done correctly.
   */
  s16 traverse(void(*callback)(MemoryNode *));

  /**
   * @brief Prints the information of the entire list.
   */

  void print (); // Prints the features and content of the vector


protected:

  /**
   *  ADT List
   * @var head_ determines the first position in the vector.
   * @var tail_ determines the first free position in the vector.
   * @var capacity_ determines the maximum amount of data to be storaged.
   * @var *storage_ pointer to a MemoryNode that will contain the data.
  */
  MemoryNode* head_;
  MemoryNode* tail_;
  u16 length_;
  u16 capacity_;

private:

};




/*struct list_ops_s {
  
  /**
   * @brief Destroys the vector and its data.
   * @param *vector to be destroyed.
   * @return kErrorCode_Null, kErrorCode_Memory_Null or kErrorCode_Ok.
   
	s16 (*destroy)(List *list);*/


 
#endif //__ADT_LIST_H__
