// adt_stack.h
// Escuela Superior de Arte y Tecnologia
// Algoritmos & Inteligencia Artificial
// ESAT 2020-2021
//

#ifndef __ADT_STACK_H__
#define __ADT_STACK_H__

#include "adt_vector.h"


/**
 * @struct ADT Stack Struct.
 * @var *storage_ pointer to a Vector that will contain the data.
 * @var *ops_ pointer to Stack Functions.
 */
typedef struct adt_stack_s {

	Vector *storage_;
	struct stack_ops_s *ops_;
} Stack;

/**
 * @struct Stack_Ops_s.
 * @brief Contains all the *ops functions.
 */
struct stack_ops_s {
  
  /**
   * @brief Destroys the Stack and its data.
   * @param *stack to be destroyed.
   * @return - kErrorCode_Stack_Null when the stak to be destroyed is NULL.
             - kErrorCode_Vector_Null when the storage of the stack is NULL or if something was wrong during the destroy.
             - kErrorCode_MemoryNode_Null  when there is a problem during the destruction.
             - kErrorCode_Ok Everething was done correctly.
   */
	s16 (*destroy)(Stack *stack);

  /**
   * @brief Soft-resets the vector of the stack.
   * @param *stack to be soft-reset.
   * @return - kErrorCode_Stack_Null when the stack to be softreset is NULL.
             - kErrorCode_Vector_Null when the storage of the stack is NULL or when something was wrong during the softReset. 
             - kErrorCode_MemoryNode_Null when there is a problem during the softReset.
             - kErrorCode_Ok Everething was done correctly.
   */
  s16 (*softReset)(Stack *stack);

  /**
   * @brief Resets the vector of the stack.
   * @param *stack to be reset.
   * @return - kErrorCode_Stack_Null when the stak to be reset is NULL. 
             - kErrorCode_Vector_Null when the storage of the stack is NULL or when something was wrong during the reset.
             - kErrorCode_MemoryNode_Null when there is a problem during the reset.
             - kErrorCode_Ok Everething was done correctly.
   */
  s16 (*reset)(Stack *stack);
  
  /**
   * @brief Modifies the stack's length.
   * @param *stack to apply the resize.
   * @param new_size determines the new size for the stack.
   * @return - kErrorCode_Stack_Null when the stack to be resized is NULL.
             - kErrorCode_Vector_Null when the storage of the stack is NULL if the vector riseze function return it.
             - kErrorCode_MemoryNode_Null, kErrorCode_ZeroSize, KErrorCode_No_Memory if the vector riseze function return it.
             - kErrorCode_Ok Everething was done correctly.
   */
	s16 (*resize)(Stack *stack, u16 new_size);


	// State queries

  /**
   * @brief Returns the amount of elementes to store in the stack.
   * @param *stack to get its capacity.
   * @return maximum number of nodes of the Stack.
   */
	u16 (*capacity)(Stack *stack);

  /**
   * @brief Returns the length of the stack.
   * @param *stack to get its length.
   * @return number of nodes that are used in the Stack.
   */
	u16 (*length)(Stack *stack);

  /**
   * @brief Checks whether the stack is empty or not.
   * @param *stack to check
   * @return true == emtpy, otherwise == false.
   */
	bool (*isEmpty)(Stack *stack);

  /**
   * @brief Checks whether the stack is full or not.
   * @param *stack to check
   * @return true == full, otherwise == false.
   */
	bool (*isFull)(Stack *stack);

	// Data queries


  /**
   * @brief returns the data of the stack.
   * @param *stack receives the satck.
   * @return the data of the last node in the stack, NULL if the stack or storage is NULL.
   */
  void* (*top)(Stack *stack);


	// Insertion  


  /**
   * @brief Inserts data in the last node of the stack.
   * @param *stack in which you want to insert the data.
   * @param *data data to insert.
   * @param bytes size of the data.
   * return - kErrorCode_Stack_Null when the stack is NULL.
            - kErrorCode_Stack_Full when the Stack is full.
            - kErrorCode_Vector_Null when the storage of the stack is NULL or if the vector insetLast function return it. 
            - kErrorCode_MemoryNode_Null, kErrorCode_Data_Null, kErrorCode_Vector_Full, kErrorCode_ZeroSize if the vector insetLast function return it.
            - kErrorCode_Ok Everething was done correctly.
   */
  s16 (*push)(Stack *stack, void *data, u16 bytes);


  

	// Extraction

 

  /**
   * @brief Extracts the data of last node of the stack that is not null.
   * @param *stack to extract the data from.
   * @return the data of that node or if the stack, it's storage or the capacity of the stack is 0 , return NULL.
   */
	void* (*pop)(Stack *stack);

 

  
	// Miscellaneous
  
  /**
   * @brief Concats the information of two stacks.
   * @param *stack First stack to concat and where the result is going to be.
   * @param *stack_src Second stack to concat.
   * @return  - kErrorCode_Stack_Null one of two stacks are NULL.
              - kErrorCode_Vector_Null one of two of both stacks storages are NULL or if the vector concat function returns it.
              - kErrorCode_MemoryNode_Null if the vector concat function returns it.
              - kErrorCode_Ok  Everething was done correctly.
   */
	s16 (*concat)(Stack *stack, Stack *stack_src);



  /**
   * @brief Prints the information of the entire stack.
   * @param *stack to print its information.
   */
   
	void (*print)(Stack *stack); // Prints the features and content of the stack
};

/**
 * @brief Careates a new empty stack
 * @param capacity determines the capacity of the new stack
 * @return the empty stack, NULL if there is no memory available for the stack.
 */
Stack* STACK_create(u16 capacity);
#endif //__ADT_STACK_H__
