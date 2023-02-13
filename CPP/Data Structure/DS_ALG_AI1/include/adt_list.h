// adt_list.h
// Escuela Superior de Arte y Tecnologia
// Algoritmos & Inteligencia Artificial
// ESAT 2020-2021
//

#ifndef __ADT_LIST_H__
#define __ADT_LIST_H__

#include "adt_memory_node.h"

/**
 * @struct ADT List 
 * @var head_ points to the first node of the list.
 * @var tail_ points to the last node of the list.
 * @var lenght_ how many nodes are in the list.
 * @var capacity_ determines the maximum amount of data to be storaged.
 * @var *ops_ pointer to list Functions.
 */
typedef struct adt_list_s {
	MemoryNode* head_;
	MemoryNode* tail_;
	u16 length_;
	u16 capacity_;
	struct list_ops_s *ops_;
} List;

/**
 * @struct List_Ops_s.
 * @brief Contains all the *ops functions.
 */
struct list_ops_s {
  
  /**
   * @brief Destroys the list and all its nodes.
   * @param *list to be destroyed.
   * @return - kErrorCode_List_Null when the list to be destroyed is NULL.
             - kErrorCode_MemoryNode_Null if the first node is NULL.
             - kErrorCode_List_Empty if the list is empty.
             - kErrorCode_Ok Everething was done correctly.
   */
	s16 (*destroy)(List *list);

  /**
   * @brief Soft-resets each list's MemoryNode.
   * @param *list to be soft-reset.
   * @return - kErrorCode_List_Null when the list to be soft-reset is NULL.
             - kErrorCode_MemoryNode_Null if the first node of the list is NULL.
             - kErrorCode_List_Empty if the list is empty.
             - kErrorCode_Ok Everething was done correctly.
   */
  s16 (*softReset)(List *list);

  /**
   * @brief Resets each list's MemoryNode.
   * @param *list to be reset.
   * @return - kErrorCode_List_Null when the list to be reset is NULL.
             - kErrorCode_MemoryNode_Null if the first node is NULL.
             - kErrorCode_List_Empty if the list is empty.
             - kErrorCode_Ok Everething was done correctly.
   */
  s16 (*reset)(List *list);
  
  /**
   * @brief Modifies the list's length.
   * @param *list to apply the resize.
   * @param new_size determines the new size for the list.
   * @return - kErrorCode_List_Null if the list to be resized is NULL.
             - kErrorCode_ZeroSize if the new size is less or equal 0.
             - kErrorCode_Ok Everething was done correctly.
   */
	s16 (*resize)(List *list, u16 new_size);


	// State queries

  /**
   * @brief Returns the amount of elementes to store in the list.
   * @param *list to get its capacity.
   * @return the maximum number of nodes of the list.
   */
	u16 (*capacity)(List *list);

  /**
   * @brief Returns the length of the list.
   * @param *list to get its length.
   * @return number of nodes that are used in the list.
   */
	u16 (*length)(List *list);

  /**
   * @brief Checks whether the list is empty or not.
   * @param *list to check
   * @return true == emtpy, otherwise == false.
   */
	bool (*isEmpty)(List *list);

  /**
   * @brief Checks whether the list is full or not.
   * @param *list to check
   * @return true == full, otherwise == false.
   */
	bool (*isFull)(List *list);

	// Data queries
  
  /**
   * @brief Returns the data of the first node of the list.
   * @param *list receives the list.
   * @return the data of the first node in the list, NULL if the list is NULL, is empty or there is no nodes in the list.
   */
  void* (*first)(List *list);

  /**
   * @brief returns the data of the last node of the list.
   * @param *list receives the list.
   * @return the data of the last node in the list, NULL if the list is NULL, is empty or there is no nodes in the list.
   */
  void* (*last)(List *list);

