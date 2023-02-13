#include <stdio.h>
#include <stdlib.h>


#include "common_def.h"
#include "adt_circular_vector.h"
#include "ABGS_MemoryManager/abgs_memory_manager.h"

//Vector Declaratoon
static s16 CIRCULAR_VECTOR_destroy(CircularVector* vector);
static s16 CIRCULAR_VECTOR_softReset(CircularVector* vector);
static s16 CIRCULAR_VECTOR_reset(CircularVector* vector);
static s16 CIRCULAR_VECTOR_resize(CircularVector* vector, u16 new_size);

static u16 CIRCULAR_VECTOR_capacity(CircularVector* vector);
static u16 CIRCULAR_VECTOR_length(CircularVector* vector);
static bool CIRCULAR_VECTOR_isEmpty(CircularVector* vector);
static bool CIRCULAR_VECTOR_isFull(CircularVector* vector);

static void* CIRCULAR_VECTOR_first(CircularVector* vector);
static void* CIRCULAR_VECTOR_last(CircularVector* vector);
static void* CIRCULAR_VECTOR_at(CircularVector* vector, u16 position);

static s16 CIRCULAR_VECTOR_insertFirst(CircularVector* vector, void* data, u16 bytes);
static s16 CIRCULAR_VECTOR_insertLast(CircularVector* vector, void* data, u16 bytes);
static s16 CIRCULAR_VECTOR_insertAt(CircularVector* vector, void* data, u16 bytes, u16 position);

static void* CIRCULAR_VECTOR_extractFirst(CircularVector* vector);
static void* CIRCULAR_VECTOR_extractLast(CircularVector* vector);
static void* CIRCULAR_VECTOR_extractAt(CircularVector* vector, u16 position);

static s16 CIRCULAR_VECTOR_concat(CircularVector* vector, CircularVector* CIRCULAR_VECTOR_src);
static s16 CIRCULAR_VECTOR_traverse(CircularVector* vector, void(*callback)(MemoryNode *));
static s16 CIRCULAR_VECTOR_print(CircularVector* vector);




// Vector's API Definitions
struct circular_vector_ops_s circular_vector_ops_s = { .destroy = CIRCULAR_VECTOR_destroy,
                                                       .softReset = CIRCULAR_VECTOR_softReset,
                                                       .reset = CIRCULAR_VECTOR_reset,
                                                       .resize = CIRCULAR_VECTOR_resize,
                                                       .capacity = CIRCULAR_VECTOR_capacity,
                                                       .length = CIRCULAR_VECTOR_length, 
                                                       .isEmpty = CIRCULAR_VECTOR_isEmpty,
                                                       .isFull = CIRCULAR_VECTOR_isFull,
                                                       .first = CIRCULAR_VECTOR_first,
                                                       .last = CIRCULAR_VECTOR_last,
                                                       .at = CIRCULAR_VECTOR_at,
                                                       .insertFirst = CIRCULAR_VECTOR_insertFirst,
                                                       .insertLast = CIRCULAR_VECTOR_insertLast,
                                                       .insertAt = CIRCULAR_VECTOR_insertAt,
                                                       .extractFirst = CIRCULAR_VECTOR_extractFirst,
                                                       .extractLast = CIRCULAR_VECTOR_extractLast,
                                                       .extractAt = CIRCULAR_VECTOR_extractAt,
                                                       .concat = CIRCULAR_VECTOR_concat,
                                                       .traverse = CIRCULAR_VECTOR_traverse,
                                                       .print = CIRCULAR_VECTOR_print             
};



// Vector Definitions

s16 CIRCULAR_VECTOR_destroy(CircularVector* vector){
  if (NULL == vector) {
    return kErrorCode_VectorC_Null;
  }
  if (NULL == vector->storage_) {
    MM->free(vector);
    return kErrorCode_MemoryNode_Null;
  }
  int j = vector->head_;
  for (int i = 0; i < vector->capacity_; ++i) {
    vector->storage_->ops_->reset(vector->storage_ + j);
    ++j;
    j%= vector->capacity_;
  }

  MM->free(vector->storage_);
  MM->free(vector);
  return kErrorCode_Ok;
}

