#include <stdio.h>
#include <stdlib.h>


#include "common_def.h"
#include "adt_movable_head_vector.h"
#include "ABGS_MemoryManager/abgs_memory_manager.h"

//Vector Declaratoon
static s16 MOVABLE_HEAD_VECTOR_destroy(MovableHeadVector* vector);
static s16 MOVABLE_HEAD_VECTOR_softReset(MovableHeadVector* vector);
static s16 MOVABLE_HEAD_VECTOR_reset(MovableHeadVector* vector);
static s16 MOVABLE_HEAD_VECTOR_resize(MovableHeadVector* vector, u16 new_size);

static u16 MOVABLE_HEAD_VECTOR_capacity(MovableHeadVector* vector);
static u16 MOVABLE_HEAD_VECTOR_length(MovableHeadVector* vector);
static bool MOVABLE_HEAD_VECTOR_isEmpty(MovableHeadVector* vector);
static bool MOVABLE_HEAD_VECTOR_isFull(MovableHeadVector* vector);

static void* MOVABLE_HEAD_VECTOR_first(MovableHeadVector* vector);
static void* MOVABLE_HEAD_VECTOR_last(MovableHeadVector* vector);
static void* MOVABLE_HEAD_VECTOR_at(MovableHeadVector* vector, u16 position);

static s16 MOVABLE_HEAD_VECTOR_insertFirst(MovableHeadVector* vector, void* data, u16 bytes);
static s16 MOVABLE_HEAD_VECTOR_insertLast(MovableHeadVector* vector, void* data, u16 bytes);
static s16 MOVABLE_HEAD_VECTOR_insertAt(MovableHeadVector* vector, void* data, u16 bytes, u16 position);

static void* MOVABLE_HEAD_VECTOR_extractFirst(MovableHeadVector* vector);
static void* MOVABLE_HEAD_VECTOR_extractLast(MovableHeadVector* vector);
static void* MOVABLE_HEAD_VECTOR_extractAt(MovableHeadVector* vector, u16 position);

static s16 MOVABLE_HEAD_VECTOR_concat(MovableHeadVector* vector, MovableHeadVector* MOVABLE_HEAD_VECTOR_src);
static s16 MOVABLE_HEAD_VECTOR_traverse(MovableHeadVector* vector, void(*callback)(MemoryNode *));
static s16 MOVABLE_HEAD_VECTOR_print(MovableHeadVector* vector);




// Vector's API Definitions
struct movable_head_vector_ops_s movable_head_vector_ops_s = { .destroy = MOVABLE_HEAD_VECTOR_destroy,
                                     .softReset = MOVABLE_HEAD_VECTOR_softReset,
                                     .reset = MOVABLE_HEAD_VECTOR_reset,
                                     .resize = MOVABLE_HEAD_VECTOR_resize,
                                     .capacity = MOVABLE_HEAD_VECTOR_capacity,
                                     .length = MOVABLE_HEAD_VECTOR_length, 
                                     .isEmpty = MOVABLE_HEAD_VECTOR_isEmpty,
                                     .isFull = MOVABLE_HEAD_VECTOR_isFull,
                                     .first = MOVABLE_HEAD_VECTOR_first,
                                     .last = MOVABLE_HEAD_VECTOR_last,
                                     .at = MOVABLE_HEAD_VECTOR_at,
                                     .insertFirst = MOVABLE_HEAD_VECTOR_insertFirst,
                                     .insertLast = MOVABLE_HEAD_VECTOR_insertLast,
                                     .insertAt = MOVABLE_HEAD_VECTOR_insertAt,
                                     .extractFirst = MOVABLE_HEAD_VECTOR_extractFirst,
                                     .extractLast = MOVABLE_HEAD_VECTOR_extractLast,
                                     .extractAt = MOVABLE_HEAD_VECTOR_extractAt,
                                     .concat = MOVABLE_HEAD_VECTOR_concat,
                                     .traverse = MOVABLE_HEAD_VECTOR_traverse,
                                     .print = MOVABLE_HEAD_VECTOR_print             
};


// Vector Definitions