  /**
   * @brief Returns the data of an spcecific node of the list.
   * @param *list receives the list.
   * @param position determines the position where the data will be read.
   * @return the data of an specific node, NULL if the list is NULL, is empty, there is no nodes in the list or position > list->length.
   */
  void* (*at)(List *list, u16 position);


	// Insertion  

  /**
   * @brief Inserts data in the first node of the list.
   * @param *list in which you want to insert the data.
   * @param *data data to insert.
   * @param bytes size of the data.
   * @return - kErrorCode_List_Null if the list is NULL.
             - kErrorCode_Data_Null if the data to insert is NULL.
             - kErrorCode_List_Full if the list is full.
             - KErrorCode_No_Memory if there is no memory available for a new node.
             - kErrorCode_ZeroSize if size of the data to insert is 0.
             - kErrorCode_Ok Everething was done correctly.
   */
  s16 (*insertFirst)(List *list, void *data, u16 bytes);

  /**
   * @brief Inserts data in the last node of the list.
   * @param *list in which you want to insert the data.
   * @param *data data to insert.
   * @param bytes size of the data.
   * @return - kErrorCode_List_Null if the list is NULL.
             - kErrorCode_Data_Null if the data to insert is NULL.
             - kErrorCode_List_Full if the list is full.
             - KErrorCode_No_Memory if there is no memory available for a new node.
             - kErrorCode_ZeroSize if size of the data to insert is 0.
             - kErrorCode_Ok Everething was done correctly.
   */
  s16 (*insertLast)(List *list, void *data, u16 bytes);

  /**
   * @brief Inserts data in a specific node of the list.
   * @param *list in which you want to insert the data.
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
  s16 (*insertAt)(List *list, void *data, u16 bytes, u16 position);
  

	// Extraction

  /**
   * @brief Extracts the data of the first node of the list that is not null.
   * @param *list to extract the data from.
   * @return the data of that node, or NULL if the list is NULL, is empty, or the first node is NULL.
   */
	void* (*extractFirst)(List *list);

  /**
   * @brief Extracts the data of last node of the list that is not null.
   * @param *list to extract the data from.
   * @return the data of that node, or NULL if the list is NULL, is empty, or the first or the last node is NULL.
   */
	void* (*extractLast)(List *list);

  /**
   * @brief Extracts the data of a specific node of the list if it is not null.
   * @param *list to extract the data from.
   * @param position determines the position where the data will be extracted.
   * @return the data of that node, or NULL if the list is NULL, is empty, the first or the last node is NULL or the position > list->length.
   */
	void* (*extractAt)(List *list, u16 position);

  
	// Miscellaneous
  
  /**
   * @brief Concats the information of two lists.
   * @param *list First list to concat and where the result is going to be.
   * @param *list_src Second list to concat.
   * @return - kErrorCode_List_Null if one of the two list param are NULL.
             - kErrorCode_MemoryNode_Null if the firs node of list is NULL. 
             - KErrorCode_No_Memory if there is no memory available for a new block of MemoryNode.
             - kErrorCode_Ok Everething was done correctly.
   */
	s16 (*concat)(List *list, List *list_src);

  /**
   * @brief Calls to a function from all elements of the list
   * @param *list to apply different functions.
   * @param *callback function to apply on a node
   * @return - kErrorCode_List_Null if the list is NULL.
             - kErrorCode_MemoryNode_Null if the first element of the list is NULL.
             - kErrorCode_NonFuction when the callback is NULL.
             - kErrorCode_Ok Everething was done correctly.
   */
	s16 (*traverse)(List *list, void (*callback)(MemoryNode *));

  /**
   * @brief Prints the information of the entire list.
   * @param *list to print its information.
   */
   
	void (*print)(List *list); // Prints the features and content of the list
};

/**
 * @brief Careates a new empty list
 * @param capacity determines the capacity of the new list
 * @return the empty list or return NULL if something went wrong (no free memory,..).
 */
List* LIST_create(u16 capacity);
#endif //__ADT_list_H__