s16 CIRCULAR_VECTOR_softReset(CircularVector* vector) {
  if (NULL == vector) {
    return kErrorCode_VectorC_Null;
  }
  if (NULL == vector->storage_) {
    return kErrorCode_MemoryNode_Null;
  }

  int j = vector->head_;
  for (int i = 0; i < vector->capacity_; ++i) {
    vector->storage_->ops_->reset(vector->storage_ + j);
    ++j;
    j %= vector->capacity_;
  }

  vector->tail_ = 0;
  vector->head_ = 0;
  return kErrorCode_Ok;
}

s16 CIRCULAR_VECTOR_reset(CircularVector* vector) {
  if (NULL == vector) {
    return kErrorCode_VectorC_Null;
  }
  if (NULL == vector->storage_) {
    return kErrorCode_MemoryNode_Null;
  }

  int j = vector->head_;
  for (int i = 0; i < vector->capacity_; ++i) {
    vector->storage_->ops_->reset(vector->storage_ + j);
    ++j;
    j %= vector->capacity_;
  }

  vector->tail_ = 0;
  vector->head_ = 0;
  return kErrorCode_Ok;
}

s16 CIRCULAR_VECTOR_resize(CircularVector* vector, u16 new_size) {

  if (NULL == vector) {
    return kErrorCode_VectorC_Null;
  }  
  if (NULL == vector->storage_) {
    return kErrorCode_MemoryNode_Null;
  }
  if (0 == new_size) {
    return kErrorCode_ZeroSize;
  }

  if (new_size == vector->capacity_) {

    return kErrorCode_Ok;

  }
  else if(new_size > vector->capacity_) { //Bigger

    MemoryNode* aux_storage = MM->malloc(sizeof(MemoryNode)*new_size);
    if (NULL == aux_storage) {
      return KErrorCode_No_Memory;
    }

    for (int i = 0; i < new_size; ++i) {
      MEMNODE_createLite((aux_storage+i));
    }

    for (int i = 0; i < vector->tail_; ++i) {
      int vector_position = i + vector->head_;
      vector_position %= vector->capacity_;

      aux_storage->ops_->setData(aux_storage + i,
                                (vector->storage_ + vector_position)->data_,
                                (vector->storage_ + vector_position)->size_);
    }
   
    MM->free(vector->storage_);
    
    vector->storage_ = aux_storage;
    vector->capacity_ = new_size;
  }
  else if(new_size < vector->capacity_){ //Smaller

    MemoryNode* aux_storage = MM->malloc(sizeof(MemoryNode)*new_size);
    if (NULL == aux_storage) {
      return KErrorCode_No_Memory;
    }
    
    if (new_size < vector->tail_) {
      vector->tail_ = new_size;
    }
      
    for (int i = 0; i < new_size; ++i) {
      MEMNODE_createLite((aux_storage + i));
      int vector_position = i + vector->head_;
      vector_position %= vector->capacity_;

      aux_storage->ops_->setData(aux_storage + i,
                                (vector->storage_ + vector_position)->data_,
                                (vector->storage_ + vector_position)->size_);
    }

    for (int i = new_size; i < vector->capacity_; ++i) {
      int reset_postition = i + vector->head_;
      reset_postition %= vector->capacity_;
      vector->storage_->ops_->reset(vector->storage_ + reset_postition);
    }
  
    
    MM->free(vector->storage_);
    
    vector->storage_ = aux_storage;
    vector->capacity_ = new_size;
  }

   return kErrorCode_Ok;
}

u16 CIRCULAR_VECTOR_capacity(CircularVector* vector) {
  if (NULL == vector || 0 == vector->capacity_ ) {
    return 0;
  }
  return vector->capacity_;
}



u16 CIRCULAR_VECTOR_length(CircularVector* vector) {
  if (NULL == vector || 0 == vector->capacity_) {
    return 0;
  }
  if (vector->tail_ == vector->head_ ) {
    return 0;
  }
  if (vector->tail_ > vector->head_) {
    return vector->tail_ - vector->head_;
  }
  return vector->capacity_ - vector->head_ + vector->tail_;
  
}

