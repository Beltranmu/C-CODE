#include <stdio.h>
#include <stdlib.h>


#include "common_def.h"
#include "adt_vector.h"
#include "ABGS_MemoryManager/abgs_memory_manager.h"

//Vector Declaratoon
static s16 VECTOR_destroy(Vector* vector);
static s16 VECTOR_softReset(Vector* vector);
static s16 VECTOR_reset(Vector* vector);
static s16 VECTOR_resize(Vector* vector, u16 new_size);

static u16 VECTOR_capacity(Vector* vector);
static u16 VECTOR_length(Vector* vector);
static boolean VECTOR_isEmpty(Vector* vector);
static boolean VECTOR_isFull(Vector* vector);

static void* VECTOR_first(Vector* vector);
static void* VECTOR_last(Vector* vector);
static void* VECTOR_at(Vector* vector, u16 position);

static s16 VECTOR_insertFirst(Vector* vector, void* data, u16 bytes);
static s16 VECTOR_insertLast(Vector* vector, void* data, u16 bytes);
static s16 VECTOR_insertAt(Vector* vector, void* data, u16 bytes, u16 position);

static void* VECTOR_extractFirst(Vector* vector);
static void* VECTOR_extractLast(Vector* vector);
static void* VECTOR_extractAt(Vector* vector, u16 position);

static s16 VECTOR_concat(Vector* vector, Vector* vector_src);
static s16 VECTOR_traverse(Vector* vector, void(*callback)(MemoryNode *));
static s16 VECTOR_print(Vector* vector);




// Vector's API Definitions
struct vector_ops_s vector_ops_s = { .destroy = VECTOR_destroy,
                                     .softReset = VECTOR_softReset,
                                     .reset = VECTOR_reset,
                                     .resize = VECTOR_resize,
                                     .capacity = VECTOR_capacity,
                                     .length = VECTOR_length, 
                                     .isEmpty = VECTOR_isEmpty,
                                     .isFull = VECTOR_isFull,
                                     .first = VECTOR_first,
                                     .last = VECTOR_last,
                                     .at = VECTOR_at,
                                     .insertFirst = VECTOR_insertFirst,
                                     .insertLast = VECTOR_insertLast,
                                     .insertAt = VECTOR_insertAt,
                                     .extractFirst = VECTOR_extractFirst,
                                     .extractLast = VECTOR_extractLast,
                                     .extractAt = VECTOR_extractAt,
                                     .concat = VECTOR_concat,
                                     .traverse = VECTOR_traverse,
                                     .print = VECTOR_print             
};

// CreateLite Definition


// Vector Definitions

s16 VECTOR_destroy(Vector* vector){
  if (NULL == vector) {
    return kErrorCode_Vector_Null;
  }
  if (NULL == vector->storage_) {
    MM->free(vector);
    return kErrorCode_Memory;
  }
  for (int i = vector->tail_-1; i>=vector->head_ ; --i) {
      vector->storage_->ops_->reset(vector->storage_ + i);
  }

  MM->free(vector->storage_);
  MM->free(vector);
  return kErrorCode_Ok;
}

s16 VECTOR_softReset(Vector* vector) {
  if (NULL == vector) {
    return kErrorCode_Vector_Null;
  }
  if (NULL == vector->storage_) {
    return kErrorCode_Memory;
  }
  for (int i = vector->tail_ - 1; i >= vector->head_; --i) {
    vector->storage_->ops_->softReset(vector->storage_ + i);
  }
  vector->tail_ = 0;
  return kErrorCode_Ok;
}

s16 VECTOR_reset(Vector* vector) {
  if (NULL == vector) {
    return kErrorCode_Vector_Null;
  }
  if (NULL == vector->storage_) {
    return kErrorCode_Memory;
  }
  for (int i = vector->head_; i < vector->tail_; ++i) {
    
      (vector->storage_ + i)->ops_->reset(vector->storage_ + i);
  
  }
  vector->tail_ = 0;
  return kErrorCode_Ok;
}