s16 MOVABLE_HEAD_VECTOR_destroy(MovableHeadVector* vector){
  if (NULL == vector) {
    return kErrorCode_VectorMH_Null;
  }
  if (NULL == vector->storage_) {
    MM->free(vector);
    return kErrorCode_MemoryNode_Null;
  }
  for (int i = vector->head_; i < vector->tail_; ++i) {
    vector->storage_->ops_->reset(vector->storage_ + i);
  }

  MM->free(vector->storage_);
  MM->free(vector);
  return kErrorCode_Ok;
}

s16 MOVABLE_HEAD_VECTOR_softReset(MovableHeadVector* vector) {
  if (NULL == vector) {
    return kErrorCode_VectorMH_Null;
  }
  if (NULL == vector->storage_) {
    return kErrorCode_MemoryNode_Null;
  }
  for (int i = vector->head_; i < vector->tail_; ++i) {
    vector->storage_->ops_->softReset(vector->storage_ + i);
  }
  vector->tail_ = vector->capacity_;
  vector->head_ = vector->capacity_;
  return kErrorCode_Ok;
}

s16 MOVABLE_HEAD_VECTOR_reset(MovableHeadVector* vector) {
  if (NULL == vector) {
    return kErrorCode_VectorMH_Null;
  }
  if (NULL == vector->storage_) {
    return kErrorCode_MemoryNode_Null;
  }
  for (int i = vector->head_; i < vector->tail_; ++i) {
    vector->storage_->ops_->reset(vector->storage_ + i);
  }
  vector->tail_ = vector->capacity_;
  vector->head_ = vector->capacity_;
  return kErrorCode_Ok;
}

s16 MOVABLE_HEAD_VECTOR_resize(MovableHeadVector* vector, u16 new_size) {

  if (NULL == vector) {
    return kErrorCode_VectorMH_Null;
  }  
  if (NULL == vector->storage_) {
    return kErrorCode_MemoryNode_Null;
  }
  if (0 == new_size) {
    return kErrorCode_ZeroSize;
  }
  if (new_size == vector->capacity_) {

    return kErrorCode_Ok;

  }else if(new_size > vector->capacity_) { //Bigger

    MemoryNode* aux_storage = MM->malloc(sizeof(MemoryNode)*new_size*2);
    if (NULL == aux_storage) {
      return KErrorCode_No_Memory;
    }
    for (int i = 0; i < new_size*2; ++i) {
      MEMNODE_createLite((aux_storage+i));
    }

    int old_length = vector->ops_->length(vector) ;

    for (int i = 0; i < old_length; ++i) {
      aux_storage->ops_->setData(aux_storage + i + (new_size - old_length/2), 
                                (vector->storage_ + i + vector->head_)->data_,
                                (vector->storage_ + i + vector->head_)->size_);
    }
   
    MM->free(vector->storage_);
    
    vector->storage_ = aux_storage;
    vector->capacity_ = new_size;
    vector->head_ = new_size - old_length / 2;
    vector->tail_ = vector->head_ + old_length;
  }
  else if(new_size < vector->capacity_){ //Smaller

    MemoryNode* aux_storage = MM->malloc(sizeof(MemoryNode)*new_size*2);
    if (NULL == aux_storage) {
      return KErrorCode_No_Memory;
    }
    int old_lenght = vector->ops_->length(vector);
    int new_head = new_size - old_lenght * 0.5f;
    int new_tail = old_lenght;

    if (old_lenght > new_size) {
      new_head = new_size - 1;
      new_tail = new_size;
    }
    
    for (int i = 0; i < 2*new_size; ++i) {
      MEMNODE_createLite((aux_storage + i));
    }
      
    for (int i = 0; i < new_size; ++i) {
      aux_storage->ops_->setData(aux_storage + i + new_head,
                                (vector->storage_ + i + vector->head_)->data_,
                                (vector->storage_ + i + vector->head_)->size_);
    }

    for (int i = new_size ; i < vector->capacity_; ++i) {
      vector->storage_->ops_->reset(vector->storage_ + i + vector->head_);
    }
    
    
    MM->free(vector->storage_);
    
    vector->storage_ = aux_storage;
    vector->capacity_ = new_size;
    vector->head_ = new_head;
    vector->tail_ = vector->head_ + new_tail;
  }

   return kErrorCode_Ok;
}

u16 MOVABLE_HEAD_VECTOR_capacity(MovableHeadVector* vector) {
  if (NULL == vector || 0 == vector->capacity_ ) {
    return 0;
  }
  return vector->capacity_;
}