bool CIRCULAR_VECTOR_isEmpty(CircularVector* vector) {
  if (NULL == vector) {
    return false;
  }

  if (vector->tail_ == vector->head_) {
    vector->tail_ = 0;
    vector->head_ = 0;
    return true;
  }
  return false;
}

bool CIRCULAR_VECTOR_isFull(CircularVector* vector) {
  if (NULL == vector) {
    return false;
    
  }
  return (vector->ops_->length(vector) == vector->capacity_);
}

void* CIRCULAR_VECTOR_first(CircularVector* vector) {
  
  if (NULL == vector) {
    return NULL;
  }
  if (NULL == vector->storage_) {
    return NULL;
  }
  if (vector->ops_->isEmpty(vector)) {
    return NULL;
  }


  return vector->storage_->ops_->data(vector->storage_ + vector->head_);
}

void* CIRCULAR_VECTOR_last(CircularVector* vector) {

  if (NULL == vector) {
    return NULL;
  }
  if (NULL == vector->storage_) {
    return NULL;
  }
  if (vector->ops_->isEmpty(vector)) {
    return NULL;
  }
  

  return vector->storage_->ops_->data(vector->storage_ + vector->tail_-1);
}

void* CIRCULAR_VECTOR_at(CircularVector* vector, u16 position) {

  if (NULL == vector) {
    return NULL;
  }
  if (NULL == vector->storage_) {
    return NULL;
  }
  if (vector->ops_->isEmpty(vector)) {
    return NULL;
  }
  if (position >= vector->capacity_) {
    return NULL;
  }
  int next_position = vector->head_ + position;
  next_position %= vector->capacity_;
  return vector->storage_->ops_->data(vector->storage_ + next_position);
}

s16 CIRCULAR_VECTOR_insertFirst(CircularVector* vector, void* data, u16 bytes) {

  if (NULL == vector) {
    return kErrorCode_VectorC_Null;
  }
  if (NULL == vector->storage_) {
    return kErrorCode_MemoryNode_Null;
  }
  if (NULL == data) {
    return kErrorCode_Data_Null;
  }
  if (vector->ops_->isFull(vector)) {
    return kErrorCode_Vector_Full;
  }
  if (bytes == 0) {
    return kErrorCode_ZeroSize;
  }

  
  --vector->head_;

  if (vector->head_ > vector->capacity_) {
    
    vector->head_ = vector->capacity_ - 1 ;
    
  }
  vector->storage_->ops_->setData(vector->storage_ + vector->head_, data, bytes);
  
  return kErrorCode_Ok;

}

s16 CIRCULAR_VECTOR_insertLast(CircularVector* vector, void* data, u16 bytes) {

  if (NULL == vector) {
    return kErrorCode_VectorC_Null;
  }
  if (NULL == vector->storage_) {
    return kErrorCode_MemoryNode_Null;
  }
  if (NULL == data) {
    return kErrorCode_Data_Null;
  }
  if (vector->ops_->isFull(vector)) {
    return kErrorCode_Vector_Full;
  }
  if (bytes == 0) {
    return kErrorCode_ZeroSize;
  }

  vector->storage_->ops_->setData(vector->storage_ + vector->tail_, data, bytes);
  ++vector->tail_;
  return kErrorCode_Ok;
}

