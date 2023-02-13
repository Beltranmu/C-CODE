// adt_movable_head_vector.h
// Escuela Superior de Arte y Tecnologia
// Algoritmos & Inteligencia Artificial
// ESAT 2020-2021
//

#ifndef __ADT_MOVABLE_HEAD_VECTOR_H__
#define __ADT_MOVABLE_HEAD_VECTOR_H__

#include "adt_memory_node.h"

/**
 * @struct ADT Movable Head Vector Struct.
 * @var head_ determines the first position in the vector.
 * @var tail_ determines the first free position in the vector.
 * @var capacity_ determines the maximum amount of data to be storaged.
 * @var *storage_ pointer to a MemoryNode that will contain the data.
 * @var *ops_ pointer to Vector Functions.
 */
typedef struct adt_movable_head_vector_s {
	u16 head_;
	u16 tail_;
	u16 capacity_;
	MemoryNode *storage_;
	struct movable_head_vector_ops_s *ops_;
} MovableHeadVector;

/**
 * @struct Movable_Head_Vector_Ops_s.
 * @brief Contains all the *ops functions.
 */
struct movable_head_vector_ops_s {
  
  /**
   * @brief Destroys the vector and its data.
   * @param *vector to be destroyed.
   * @return - kErrorCode_VectorMH_Null if the vector to be destroyed is NULL.
             - kErrorCode_MemoryNode_Null if the storage of that vector is NULL.
             - kErrorCode_Ok Everething was done correctly.
   */
	s16 (*destroy)(MovableHeadVector *vector);

  /**
   * @brief Soft-resets each vector's MemoryNode.
   * @param *vector to be soft-reset.
   * @return - kErrorCode_VectorMH_Null if the vector to be destroyed is NULL.
             - kErrorCode_MemoryNode_Null if the storage of that vector is NULL.
             - kErrorCode_Ok Everething was done correctly.
   */
  s16 (*softReset)(MovableHeadVector *vector);

  /**
   * @brief Resets each vector's MemoryNode.
   * @param *vector to be reset.
   * @return - kErrorCode_VectorMH_Null if the vector to be destroyed is NULL.
             - kErrorCode_MemoryNode_Null if the storage of that vector is NULL.
             - kErrorCode_Ok Everething was done correctly.
   */
  s16 (*reset)(MovableHeadVector *vector);
  
  /**
   * @brief Modifies the vector's length.
   * @param *vector to apply the resize.
   * @param new_size determines the new size for the vector.
   * @return - kErrorCode_VectorMH_Null when the vectr to be risezed is NULL.
             - kErrorCode_MemoryNode_Null when the its storage is NULL.
             - kErrorCode_ZeroSize when the new size of the vector is == 0.
             - KErrorCode_No_Memory when there is no memory free for the new capacity of the vector.
             - kErrorCode_Ok Everething was done correctly.
   */
	s16 (*resize)(MovableHeadVector *vector, u16 new_size);


	// State queries

  /**
   * @brief Returns the amount of elementes to store in the vector.
   * @param *vector to get its capacity.
   * @return maximum number of nodes of the vector.
   */
	u16 (*capacity)(MovableHeadVector *vector);

  /**
   * @brief Returns the length of the vector.
   * @param *vector to get its length.
   * @return number of nodes that are used in the vector.
   */
	u16 (*length)(MovableHeadVector *vector);

  /**
   * @brief Checks whether the vector is empty or not.
   * @param *vector to check
   * @return true == emtpy, otherwise == false.
   */
	bool (*isEmpty)(MovableHeadVector *vector);

  /**
   * @brief Checks whether the vector is full or not.
   * @param *vector to check
   * @return true == full, otherwise == false.
   */
	bool (*isFull)(MovableHeadVector *vector);

	// Data queries
  
  /**
   * @brief Returns the data of the first node of the vector.
   * @param *vector receives the vector.
   * @return the data of the first node in the vector, NULL if the vector is empty.
   */
  void* (*first)(MovableHeadVector *vector);

  /**
   * @brief returns the data of the last node of the vector.
   * @param *vector receives the vector.
   * @return the data of the last node in the vector, NULL if the vector is empty.
   */
  void* (*last)(MovableHeadVector *vector);

  /**
   * @brief Returns the data of an spcecific node of the vector.
   * @param *vector receives the vector.
   * @param position determines the position where the data will be read.
   * @return the data of an specific node tell by the position, NULL if the vector is empty or if position >= tail.
   */
  void* (*at)(MovableHeadVector *vector, u16 position);


	// Insertion  

