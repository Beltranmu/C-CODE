// adt_circular_vector.h
// Escuela Superior de Arte y Tecnologia
// Algoritmos & Inteligencia Artificial
// ESAT 2020-2021
//

#ifndef __ADT_CIRCULAR_VECTOR_H__
#define __ADT_CIRCULAR_VECTOR_H__

#include "adt_memory_node.h"

/**
 * @struct ADT Circular Vector Struct.
 * @var head_ determines the first position in the vector.
 * @var tail_ determines the first free position in the vector.
 * @var capacity_ determines the maximum amount of data to be storaged.
 * @var *storage_ pointer to a MemoryNode that will contain the data.
 * @var *ops_ pointer to Vector Functions.
 */
typedef struct adt_circular_vector_s {
	u16 head_;
	u16 tail_;
	u16 capacity_;
	MemoryNode *storage_;
	struct circular_vector_ops_s *ops_;
} CircularVector;

/**
 * @struct Circular_Vector_Ops_s.
 * @brief Contains all the *ops functions.
 */
struct circular_vector_ops_s {
  
  /**
   * @brief Destroys and frees the vector and its data.
   * @param *vector to be destroyed.
   * @return - kErrorCode_VectorC_Null if the vector to be destroyed is NULL.
             - kErrorCode_MemoryNode_Null if the storage of that vector is NULL.
             - kErrorCode_Ok Everething was done correctly.
   */
	s16 (*destroy)(CircularVector *vector);

  /**
   * @brief Soft-resets each vector's MemoryNode.
   * @param *vector to be soft-reset.
   * @return - kErrorCode_VectorC_Null if the vector to be destroyed is NULL.
             - kErrorCode_MemoryNode_Null if the storage of that vector is NULL.
             - kErrorCode_Ok Everething was done correctly.
   */
  s16 (*softReset)(CircularVector *vector);

  /**
   * @brief Resets each vector's MemoryNode.
   * @param *vector to be reset.
   * @return - kErrorCode_VectorC_Null if the vector to be destroyed is NULL.
             - kErrorCode_MemoryNode_Null if the storage of that vector is NULL.
             - kErrorCode_Ok Everething was done correctly.
   */
  s16 (*reset)(CircularVector *vector);
  
  /**
   * @brief Modifies the vector's length.
   * @param *vector to apply the resize.
   * @param new_size determines the new size for the vector.
   * @return - kErrorCode_VectorC_Null when the vectr to be risezed is NULL.
             - kErrorCode_MemoryNode_Null when the its storage is NULL.
             - kErrorCode_ZeroSize when the new size of the vector is == 0.
             - KErrorCode_No_Memory when there is no memory free for the new capacity of the vector.
             - kErrorCode_Ok Everething was done correctly.
   */
	s16 (*resize)(CircularVector *vector, u16 new_size);


	// State queries

  /**
   * @brief Returns the amount of elementes to store in the vector.
   * @param *vector to get its capacity.
   * @return maximum number of nodes of the vector.
   */
	u16 (*capacity)(CircularVector *vector);

  /**
   * @brief Returns the length of the vector.
   * @param *vector to get its length.
   * @return number of nodes that are used in the vector.
   */
	u16 (*length)(CircularVector *vector);

  /**
   * @brief Checks whether the vector is empty or not.
   * @param *vector to check
   * @return true == emtpy, otherwise == false.
   */
	bool (*isEmpty)(CircularVector *vector);

  /**
   * @brief Checks whether the vector is full or not.
   * @param *vector to check
   * @return true == full, otherwise == false.
   */
	bool (*isFull)(CircularVector *vector);

	// Data queries
  
  /**
   * @brief Returns the data of the first node of the vector.
   * @param *vector receives the vector.
   * @return the data of the first node in the vector, NULL if the vector is empty.
   */
  void* (*first)(CircularVector *vector);

  /**
   * @brief returns the data of the last node of the vector.
   * @param *vector receives the vector.
   * @return the data of the last node in the vector, NULL if the vector is empty.
   */
  void* (*last)(CircularVector *vector);

  /**
   * @brief Returns the data of an spcecific node of the vector.
   * @param *vector receives the vector.
   * @param position determines the position where the data will be read.
   * @return the data of an specific node, NULL if the vector is empty or if position >= tail.
   */
  void* (*at)(CircularVector *vector, u16 position);


	// Insertion  