s16 VECTOR_resize(Vector* vector, u16 new_size) {

  if (NULL == vector) {
    return kErrorCode_Vector_Null;
  }  
  if (NULL == vector->storage_) {
    return kErrorCode_Data;
  }
  if (0 == new_size) {
    return kErrorCode_ZeroSize;
  }

  if (new_size == vector->capacity_) {

    return kErrorCode_Ok;

  }else if(new_size > vector->capacity_) { //Bigger

    MemoryNode* aux_storage = MM->malloc(sizeof(MemoryNode)*new_size);
    for (int i = 0; i < new_size; ++i) {
      MEMNODE_createLite((aux_storage+i));
    }

    for (int i = 0; i < vector->tail_; ++i) {
      aux_storage->ops_->setData(aux_storage + i, (vector->storage_ + i)->data_,
                                (vector->storage_+i)->size_);
    }
   
  
  
    MM->free(vector->storage_);
    
    vector->storage_ = aux_storage;
    vector->capacity_ = new_size;
  }
  else if(new_size < vector->capacity_){ //Smaller

    MemoryNode* aux_storage = MM->malloc(sizeof(MemoryNode)*new_size);
    if (new_size < vector->tail_) {
      vector->tail_ = new_size;
    }
      
    for (int i = 0; i < new_size; ++i) {
      MEMNODE_createLite((aux_storage + i));
      aux_storage->ops_->setData(aux_storage + i, (vector->storage_ + i)->data_,
                                 vector->storage_->size_);
    }

 
    
    for (int i = vector->capacity_ ; i >= new_size; --i) {
      vector->storage_->ops_->reset(vector->storage_ + i);
    }
    
    
    MM->free(vector->storage_);
    
    vector->storage_ = aux_storage;
    vector->capacity_ = new_size;
  }

   return kErrorCode_Ok;
}

u16 VECTOR_capacity(Vector* vector) {
  if (NULL == vector || 0 == vector->capacity_ ) {
    return 0;
  }
  return vector->capacity_;
}

u16 VECTOR_length(Vector* vector) {
  if (NULL == vector || 0 == vector->capacity_) {
    return 0;
  }
  return (vector->tail_-vector->head_);
}

boolean VECTOR_isEmpty(Vector* vector) {
  if (NULL == vector) {
    return true;
  }

  if (vector->tail_ == vector->head_) {
    return true;
  }
  return false;
}

boolean VECTOR_isFull(Vector* vector) {
  if (NULL == vector) {
    return false;
    
  }
  if (vector->tail_ == vector->capacity_) {
    return true;
  }
  return false;
}

void* VECTOR_first(Vector* vector) {
  
  if (NULL == vector) {
    return NULL;
  }
  if (NULL == vector->storage_) {
    return NULL;
  }

  return vector->storage_->ops_->data(vector->storage_);
}