u16 MOVABLE_HEAD_VECTOR_length(MovableHeadVector* vector) {
  if (NULL == vector || 0 == vector->capacity_) {
    return 0;
  }
  return (vector->tail_-vector->head_);
}

bool MOVABLE_HEAD_VECTOR_isEmpty(MovableHeadVector* vector) {
  if (NULL == vector) {
    return false;
  }
  return vector->tail_ == vector->head_;
}

bool MOVABLE_HEAD_VECTOR_isFull(MovableHeadVector* vector) {
  if (NULL == vector) {
    return false;
  }
  return vector->ops_->length(vector) == vector->capacity_;
   
  
}

void* MOVABLE_HEAD_VECTOR_first(MovableHeadVector* vector) {
  
  if (NULL == vector) {
    return NULL;
  }
  if (NULL == vector->storage_) {
    return NULL;
  }

  return vector->storage_->ops_->data(vector->storage_+ vector->head_);
}

void* MOVABLE_HEAD_VECTOR_last(MovableHeadVector* vector) {

  if (NULL == vector) {
    return NULL;
  }
  if (vector->tail_ == 0) {
    return NULL;
  }
  if (NULL == vector->storage_) {
    return NULL;
  }

  return vector->storage_->ops_->data(vector->storage_ + vector->tail_-1);
  
}

void* MOVABLE_HEAD_VECTOR_at(MovableHeadVector* vector, u16 position) {

  if (NULL == vector) {
    return NULL;
  }
  if (NULL == vector->storage_) {
    return NULL;
  }
  if (position >= vector->ops_->length(vector)) {
    return NULL;
  }

  return vector->storage_->ops_->data(vector->storage_ + vector->head_ + position);
}

