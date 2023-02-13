// adt_vector.h
// Escuela Superior de Arte y Tecnologia
// Algoritmos & Inteligencia Artificial
// ESAT 2020-2021

#ifndef __ADT_VECTOR_H__
#define __ADT_VECTOR_H__ 1

#include "adt_memory_node.h"

class Vector{

  public:

    Vector();

    Vector(Vector& copyvector);
    Vector& operator=(const Vector& other);

    ~Vector();  // Use delete(this) at the end




    /**
    * @brief Careates a new empty vector.
    * @param capacity determines the capacity of the new vector.
    * @return - kErrorCode_Vector_Null if the vector to be initiated is NULL.
              - kErrorCode_ZeroSize when the capacity is <= 0.
              - KErrorCode_No_Memory when there is no memory available for the nodes of the list.
              - kErrorCode_Ok Everething was done correctly.
    */
    s16 init(u16 capacity);

   
    /**
     * @brief Soft-resets each vector's MemoryNode.
     * @return - kErrorCode_Vector_Null if the vector to be destroyed is NULL.
               - kErrorCode_MemoryNode_Null if the storage of that vector is NULL.
               - kErrorCode_Ok Everething was done correctly.
     */
    s16 softReset();

    /**
     * @brief Resets each vector's MemoryNode.
     * @return - kErrorCode_Vector_Null if the vector to be destroyed is NULL.
               - kErrorCode_MemoryNode_Null if the storage of that vector is NULL.
               - kErrorCode_Ok Everething was done correctly
     */
    s16 reset();

    /**
     * @brief Modifies the vector's length.
     * @param new_size determines the new size for the vector.
     * @return - kErrorCode_Vector_Null when the vectr to be risezed is NULL.
               - kErrorCode_MemoryNode_Null when the its storage is NULL.
               - kErrorCode_ZeroSize when the new size of the vector is == 0.
               - KErrorCode_No_Memory when there is no memory free for the new capacity of the vector.
               - kErrorCode_Ok Everething was done correctly.
     */
    s16 resize(u16 new_size);


    // State queries

    /**
     * @brief Returns the amount of elementes to store in the vector.
     * @return maximum number of nodes of the vector.
     */
    u16 capacity();

    /**
     * @brief Returns the length of the vector.
     * @return number of nodes that are used in the vector.
     */
    u16 length();

    /**
     * @brief Checks whether the vector is empty or not.
     * @return true == emtpy, otherwise == false.
     */
    bool isEmpty();

    /**
     * @brief Checks whether the vector is full or not.
     * @return true == full, otherwise == false.
     */
    bool isFull();

    // Data queries

    /**
     * @brief Returns the data of the first node of the vector.
     * @return the data of the first node in the vector, NULL if the vector is empty.
     */
    void* first();

    /**
     * @brief returns the data of the last node of the vector.
     * @return the data of the last node in the vector, NULL if the vector is empty.
     */
    void* last();

    /**
     * @brief Returns the data of an spcecific node of the vector.
     * @param position determines the position where the data will be read.
     * @return the data of an specific node, NULL if the vector is empty or if position >= tail.
     */
    void*  at(u16 position);


    // Insertion  

    /**
     * @brief Inserts data in the first node of the vector.
     * @param *data data to insert.
     * @param bytes size of the data.
     * @return - kErrorCode_Vector_Null when the vector in which the insertion is going to be done is NULL.         
               - kErrorCode_MemoryNode_Null when its storage is NULL.
               - kErrorCode_Data_Null when the data to insert is NULL.
               - kErrorCode_Vector_Full when the vector is full.
               - kErrorCode_ZeroSize when the new data's size is == 0.
               - kErrorCode_Ok Everething was done correctly.
     */
    s16 insertFirst(void *data, u16 bytes);

    /**
     * @brief Inserts data in the last node of the vector.
     * @param *data data to insert.
     * @param bytes size of the data.
     * @return - kErrorCode_Vector_Null when the vector in which the insertion is going to be done is NULL.             
               - kErrorCode_MemoryNode_Null when its storage is NULL.
               - kErrorCode_Data_Null when the data to insert is NULL.
               - kErrorCode_Vector_Full when the vector is full. 
               - kErrorCode_ZeroSize when the new data's size is == 0.
               - kErrorCode_Ok Everething was done correctly.
     */
    s16 insertLast(void *data, u16 bytes);

    /**
     * @brief Inserts data in a specific node of the vector.
     * @param *data data to insert.
     * @param bytes size of the data.
     * @param position determines the position where the data will be inserted.
     * @return - kErrorCode_Vector_Null when the vector in which the insertion is going to be done is NULL.             
               - kErrorCode_MemoryNode_Null when its storage is NULL.
               - kErrorCode_Data_Null when the data to insert is NULL.
               - kErrorCode_Vector_Full when the vector is full.
               - kErrorCode_ZeroSize when the new data's size is == 0.
               - kErrorCode_Ok Everething was done correctly.
     */
    s16 insertAt(void *data, u16 bytes, u16 position);


    // Extraction

    /**
     * @brief Extracts the data of the first node of the vector that is not null.
     * @return the data of that node, NULL if the vector is empty.
     */
    void* extractFirst();

    /**
     * @brief Extracts the data of last node of the vector that is not null.
     * @return the data of that node, NULL if the vector is empty.
     */
    void* extractLast();

    /**
     * @brief Extracts the data of a specific node of the vector if it is not null.
     * @param position determines the position where the data will be extracted.
     * @return the data of that node, NULL if the vector is empty or position > tail.
     */
    void* extractAt(u16 position);


    // Miscellaneous

    /**
     * @brief Concats the information of two vectors.
     * @param *vector_src Second vector to concat.
     * @return - kErrorCode_Vector_Null when one of the two vector to concat are NULL.
               - kErrorCode_MemoryNode_Null when one of the storages of these two vector is NULL. 
               - KErrorCode_No_Memory when there is no memory free for the new vector.
               - kErrorCode_Ok Everething was done correctly.
     */
    s16 concat (Vector *vector_src);

    /**
     * @brief Calls to a function from all elements of the vector
     * @param *callback function to apply on a node.
     * @return - kErrorCode_Vector_Null when the vector is NULL.
               - kErrorCode_MemoryNode_Null when its storage is NULL.
               - kErrorCode_NonFuction when the callback is NULL.
               - kErrorCode_Ok Everething was done correctly.
     */
    s16 traverse(void(*callback)(MemoryNode *));

    /**
     * @brief Prints the information of the entire vector.
     
     */

    void print (); // Prints the features and content of the vector




    
  protected:
    /**
     *  ADT Vector Attributes.
     * @var head_ determines the first position in the vector.
     * @var tail_ determines the first free position in the vector.
     * @var capacity_ determines the maximum amount of data to be storaged.
     * @var *storage_ pointer to a MemoryNode that will contain the data.
     */
    u16 head_;
    u16 tail_;
    u16 capacity_;
    MemoryNode* storage_;
    
};

#endif