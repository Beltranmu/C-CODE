// adt_stack.h
// Escuela Superior de Arte y Tecnologia
// Algoritmos & Inteligencia Artificial
// ESAT 2020-2021
//

#ifndef __ADT_STACK_H__
#define __ADT_STACK_H__

#include "adt_vector.h"


/**
 * @class Stack
 * @var *storage_ pointer to a Vector that will contain the data.
 */

class Stack
{
public:
  Stack();
  Stack(Stack &copystack);
  Stack& operator=(const Stack &stack);
  ~Stack();

  /**
   * @brief Create an empty container for the stack.
   * @param capacity determines the capacity of the new stack
   * @return - if the Stack to be initiated is NULL.
   - kErrorCode_ZeroSize when the capacity is <= 0.
             - KErrorCode_No_Memory when there is no memory available for the nodes of the list.
             - kErrorCode_Ok Everething was done correctly.
  */
  s16 init(u16 capacity);

  /**
   * @brief Soft-resets the vector of the stack.
   * @return - kErrorCode_Stack_Null when the stack to be softreset is NULL.
             - kErrorCode_Vector_Null when the storage of the stack is NULL or when something was wrong during the softReset. 
             - kErrorCode_MemoryNode_Null when there is a problem during the softReset.
             - kErrorCode_Ok Everething was done correctly.
  */
  s16 softReset();


  /**
  * @brief Resets each the vector of the stack
  * @return - kErrorCode_Stack_Null when the stak to be reset is NULL. 
            - kErrorCode_Vector_Null when the storage of the stack is NULL or when something was wrong during the reset.
            - kErrorCode_MemoryNode_Null when there is a problem during the reset.
            - kErrorCode_Ok Everething was done correctly.
  */
  s16 reset();


  /**
   * @brief Modifies the stack's length.
   * @param new_size determines the new size for the vector.
   * @return - kErrorCode_Stack_Null when the stack to be resized is NULL.
             - kErrorCode_Vector_Null when the storage of the stack is NULL if the vector riseze function return it.
             - kErrorCode_MemoryNode_Null, kErrorCode_ZeroSize, KErrorCode_No_Memory if the vector riseze function return it.
             - kErrorCode_Ok Everething was done correctly.
 */
  s16 resize(u16 new_size);

  /**
   * @brief Returns the amount of elementes to store in the stack.
   * @return maximum number of nodes of the Stack.
  */
  u16 capacity();

  /**
   * @brief Returns the length of the stack.
   * @return number of nodes that are used in the Stack.
  */
  u16 length();

  /**
   * @brief Checks whether the stack is empty or not.
   * @return true == emtpy, otherwise == false.
  */
  bool isEmpty();

  /**
   * @brief Checks whether the stack is full or not.
   * @return true == full, otherwise == false.
  */
  bool isFull();


  // Data queries
  /**
   * @brief returns the last data of the stack.
   * @return the data of the last node in the stack, NULL if the stack or storage is NULL.
  */
  void* top();


  // Insertion 
  /**
   * @brief Inserts data at the end of the stack.
   * @param *data data to insert.
   * @param bytes size of the data.
   * return - kErrorCode_Stack_Null when the stack is NULL.
            - kErrorCode_Stack_Full when the Stack is full.
            - kErrorCode_Vector_Null when the storage of the stack is NULL or if the vector insetLast function return it. 
            - kErrorCode_MemoryNode_Null, kErrorCode_Data_Null, kErrorCode_Vector_Full, kErrorCode_ZeroSize if the vector insetLast function return it.
            - kErrorCode_Ok Everething was done correctly.
  */
  s16 push( void *data, u16 bytes);


  // Extraction
  /**
   * @brief Extracts the data of last data of the stack.
   * @return the data of that node or if the stack, it's storage or the capacity of the stack is 0 , return NULL.
  */
  void* pop();


  // Miscellaneous

  /**
   * @brief Concats the information of two stacks.
   * @param *stack_src stack to concat.
   * @return  - kErrorCode_Stack_Null one of two stacks are NULL.
              - kErrorCode_Vector_Null one of two of both stacks storages are NULL or if the vector concat function returns it.
              - kErrorCode_MemoryNode_Null if the vector concat function returns it.
              - kErrorCode_Ok  Everething was done correctly.
  */
  s16 concat(Stack *stack_src);

  /**
   * @brief Prints the information of the entire stack.
   */
  void print(); // Prints the features and content of the vector
protected:
  Vector *storage_; ///@var *storage_ pointer to a List that will contain the datas.

private:
};

#endif