  /**
   * @brief Inserts data in the first node of the vector.
   * @param *vector in which you want to insert the data.
   * @param *data data to insert.
   * @param bytes size of the data.
   * @return - kErrorCode_VectorC_Null when the vector in which the insertion is going to be done is NULL.
             - kErrorCode_MemoryNode_Null when its storage is NULL.
             - kErrorCode_Data_Null when the data to insert is NULL.
             - kErrorCode_Vector_Full when the vector is full. 
             - kErrorCode_ZeroSize when the new data's size is == 0.
             - kErrorCode_Ok Everething was done correctly.
   */
  s16 (*insertFirst)(CircularVector *vector, void *data, u16 bytes);

  /**
   * @brief Inserts data in the last node of the vector.
   * @param *vector in which you want to insert the data.
   * @param *data data to insert.
   * @param bytes size of the data.
   * @return - kErrorCode_VectorC_Null when the vector in which the insertion is going to be done is NULL.  
             - kErrorCode_MemoryNode_Null when its storage is NULL.
             - kErrorCode_Data_Null when the data to insert is NULL.
             - kErrorCode_Vector_Full when the vector is full.
             - kErrorCode_ZeroSize when the new data's size is == 0.
             - kErrorCode_Ok Everething was done correctly.
   */
  s16 (*insertLast)(CircularVector *vector, void *data, u16 bytes);

  /**
   * @brief Inserts data in a specific node of the vector.
   * @param *vector in which you want to insert the data.
   * @param *data data to insert.
   * @param bytes size of the data.
   * @param position determines the position where the data will be inserted.
   * @return - kErrorCode_VectorC_Null when the vector in which the insertion is going to be done is NULL.     
             - kErrorCode_MemoryNode_Null when its storage is NULL.
             - kErrorCode_Data_Null when the data to insert is NULL.
             - kErrorCode_Vector_Full when the vector is full. 
             - kErrorCode_ZeroSize when the new data's size is == 0.
             - kErrorCode_Ok Everething was done correctly.
   */
  s16 (*insertAt)(CircularVector *vector, void *data, u16 bytes, u16 position);
  

	// Extraction

  /**
   * @brief Extracts the data of the first node of the vector that is not null.
   * @param *vector to extract the data from.
   * @return the data of that node, NULL if the vector is empty.
   */
	void* (*extractFirst)(CircularVector *vector);

  /**
   * @brief Extracts the data of last node of the vector that is not null.
   * @param *vector to extract the data from.
   * @return the data of that node, NULL if the vector is empty.
   */
	void* (*extractLast)(CircularVector *vector);

  /**
   * @brief Extracts the data of a specific node of the vector if it is not null.
   * @param *vector to extract the data from.
   * @param position determines the position where the data will be extracted.
   * @return the data of that node, NULL if the vector is empty or position > tail.
   */
	void* (*extractAt)(CircularVector *vector, u16 position);

  
	// Miscellaneous
  
  /**
   * @brief Concats the information of two vectors.
   * @param *vector First vector to concat and where the result is going to be.
   * @param *vector_src Second vector to concat.
   * @return - kErrorCode_VectorC_Null when one of the two vector to concat are NULL.
             - kErrorCode_MemoryNode_Null when one of the storages of these two vector is NULL. 
             - KErrorCode_No_Memory when there is no memory free for the new vector.
             - kErrorCode_Ok Everething was done correctly.
   */
	s16 (*concat)(CircularVector *vector, CircularVector *vector_src);

  /**
   * @brief Calls to a function from all elements of the vector
   * @param *vector to apply different functions.
   * @param *callback function to apply on a node
   * @return - kErrorCode_VectorC_Null when the vector is NULL.
             - kErrorCode_MemoryNode_Null when its storage is NULL.
             - kErrorCode_NonFuction when the callback is NULL.
             - kErrorCode_Ok Everething was done correctly.
   */
	s16 (*traverse)(CircularVector *vector, void (*callback)(MemoryNode *));

  /**
   * @brief Prints the information of the entire vector.
   * @param *vector to print its information.
   */
   
	void (*print)(CircularVector *vector); // Prints the features and content of the vector.
};

/**
 * @brief Careates a new empty vector.
 * @param capacity determines the capacity of the new vector.
 * @return the empty vector or return NULL if something went wrong (no free memory,..).
 */
CircularVector* CIRCULAR_VECTOR_create(u16 capacity);
#endif //__ADT_VECTOR_H__
