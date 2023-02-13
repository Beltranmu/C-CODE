#include <stdio.h>
#include <stdlib.h>


#include "common_def.h"
#include "adt_queue.h"

#include "adt_memory_node.c"
#include "ABGS_MemoryManager/abgs_memory_manager.h"

//Vector Declaratoon
static s16 QUEUE_destroy(Queue* queue);
static s16 QUEUE_softReset(Queue* queue);
static s16 QUEUE_reset(Queue* queue);
static s16 QUEUE_resize(Queue* queue, u16 new_size);

static u16 QUEUE_capacity(Queue* queue);
static u16 QUEUE_length(Queue* queue);
static bool QUEUE_isEmpty(Queue* queue);
static bool QUEUE_isFull(Queue* queue);

static void* QUEUE_front(Queue* queue);
static void* QUEUE_back(Queue* queue);

static s16 QUEUE_enqueue(Queue* queue, void* data, u16 bytes);
static void* QUEUE_dequeue(Queue* queue);


static s16 QUEUE_concat(Queue* queue, Queue*  queue_src);
static void QUEUE_print(Queue* queue);



// Vector's API Definitions
struct queue_ops_s queue_ops_s = {.destroy = QUEUE_destroy,
                                  .softReset = QUEUE_softReset,
                                  .reset = QUEUE_reset,
                                  .resize = QUEUE_resize,
                                  .capacity = QUEUE_capacity,
                                  .length = QUEUE_length,
                                  .isEmpty = QUEUE_isEmpty,
                                  .isFull = QUEUE_isFull,
                                  .front = QUEUE_front,
                                  .back = QUEUE_back,
                                  .enqueue = QUEUE_enqueue,
                                  .dequeue = QUEUE_dequeue,                          
                                  .concat = QUEUE_concat,
                                  .print = QUEUE_print
};

// CreateLite Definition


// Vector Definitions

s16 QUEUE_destroy(Queue* queue){
  if (NULL == queue) {
    return kErrorCode_Queue_Null;
  }
  if (NULL == queue->storage_) {
    MM->free(queue);
    return kErrorCode_List_Null;
  }

  s16 return_code = queue->storage_->ops_->destroy(queue->storage_);

  MM->free(queue);
  return return_code;

}

s16 QUEUE_softReset(Queue* queue) {

  if (queue == NULL) {
    return kErrorCode_Queue_Null;
  }
  if (NULL == queue->storage_) {
    return kErrorCode_List_Null;
  }
 
  return queue->storage_->ops_->softReset(queue->storage_);
}

s16 QUEUE_reset(Queue* queue) {
  if (queue == NULL) {
    return kErrorCode_Queue_Null;
  }
  if (NULL == queue->storage_) {
    return kErrorCode_List_Null;
  }
  /*if (queue->storage_->ops_->isEmpty(queue)) {
    return kErrorCode_Queue_Empty;
  }*/

  return queue->storage_->ops_->reset(queue->storage_);
}

s16 QUEUE_resize(Queue* queue, u16 new_size) {

  if (NULL == queue) {
    return kErrorCode_Queue_Null;
  }
  if (NULL == queue->storage_) {
    return kErrorCode_List_Null;
  }/*
  if (new_size <= 0) {
    return kErrorCode_ZeroSize;
  }*/

  return queue->storage_->ops_->resize(queue->storage_, new_size);;
}

u16 QUEUE_capacity(Queue* queue) {
  if (NULL == queue) {
    return 0;
  }
  if (NULL == queue->storage_) {
    return 0;
  }
  return queue->storage_->ops_->capacity(queue->storage_);
}

u16 QUEUE_length(Queue* queue) {
  if (NULL == queue) {
    return 0;
  }
  if (NULL == queue->storage_) {
    return 0;
  }
  return queue->storage_->ops_->length(queue->storage_);
}

bool QUEUE_isEmpty(Queue* queue) {
  if (NULL == queue) {
    return false;
  }
  if (NULL == queue->storage_) {
    return false;;
  }
  return queue->storage_->ops_->isEmpty(queue->storage_);
}

bool QUEUE_isFull(Queue* queue) {
  if (NULL == queue) {
    return false;
  }
  if (NULL == queue->storage_) {
    return false;
  }
  return queue->storage_->ops_->isFull(queue->storage_);
}

//First
void* QUEUE_front(Queue* queue) {
  if (NULL == queue) {
    return NULL;
  }
  if (queue->ops_->isEmpty(queue)) {
    return NULL;
  }
  if (NULL == queue->storage_) {
    return NULL;
  }
  return queue->storage_->ops_->first(queue->storage_);
}

//Last
void* QUEUE_back(Queue* queue) {
  if (NULL == queue) {
    return NULL;
  }
  if (queue->ops_->isEmpty(queue)) {
    return NULL;
  }
  if (NULL == queue->storage_) {
    return NULL;
  }
  return queue->storage_->ops_->last(queue->storage_);
}


// Insert Last
s16 QUEUE_enqueue(Queue* queue, void* data, u16 bytes) {

  if (NULL == queue) {
    return kErrorCode_Queue_Null;
  }
  /*if (NULL == data) {
    return kErrorCode_Data_Null;
  }*/
  if (NULL == queue->storage_) {
    return kErrorCode_List_Null;
  }/*
  if (bytes <= 0) {
    return kErrorCode_ZeroSize;
  }*/
  if (queue->ops_->isFull(queue)) {
    return kErrorCode_Queue_Full;
  }

  return queue->storage_->ops_->insertLast(queue->storage_, data, bytes);;
}

// Extract First
void* QUEUE_dequeue(Queue* queue) {

  if (NULL == queue) {
    return NULL;
  }
  if (NULL == queue->storage_) {
    return NULL;
  }

  return queue->storage_->ops_->extractFirst(queue->storage_);
}

s16 QUEUE_concat(Queue* queue, Queue* queue_src) {

  if (NULL == queue || NULL == queue_src) {
    return kErrorCode_Queue_Null;
  }
  if (NULL == queue->storage_) {
    return kErrorCode_List_Null;
  }
  
  return queue->storage_->ops_->concat(queue->storage_, queue_src->storage_);
}


void QUEUE_print(Queue* queue) {

  if (NULL != queue) {
    printf("[Queue Info] Address: %p\n", queue);
    if (NULL != queue->storage_) {
      printf("[Queue Info] Storage Address: %p\n", queue->storage_);
      queue->storage_->ops_->print(queue->storage_);
    }
    else {
      printf("[Queue Info] Storage Address: NULL\n");
    }
  }
  else {
    printf("[Queue Info] Address: NULL\n");
    printf("[Queue Info] Storage Address: NULL\n");
  }
}

Queue* QUEUE_create(u16 capacity) {
  
  if (capacity == 0) {
    return NULL;
  }
  Queue *new_queue = MM->malloc(sizeof(Queue));
  if (NULL == new_queue) {
    return NULL;
  }
  new_queue->storage_ = LIST_create(capacity);
  if (NULL == new_queue->storage_) {
    return NULL;
  }
  new_queue->ops_ = &queue_ops_s;
  return new_queue;
}