s16 CIRCULAR_VECTOR_insertAt(CircularVector* vector, void* data, u16 bytes, u16 position) {
  if (NULL == vector) {
    return kErrorCode_VectorC_Null;
  }
  if (NULL == vector->storage_) {
    return kErrorCode_MemoryNode_Null;
  }
  if (NULL == data) {
    return kErrorCode_Data_Null;
  }
  if (vector->ops_->isFull(vector)) {
    return kErrorCode_Vector_Full;
  }
  if (bytes == 0) {
    return kErrorCode_ZeroSize;
  }
  //Insert First
  if (position == 0) {
    vector->ops_->insertFirst(vector, data, bytes);
  }

  //Insert Last
  else if (position == vector->ops_->length(vector)) {
    vector->ops_->insertLast(vector, data, bytes);
  }
  // Insert others
  else {

    
    int next_position = vector->head_ + position;
    next_position %= vector->capacity_;
    int  len = vector->ops_->length(vector);

    if (next_position > len || next_position > vector->tail_) {
      next_position = vector->tail_;
    }

    int lim = len - position;

    for (int i = 0; i < lim; ++i) {

      int iaux = vector->tail_ - i;
      int iaux2 = iaux - 1;
      if (iaux < 0) {
        iaux = vector->capacity_ - 1;
      }
      if (iaux2 < 0) {
        iaux2 = vector->capacity_ - 1;
      }
      vector->storage_->ops_->setData(vector->storage_ + iaux,
        (vector->storage_ + iaux2)->data_,
        (vector->storage_ + iaux2)->size_);
    }

    vector->storage_->ops_->setData((vector->storage_ + next_position), data, bytes);
    ++vector->tail_;
  }
  return kErrorCode_Ok;

}

void* CIRCULAR_VECTOR_extractFirst(CircularVector* vector) {
    
  if (NULL == vector) {
    return NULL;
  }
  if (NULL == vector->storage_) {
    return NULL;
  }

  if (vector->ops_->isEmpty(vector)) {
    return NULL;
  }

  void* aux_node = vector->storage_->ops_->data(vector->storage_ + vector->head_);
  
  vector->storage_->ops_->softReset(vector->storage_ + vector->head_);
  ++vector->head_;
  vector->head_ %= vector->capacity_;
  return aux_node;

}

void* CIRCULAR_VECTOR_extractLast(CircularVector* vector) {

  if (NULL == vector) {
    return NULL;
  }
  if (NULL == vector->storage_) {
    return NULL;
  }
  if (vector->ops_->isEmpty(vector)) {
    return NULL;
  }

  void* aux_node = vector->storage_->ops_->data(vector->storage_ + vector->tail_ - 1);
 
  --vector->tail_;
  vector->storage_->ops_->softReset(vector->storage_ + vector->tail_);
  return aux_node;

}

void* CIRCULAR_VECTOR_extractAt(CircularVector* vector, u16 position) {

  if (NULL == vector) {
    return NULL;
  }
  if (NULL == vector->storage_) {
    return NULL;
  }
  if (position > vector->ops_->length(vector)) {
    return NULL;
  }
  if (vector->ops_->isEmpty(vector)) {
    return NULL;
  }
  // First
  if (position == 0) {
    return vector->ops_->extractFirst(vector);
  }
  //Last
  else if (position == vector->ops_->length(vector)) {
    return vector->ops_->extractLast(vector);
  }

  //Others
  else {
    //Next position
    int next_position = vector->head_ + position;
    next_position %= vector->capacity_;

    int  v_len = vector->ops_->length(vector);
    void* aux_node = vector->storage_->ops_->data(vector->storage_ + next_position);
    
    //nearest to tail
   // if (position > v_len * 0.5) {
      for (int i = position; i < v_len; ++i) {

        int iaux = i;
        int iaux1 = iaux + 1;

        iaux %= vector->capacity_;
        iaux1 %= vector->capacity_;

        vector->storage_->ops_->setData(vector->storage_ + iaux,
          (vector->storage_ + iaux1)->data_,
          (vector->storage_ + iaux1)->size_);
      }

      --vector->tail_;
    vector->storage_->ops_->softReset(vector->storage_ + vector->tail_);
   /*}
    else {
      for (int i = position ; i >0 ; --i) {

        int iaux = i + vector->head_;
        iaux %= vector->capacity_;
        int iaux1 = iaux - 1;
        if (iaux1 < 0) {
          iaux1 = vector->capacity_ - 1;
        }
        vector->storage_->ops_->setData(vector->storage_ + i,
          (vector->storage_ + iaux)->data_,
          (vector->storage_ + iaux)->size_);
      }

      //vector->storage_->ops_->softReset(vector->storage_ + vector->head_);
      ++vector->head_;
      vector->head_%= vector->capacity_;
    }*/

 
   
    return aux_node;
  }

}

