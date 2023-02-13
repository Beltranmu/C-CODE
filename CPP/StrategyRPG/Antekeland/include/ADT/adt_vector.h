// adt_vector.h
// Escuela Superior de Arte y Tecnologia
// Algoritmos & Inteligencia Artificial
// ESAT 2020-2021
//

/*
Includes
prototipados estaticos
los callbacks
Funciones
Vector sucesion de nodos de memoria
tail-> primera posicion libre
head -> es donde empieza siempre 0
capacity -> cuantos memory node tengo
Memorynode -> de esta manera linkamos con nuestr informacion
ops_ -> almacena los callbacks
uniremos los call bakcs
*/
#ifndef __ADT_VECTOR_H__
#define __ADT_VECTOR_H__

#include "adt_memory_node.h"


typedef struct adt_vector_s {
	u16 head_;
	u16 tail_;
	u16 capacity_;
	MemoryNode *storage_;
	struct vector_ops_s *ops_;
} Vector;

/**
 * @struct Vector_Ops_s.
 * @brief Contains all the *ops functions.
 */
struct vector_ops_s {
  
  /**
   * @brief Destroys the vector and its data.
   * @param *vector to be destroyed.
   * @return kErrorCode_Null, kErrorCode_Memory or kErrorCode_Ok.
   */
	s16 (*destroy)(Vector *vector);

  /**
   * @brief Soft-resets each vector's MemoryNode.
   * @param *vector to be soft-reset.
   * @return kErrorCode_Vector_Null, kErrorCode_Memory or kErrorCode_Ok.
   */
  s16 (*softReset)(Vector *vector);

  /**
   * @brief Resets each vector's MemoryNode.
   * @param *vector to be reset.
   * @return kErrorCode_Vector_Null, kErrorCode_Memory or kErrorCode_Ok.
   */
  s16 (*reset)(Vector *vector);
  
  /**
   * @brief Modifies the vector's length.
   * @param *vector to apply the resize.
   * @param new_size determines the new size for the vector.
   * @return kErrorCode_Vector_Null, kErrorCode_Data, kErrorCode_ZeroSize or kErrorCode_Ok.
   */
	s16 (*resize)(Vector *vector, u16 new_size);


	// State queries

  /**
   * @brief Returns the amount of elementes to store in the vector.
   * @param *vector to get its capacity.
   * @return capacity.
   */
	u16 (*capacity)(Vector *vector);

  /**
   * @brief Returns the length of the vector.
   * @param *vector to get its length.
   * @return length.
   */
	u16 (*length)(Vector *vector);

  /**
   * @brief Checks whether the vector is empty or not.
   * @param *vector to check
   * @return true = emtpy, otherwise = false.
   */
	boolean (*isEmpty)(Vector *vector);

  /**
   * @brief Checks whether the vector is full or not.
   * @param *vector to check
   * @return true = full, otherwise = false.
   */
	boolean (*isFull)(Vector *vector);

	// Data queries
  
  /**
   * @brief Returns the data of the first node of the vector.
   * @param *vector receives the vector.
   * @return the data of the first node in the vector.
   */
  void* (*first)(Vector *vector);

  /**
   * @brief returns the data of the last node of the vector.
   * @param *vector receives the vector.
   * @return the data of the last node in the vector.
   */
  void* (*last)(Vector *vector);

  /**
   * @brief Returns the data of an spcecific node of the vector.
   * @param *vector receives the vector.
   * @param position determines the position where the data will be read.
   * @return the data of an specific node.
   */
  void* (*at)(Vector *vector, u16 position);


	// Insertion  

  /**
   * @brief Inserts data in the first node of the vector.
   * @param *vector in which you want to insert the data.
   * @param *data data to insert.
   * @param bytes size of the data.
   * @return kErrorCode_Vector_Null, kErrorCode_Memory, kErrorCode_Data, kErrorCode_VectorFull, kErrorCode_ZeroSize, kErrorCode_Ok
   */
  s16 (*insertFirst)(Vector *vector, void *data, u16 bytes);

  /**
   * @brief Inserts data in the last node of the vector.
   * @param *vector in which you want to insert the data.
   * @param *data data to insert.
   * @param bytes size of the data.
   * @return kErrorCode_Vector_Null, kErrorCode_Memory, kErrorCode_Data, kErrorCode_VectorFull, kErrorCode_ZeroSize, kErrorCode_Ok
   */
  s16 (*insertLast)(Vector *vector, void *data, u16 bytes);

  /**
   * @brief Inserts data in a specific node of the vector.
   * @param *vector in which you want to insert the data.
   * @param *data data to insert.
   * @param bytes size of the data.
   * @param position determines the position where the data will be inserted.
   * @return return kErrorCode_Vector_Null, kErrorCode_Memory, kErrorCode_Data, kErrorCode_VectorFull, kErrorCode_ZeroSize, kErrorCode_Ok
   */
  s16 (*insertAt)(Vector *vector, void *data, u16 bytes, u16 position);
  

	// Extraction

  /**
   * @brief Extracts the data of the first node of the vector that is not null.
   * @param *vector to extract the data from.
   * @return the data of that node.
   */
	void* (*extractFirst)(Vector *vector);

  /**
   * @brief Extracts the data of last node of the vector that is not null.
   * @param *vector to extract the data from.
   * @return the data of that node.
   */
	void* (*extractLast)(Vector *vector);

  /**
   * @brief Extracts the data of a specific node of the vector if it is not null.
   * @param *vector to extract the data from.
   * @param position determines the position where the data will be extracted.
   * @return the data of that node.
   */
	void* (*extractAt)(Vector *vector, u16 position);

  
	// Miscellaneous
  
  /**
   * @brief Concats the information of two vectors.
   * @param *vector First vector to concat.
   * @param *vector Second vector to concat.
   * @return kErrorCode_Vector_Null, kErrorCode_Ok
   */
	s16 (*concat)(Vector *vector, Vector *vector_src);

  /**
   * @brief Calls to a function from all elements of the vector
   * @param *vector to apply different functions.
   * @param *callback function to apply on a node
   * @return kErrorCode_Vector_Null, kErrorCode_Vector_Null, kErrorCode_Ok
   */
	s16 (*traverse)(Vector *vector, void (*callback)(MemoryNode *));

  /**
   * @brief Prints the information of the entire vector.
   * @param *vector to print its information.
   */
	void (*print)(Vector *vector); // Prints the features and content of the vector
};

/**
 * @brief Careates a new empty vector
 * @param capacity determines the capacity of the new vector
 * @return the empty vector 
 */
Vector* VECTOR_create(u16 capacity);
#endif //__ADT_VECTOR_H__