s16 MOVABLE_HEAD_VECTOR_insertFirst(MovableHeadVector* vector, void* data, u16 bytes) {

  if (NULL == vector) {
    return kErrorCode_VectorMH_Null;
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
  
  //printf("%d", vector->head_ );
  --vector->head_;
  //printf("%d", vector->head_ );

  if (vector->head_ < 0) {
    //New position
    int length = vector->ops_->length(vector);
    vector->head_ = vector->capacity_ - (length /2);
    vector->tail_ = vector->head_ + length / 2;
    //Move
    for (int i = length;  i > 0; --i) {

      vector->storage_->ops_->setData(vector->storage_ + i,
                                     (vector->storage_ + i + vector->head_)->data_,
                                     (vector->storage_ + i + vector->head_)->size_);
    }
  }
  vector->storage_->ops_->setData(vector->storage_ + vector->head_, data, bytes);
  return kErrorCode_Ok;

}

s16 MOVABLE_HEAD_VECTOR_insertLast(MovableHeadVector* vector, void* data, u16 bytes) {

  if (NULL == vector) {
    return kErrorCode_VectorMH_Null;
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

  if (vector->tail_ >= vector->capacity_*2) {
    //New position
    int length = vector->ops_->length(vector);
    int new_head = vector->capacity_ - (length / 2);
    vector->tail_ = new_head + length ;
    //Move
    for (int i = 0; i < length ; ++i) {

      vector->storage_->ops_->setData(vector->storage_ + i + new_head,
                                     (vector->storage_ + i + vector->head_)->data_,
                                     (vector->storage_ + i + vector->head_)->size_);
    }
    vector->head_ = new_head;
  }

  vector->storage_->ops_->setData(vector->storage_ + vector->tail_, data, bytes);
  ++vector->tail_;
  return kErrorCode_Ok;
}

s16 MOVABLE_HEAD_VECTOR_insertAt(MovableHeadVector* vector, void* data, u16 bytes, u16 position) {
  if (NULL == vector) {
    return kErrorCode_VectorMH_Null;
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
  if (position > vector->ops_->length(vector)) {
    position = vector->tail_;
  }

  if (vector->head_ < 0) {
    //New position
    int length = vector->ops_->length(vector);
    vector->head_ = vector->capacity_ - (length / 2);
    vector->tail_ = vector->head_ + length / 2;
    //Move
    for (int i = length; i > 0; --i) {

      vector->storage_->ops_->setData(vector->storage_ + i,
        (vector->storage_ + i + vector->head_)->data_,
        (vector->storage_ + i + vector->head_)->size_);
    }
  }

  if (vector->tail_ >= vector->capacity_ * 2) {
    //New position
    int length = vector->ops_->length(vector);
    vector->head_ = vector->capacity_ - (length / 2);
    vector->tail_ = vector->head_ + length / 2;
    //Move
    for (int i = 0; i < length; ++i) {

      vector->storage_->ops_->setData(vector->storage_ + i,
        (vector->storage_ + i + vector->head_)->data_,
        (vector->storage_ + i + vector->head_)->size_);
    }
  }

  if (position == 0) {
    vector->ops_->insertFirst(vector, data, bytes);
  }
  else if (position == vector->tail_) {
    vector->ops_->insertLast(vector, data, bytes);
  }

  else{
    //nearer to tail
    if (position > vector->capacity_ / 2) {
      for (int i = vector->tail_; i > position; --i) {
          vector->storage_->ops_->setData(vector->storage_ + i,
                                         (vector->storage_ + i - 1)->data_,
                                         (vector->storage_ + i - 1)->size_);
      }
      vector->storage_->ops_->setData((vector->storage_ + vector->head_ + position), data, bytes);
      ++vector->tail_;
    }
    //nearer to head
    else {
      for (int i = 0 ; i < position; ++i) {
        vector->storage_->ops_->setData(vector->storage_ + vector->head_- 1 + i,
                                       (vector->storage_ + vector->head_ + i)->data_,
                                       (vector->storage_ + vector->head_ + i)->size_);
      }
      --vector->head_;
      vector->storage_->ops_->setData((vector->storage_ + vector->head_ + position), data, bytes);
    }

   
  }
  return kErrorCode_Ok;

}

void* MOVABLE_HEAD_VECTOR_extractFirst(MovableHeadVector* vector) {
    
  if (NULL == vector) {
    return NULL;
  }
  if (NULL == vector->storage_) {
    return NULL;
  }
  if (vector->tail_ == 0) {
    return NULL;
  }

  void* aux_node = vector->storage_->ops_->data(vector->storage_ + vector->head_);
  
  vector->storage_->ops_->softReset(vector->storage_ + vector->head_);
  vector->head_++;
  
  if (vector->ops_->isEmpty(vector)) {
    vector->head_ = vector->capacity_;
    vector->tail_ = vector->head_;
  }
  
  return aux_node;

}

void* MOVABLE_HEAD_VECTOR_extractLast(MovableHeadVector* vector) {

  if (NULL == vector) {
    return NULL;
  }
  if (NULL == vector->storage_) {
    return NULL;
  }
  if (vector->tail_ == 0) {
    return NULL;
  }

  void* aux_node = vector->storage_->ops_->data(vector->storage_ + vector->tail_ - 1);
 
  --vector->tail_;
  vector->storage_->ops_->softReset(vector->storage_ + vector->tail_);
  
  if (vector->ops_->isEmpty(vector)) {
    vector->head_ = vector->capacity_;
    vector->tail_ = vector->head_;
  }

  return aux_node;

}

void* MOVABLE_HEAD_VECTOR_extractAt(MovableHeadVector* vector, u16 position) {

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
  
  void* aux_node = vector->storage_->ops_->data(vector->storage_ + vector->head_ + position);
  /*for (int i = position + vector->head_; i < vector->tail_; ++i) {
    vector->storage_->ops_->setData(vector->storage_ + i,
      (vector->storage_ + i + 1)->data_,
      (vector->storage_ + i + 1)->size_);
  }

  --vector->tail_;
  vector->storage_->ops_->softReset(vector->storage_ + vector->tail_);*/
  
  if (position > vector->capacity_ * 0.5) {
    for (int i = position + vector->head_; i < vector->tail_; ++i) {
      vector->storage_->ops_->setData(vector->storage_ + i,
        (vector->storage_ + i + 1)->data_,
        (vector->storage_ + i + 1)->size_);
    }

    vector->storage_->ops_->softReset(vector->storage_ + vector->tail_);
    --vector->tail_;
  }
  else {
    for (int i = position + vector->head_; i > vector->head_; --i) {
      vector->storage_->ops_->setData(vector->storage_ + i,
        (vector->storage_ + i - 1)->data_,
        (vector->storage_ + i - 1)->size_);
    }

    vector->storage_->ops_->softReset(vector->storage_ + vector->head_);
    ++vector->head_;
  }

  if (vector->ops_->isEmpty(vector)) {
    vector->head_ = vector->capacity_;
    vector->tail_ = vector->head_;
  }

  return aux_node;
}

s16 MOVABLE_HEAD_VECTOR_concat(MovableHeadVector* vector, MovableHeadVector* vector_src) {

  if (NULL == vector || NULL == vector_src) {
    return kErrorCode_VectorMH_Null;
  }
  if (NULL == vector->storage_ || NULL == vector_src->storage_) {
    return kErrorCode_MemoryNode_Null;
  }
  vector->capacity_ += vector_src->capacity_;

  MemoryNode* aux_node = MM->malloc(2*sizeof(MemoryNode) * vector->capacity_);
  if (NULL == aux_node) {
    return KErrorCode_No_Memory;
  }

  int total_length = vector->ops_->length(vector) + 
                     vector->ops_->length(vector_src);
  int new_head = vector->capacity_ - total_length * 0.5;

  for (int i = 0; i < 2*vector->capacity_; ++i) {
    MEMNODE_createLite((aux_node+i));
  }
  //Copy first vector
  for (int i = 0 ; i < vector->ops_->length(vector); ++i) {
   
    aux_node->ops_->memCopy((aux_node + i + new_head ), 
                            (vector->storage_ + vector->head_ + i)->data_, 
                            (vector->storage_ + vector->head_ + i)->size_);
  }
 

  //Copy Second vector
  for (int i = 0; i < vector->ops_->length(vector_src); ++i) {

    aux_node->ops_->memCopy((aux_node + i + new_head + vector->ops_->length(vector)) ,
                            (vector_src->storage_ + vector_src->head_ + i)->data_,
                            (vector_src->storage_ + vector_src->head_ + i)->size_);
 
  }


  vector->ops_->reset(vector);
  vector->head_ = new_head;
  vector->tail_ = new_head + total_length;
  

  MM->free(vector->storage_);
  vector->storage_ = aux_node;

  return kErrorCode_Ok;

}

s16 MOVABLE_HEAD_VECTOR_traverse(MovableHeadVector* vector, void(*callback)(MemoryNode *)) {
  if (NULL == vector) {
    return kErrorCode_VectorMH_Null;
  }
  if (NULL == vector->storage_) {
    return kErrorCode_MemoryNode_Null;
  }
  if (NULL == callback) {
    return kErrorCode_NonFuction;
  }

  //Go to all storage of the vector not null
  for (int i = vector->head_; i < vector->tail_; ++i) {
    callback(vector->storage_ + i);
  }

  return kErrorCode_Ok; 
}

s16 MOVABLE_HEAD_VECTOR_print(MovableHeadVector* vector){

  if (NULL != vector) {
    printf("[Movable Head Vector Info] Address: %p\n", vector);
    printf("[Movable Head Vector Info] Head: %d\n", vector->head_);
    printf("[Movable Head Vector Info] Tail: %d\n", vector->tail_);
    printf("[Movable Head Vector Info] Length: %d\n", vector->ops_->length(vector));
    printf("[Movable Head Vector Info] Capacity: %d\n", vector->capacity_);
    printf("[Movable Head Vector Info] Data Address: %p\n", vector->storage_);

    for (int i = vector->head_; i < vector->tail_; ++i) {
   
      printf("  ");
      printf("[Movable Head Vector Info] Storage %d#\n", i);
      
      vector->storage_->ops_->print(vector->storage_ + i);
    }
  }
  else {
    printf("[Movable Head Vector Info] Address: NULL");
  }
}

MovableHeadVector* MOVABLE_HEAD_VECTOR_create(u16 capacity) {
  if (0 == capacity) {
    return NULL;
  }
  MovableHeadVector* new_vector = MM->malloc(sizeof(MovableHeadVector));
  if (NULL == new_vector) {
    return NULL;
  }
  new_vector->head_ = capacity ;
  new_vector->tail_ = capacity ;

  new_vector->capacity_ = capacity;
  new_vector->ops_ = &movable_head_vector_ops_s;
  new_vector->storage_ = MM->malloc(2*sizeof(MemoryNode)*capacity);
  if (NULL == new_vector->storage_) {
    return NULL;
  }
 
  for (int i = 0; i < capacity*2; ++i) {
    MEMNODE_createLite((new_vector->storage_ + i));
  }
  return new_vector;
}