s16 CIRCULAR_VECTOR_concat(CircularVector* vector, CircularVector* vector_src) {

  if (NULL == vector || NULL == vector_src) {
    return kErrorCode_VectorC_Null;
  }
  if (NULL == vector->storage_ || NULL == vector_src->storage_) {
    return kErrorCode_MemoryNode_Null;
  }
  int new_capacity = vector->capacity_ + vector_src->capacity_;

  
  MemoryNode* aux_node = MM->malloc(sizeof(MemoryNode) * new_capacity);
  if (NULL == aux_node) {
    return KErrorCode_No_Memory;
  }
  
  for (int i = 0; i < new_capacity; ++i) {
    MEMNODE_createLite((aux_node+i));
  }

  int v1_len = vector->ops_->length(vector);

  for (int i = 0; i < v1_len; ++i) {
    int auxi = i + vector->head_;
    auxi %= vector->capacity_;

    aux_node->ops_->memCopy((aux_node + i ), 
                            (vector->storage_ + auxi)->data_, 
                            (vector->storage_ + auxi)->size_);
  }
 
  for (int i = 0; i < vector_src->ops_->length(vector_src); ++i) {
    int auxi = i + vector_src->head_;
    auxi %= vector_src->capacity_;

    aux_node->ops_->memCopy((aux_node + i + v1_len) ,
                            (vector_src->storage_ + auxi)->data_,
                            (vector_src->storage_ + auxi)->size_);
  }

  int aux_tail = v1_len;
  vector->ops_->reset(vector);
  vector->tail_ = vector_src->ops_->length(vector_src) + aux_tail;
 
  MM->free(vector->storage_);
  vector->storage_ = aux_node;
  vector->capacity_ = new_capacity;

  return kErrorCode_Ok;

}

s16 CIRCULAR_VECTOR_traverse(CircularVector* vector, void(*callback)(MemoryNode *)) {
  if (NULL == vector) {
    return kErrorCode_VectorC_Null;
  }
  if (NULL == vector->storage_) {
    return kErrorCode_MemoryNode_Null;
  }
  if (NULL == callback) {
    return kErrorCode_NonFuction;
  }

  //Go to all storage of the vector not null
  int length = vector->ops_->length(vector);

  for (int i = 0; i < length; ++i) {
    int next_pos = i;
    next_pos %= vector->capacity_;
    callback(vector->storage_ + next_pos);
  }

  return kErrorCode_Ok; 
}

s16 CIRCULAR_VECTOR_print(CircularVector* vector){

  if (NULL != vector) {
    printf("[Circular Vector Info] Address: %p\n", vector);
    printf("[Circular Vector Info] Head: %d\n", vector->head_);
    printf("[Circular Vector Info] Tail: %d\n", vector->tail_);
    printf("[Circular Vector Info] Length: %d\n", vector->ops_->length(vector));
    printf("[Circular Vector Info] Capaciy: %d\n", vector->capacity_);
    printf("[Circular Vector Info] Data Address: %p\n", vector->storage_);

   int length = vector->ops_->length(vector);
    //int length = vector->capacity_;

    for (int i = 0; i < length; ++i) {
      printf("  ");
     
      int next_pos = vector->head_ + i;
      next_pos %= vector->capacity_;
      printf("[Circular Vector Info] Storage %d#\n", next_pos);
      vector->storage_->ops_->print(vector->storage_ + next_pos);
    }
  }
  else {
    printf("[Circular Vector Info] Address: NULL");
  }
}

CircularVector* CIRCULAR_VECTOR_create(u16 capacity) {
  if (0 == capacity) {
    return NULL;
  }

  CircularVector* new_vector = MM->malloc(sizeof(CircularVector));
  if (NULL == new_vector) {
    return NULL;
  }
  new_vector->head_ = 0;
  new_vector->tail_ = 0;

  new_vector->capacity_ = capacity;
  new_vector->ops_ = &circular_vector_ops_s;
  new_vector->storage_ = MM->malloc(sizeof(MemoryNode)*capacity);
  if (NULL == new_vector->storage_) {
    return NULL;
  }
 
  for (int i = 0; i < capacity; ++i) {
    MEMNODE_createLite((new_vector->storage_+i));
  }
  return new_vector;
}