  /**
   * @brief Inserts data in the first node of the vector.
   * @param *vector in which you want to insert the data.
   * @param *data data to insert.
   * @param bytes size of the data.
   * @return - kErrorCode_VectorMH_Null when the vector in which the insertion is going to be done is NULL.
             - kErrorCode_MemoryNode_Null when its storage is NULL.
             - kErrorCode_Data_Null when the data to insert is NULL.
             - kErrorCode_Vector_Full when the vector is full .
             - kErrorCode_ZeroSize when the new data's size is == 0.
             - kErrorCode_Ok Everething was done correctly.
   */
  s16 (*insertFirst)(MovableHeadVector *vector, void *data, u16 bytes);

  /**
   * @brief Inserts data in the last node of the vector.
   * @param *vector in which you want to insert the data.
   * @param *data data to insert.
   * @param bytes size of the data.
   * @return - kErrorCode_VectorMH_Null when the vector in which the insertion is going to be done is NULL.  
             - kErrorCode_MemoryNode_Null when its storage is NULL.
             - kErrorCode_Data_Null when the data to insert is NULL.
             - kErrorCode_Vector_Full when the vector is full.
             - kErrorCode_ZeroSize when the new data's size is == 0.
             - kErrorCode_Ok Everething was done correctly.
   */
  s16 (*insertLast)(MovableHeadVector *vector, void *data, u16 bytes);

  /**
   * @brief Inserts data in a specific node of the vector.
   * @param *vector in which you want to insert the data.
   * @param *data data to insert.
   * @param bytes size of the data.
   * @param position determines the position where the data will be inserted.
   * @return - kErrorCode_VectorMH_Null when the vector in which the insertion is going to be done is NULL.     
             - kErrorCode_MemoryNode_Null when its storage is NULL.
             - kErrorCode_Data_Null when the data to insert is NULL.
             - kErrorCode_Vector_Full when the vector is full. 
             - kErrorCode_ZeroSize when the new data's size is == 0.
             - kErrorCode_Ok Everething was done correctly.
   */
  s16 (*insertAt)(MovableHeadVector *vector, void *data, u16 bytes, u16 position);
  

	// Extraction

  /**
   * @brief Extracts the data of the first node of the vector that is not null.
   * @param *vector to extract the data from.
   * @return the data of that node, NULL if the vector is empty.
   */
	void* (*extractFirst)(MovableHeadVector *vector);

  /**
   * @brief Extracts the data of last node of the vector that is not null.
   * @param *vector to extract the data from, NULL if the vector is empty.
   * @return the data of that node, NULL if the vector is empty.
   */
	void* (*extractLast)(MovableHeadVector *vector);

  /**
   * @brief Extracts the data of a specific node of the vector if it is not null.
   * @param *vector to extract the data from.
   * @param position determines the position where the data will be extracted.
   * @return the data of that node, NULL if the vector is empty or position > tail.
   */
	void* (*extractAt)(MovableHeadVector *vector, u16 position);

  
	// Miscellaneous
  
  /**
   * @brief Concats the information of two vectors.
   * @param *vector First vector to concat and where the result is going to be.
   * @param *vector_src Second vector to concat.
   * @return - kErrorCode_VectorMH_Null when one of the two vector to concat are NULL
             - kErrorCode_MemoryNode_Null when one of the storages of these two vector is NULL. 
             - KErrorCode_No_Memory when there is no memory free for the new vector.
             - kErrorCode_Ok Everething was done correctly.
   */
	s16 (*concat)(MovableHeadVector *vector, MovableHeadVector *vector_src);

  /**
   * @brief Calls to a function from all elements of the vector
   * @param *vector to apply different functions.
   * @param *callback function to apply on a node
   * @return - kErrorCode_VectorMH_Null when the vector is NULL.
             - kErrorCode_MemoryNode_Null when its storage is NULL.
             - kErrorCode_NonFuction when the callback is NULL.
             - kErrorCode_Ok Everething was done correctly.
   */
	s16 (*traverse)(MovableHeadVector *vector, void (*callback)(MemoryNode *));

  /**
   * @brief Prints the information of the entire vector.
   * @param *vector to print its information.
   */
   
	void (*print)(MovableHeadVector *vector); // Prints the features and content of the vector
};

/**
 * @brief Careates a new empty vector
 * @param capacity determines the capacity of the new vector
 * @return the empty vector or return NULL if something went wrong (no free memory,..).
 */
MovableHeadVector* MOVABLE_HEAD_VECTOR_create(u16 capacity);
#endif //__ADT_MOVABLE_HEAD_VECTOR_H__