void* VECTOR_last(Vector* vector) {

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

void* VECTOR_at(Vector* vector, u16 position) {

  if (NULL == vector) {
    return NULL;
  }
  if (NULL == vector->storage_) {
    return NULL;
  }
  if (position >= vector->tail_) {
    return NULL;
  }

  return vector->storage_->ops_->data(vector->storage_ + position);
}

s16 VECTOR_insertFirst(Vector* vector, void* data, u16 bytes) {

  if (NULL == vector) {
    return kErrorCode_Vector_Null;
  }
  if (NULL == vector->storage_) {
    return kErrorCode_Memory;
  }
  if (NULL == data) {
    return kErrorCode_Data;
  }
  if (vector->capacity_ <= vector->tail_) {
    return kErrorCode_VectorFull;
  }
  if (bytes == 0) {
    return kErrorCode_ZeroSize;
  }
  
  for (int i = vector->tail_; i > vector->head_; --i) {
    vector->storage_->ops_->setData(vector->storage_ + i,
                                   (vector->storage_+i-1)->data_,
                                   (vector->storage_+i-1)->size_);
  }
  vector->storage_->ops_->setData(vector->storage_ + vector->head_, data, bytes);
  ++vector->tail_;
  return kErrorCode_Ok;

}

s16 VECTOR_insertLast(Vector* vector, void* data, u16 bytes) {

  if (NULL == vector) {
    return kErrorCode_Vector_Null;
  }
  if (NULL == vector->storage_) {
    return kErrorCode_Memory;
  }
  if (NULL == data) {
    return kErrorCode_Data;
  }
  if (vector->capacity_ <= vector->tail_) {
    return kErrorCode_VectorFull;
  }
  if (bytes == 0) {
    return kErrorCode_ZeroSize;
  }

  vector->storage_->ops_->setData(vector->storage_ + vector->tail_, data, bytes);
  ++vector->tail_;
  return kErrorCode_Ok;
}

s16 VECTOR_insertAt(Vector* vector, void* data, u16 bytes, u16 position) {
  if (NULL == vector) {
    return kErrorCode_Vector_Null;
  }
  if (NULL == vector->storage_) {
    return kErrorCode_Memory;
  }
  if (NULL == data) {
    return kErrorCode_Data;
  }
  if (vector->capacity_ <= vector->tail_) {
    return kErrorCode_VectorFull;
  }
  if (bytes == 0) {
    return kErrorCode_ZeroSize;
  }
  if (position > vector->tail_) {
    position = vector->tail_;
  }
  for (int i = vector->tail_; i > position; --i) {
    vector->storage_->ops_->setData(vector->storage_ + i,
                                   (vector->storage_ + i - 1)->data_,
                                   (vector->storage_ + i - 1)->size_);
  }
  vector->storage_->ops_->setData((vector->storage_ + vector->head_+ position), data, bytes);
  ++vector->tail_;

  return kErrorCode_Ok;

}

void* VECTOR_extractFirst(Vector* vector) {
    
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
  
  for (int i = vector->head_; i < vector->tail_-1; ++i) {
    vector->storage_->ops_->setData(vector->storage_ + i,
                           (vector->storage_ + i + 1)->data_,
                           (vector->storage_ + i + 1)->size_);
  }

  --vector->tail_;
  vector->storage_->ops_->softReset(vector->storage_ + vector->tail_);
  return aux_node;

}

void* VECTOR_extractLast(Vector* vector) {

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
 
  vector->storage_->ops_->softReset(vector->storage_ + vector->tail_);
  --vector->tail_;
  return aux_node;

}

void* VECTOR_extractAt(Vector* vector, u16 position) {

  if (NULL == vector) {
    return NULL;
  }
  if (NULL == vector->storage_) {
    return NULL;
  }
  if (position > vector->tail_) {
    return NULL;
  }
  if (vector->tail_ == 0) {
    return NULL;
  }
  
  void* aux_node = vector->storage_->ops_->data(vector->storage_ + vector->head_ + position);

  for (int i = position; i < vector->tail_; ++i) {
    vector->storage_->ops_->setData(vector->storage_ + i,
                           (vector->storage_ + i + 1)->data_,
                           (vector->storage_ + i + 1)->size_);
  }

  --vector->tail_;
  vector->storage_->ops_->softReset(vector->storage_ + vector->tail_);
  return aux_node;

}

s16 VECTOR_concat(Vector* vector, Vector* vector_src) {

  if (NULL == vector || NULL == vector_src) {
    return kErrorCode_Vector_Null;
  }
  if (NULL == vector->storage_ || NULL == vector_src->storage_) {
    return kErrorCode_Vector_Null;
  }
  vector->capacity_ += vector_src->capacity_;

  
  
  MemoryNode* aux_node = MM->malloc(sizeof(MemoryNode) * vector->capacity_);
  for (int i = 0; i < vector->capacity_; ++i) {
    MEMNODE_createLite((aux_node+i));
  }
  for (int i = vector->head_; i < vector->tail_; ++i) {
   
    aux_node->ops_->memCopy((aux_node + i ), 
                            (vector->storage_ + i)->data_, 
                            (vector->storage_ + i)->size_);
  }
 
  for (int i = vector_src->head_; i < vector_src->tail_; ++i) {

    aux_node->ops_->memCopy((aux_node + i + vector->tail_) ,
                            (vector_src->storage_ + i )->data_, 
                            (vector_src->storage_ + i)->size_);
 
  }
  int aux_tail = vector->tail_;
  vector->ops_->reset(vector);
  vector->tail_ = vector_src->tail_ + aux_tail;
  

  MM->free(vector->storage_);
  vector->storage_ = aux_node;

  return kErrorCode_Ok;

}

s16 VECTOR_traverse(Vector* vector, void(*callback)(MemoryNode *)) {
  if (NULL == vector) {
    return kErrorCode_Vector_Null;
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

s16 VECTOR_print(Vector* vector){

  if (NULL != vector) {
    printf("[Vector Info] Address: %p\n", vector);
    printf("[Vector Info] Head: %d\n", vector->head_);
    printf("[Vector Info] Tail: %d\n", vector->tail_);
    printf("[Vector Info] Length: %d\n", VECTOR_length(vector));
    printf("[Vector Info] Capaciy: %d\n", vector->capacity_);
    printf("[Vector Info] Data Address: %p\n", vector->storage_);

    for (int i = vector->head_; i < vector->tail_; ++i) {
      printf("  ");
      printf("[Vector Info] Storage %d#\n", i);
      
      vector->storage_->ops_->print(vector->storage_ + i);
    }
  }
}

Vector* VECTOR_create(u16 capacity) {
  if (0 == capacity) {
    return NULL;
  }

  Vector* aux_vector = MM->malloc(sizeof(Vector));
  aux_vector->head_ = 0;
  aux_vector->tail_ = 0;

  aux_vector->capacity_ = capacity;
  aux_vector->ops_ = &vector_ops_s;
  aux_vector->storage_ = MM->malloc(sizeof(MemoryNode)*capacity);
 
  for (int i = 0; i < capacity; ++i) {
    
    MEMNODE_createLite((aux_vector->storage_+i));
    
  }
  return aux_vector;
}