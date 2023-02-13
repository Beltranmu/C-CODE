#include <stdio.h>
#include <stdlib.h>


#include "common_def.h"
#include "adt_stack.h"

#include "adt_memory_node.c"
#include "ABGS_MemoryManager/abgs_memory_manager.h"

//Stack Declaratoon
static s16 STACK_destroy(Stack* vector);
static s16 STACK_softReset(Stack* vector);
static s16 STACK_reset(Stack* vector);
static s16 STACK_resize(Stack* vector, u16 new_size);

static u16 STACK_capacity(Stack* vector);
static u16 STACK_length(Stack* vector);
static bool STACK_isEmpty(Stack* vector);
static bool STACK_isFull(Stack* vector);

static void* STACK_top(Stack* vector);
static s16 STACK_push(Stack* vector , void* data, u16 bytes);
static void* STACK_pop(Stack* vector);

static s16 STACK_concat(Stack* vector, Stack* vector_src);
static void STACK_print(Stack* vector);




// Stack's API Definitions
struct stack_ops_s stack_ops_s = { .destroy = STACK_destroy,
                                   .softReset = STACK_softReset,
                                   .reset = STACK_reset,
                                   .resize = STACK_resize,
                                   .capacity = STACK_capacity,
                                   .length = STACK_length,
                                   .isEmpty = STACK_isEmpty,
                                   .isFull = STACK_isFull,
                                   .top = STACK_top,
                                   .push = STACK_push,
                                   .pop = STACK_pop,
                                   .concat = STACK_concat,                       
                                   .print = STACK_print
};

// CreateLite Definition


// Vector Definitions

s16 STACK_destroy(Stack* stack){
  if (NULL == stack) {
    return kErrorCode_Stack_Null;
  }
  if (NULL == stack->storage_) {
    MM->free(stack);
    return kErrorCode_Vector_Null;
    
  }
  
  s16 return_code = stack->storage_->ops_->destroy(stack->storage_);
  
  MM->free(stack);
  return return_code;
}

s16 STACK_softReset(Stack* stack) {
  if (NULL == stack) {
    return kErrorCode_Stack_Null;
  }
  if (NULL == stack->storage_) {
    return kErrorCode_Vector_Null;
  }
  
  return stack->storage_->ops_->softReset(stack->storage_);
}

s16 STACK_reset(Stack* stack) {
  if (NULL == stack) {
    return kErrorCode_Stack_Null;
  }
  if (NULL == stack->storage_) {
    return kErrorCode_Vector_Null;
  }
  
  return stack->storage_->ops_->reset(stack->storage_);
}

s16 STACK_resize(Stack* stack, u16 new_size) {

  if (NULL == stack) {
    return kErrorCode_Stack_Null;
  } 
  if (NULL == stack->storage_) {
    return kErrorCode_Vector_Null;
  }/*
  if (0 == new_size) {
    return kErrorCode_ZeroSize;
  }*/
  
  return stack->storage_->ops_->resize(stack->storage_, new_size);
}

u16 STACK_capacity(Stack* stack) {
  if (NULL == stack || NULL == stack->storage_) {
    return 0;
  }
  return stack->storage_->ops_->capacity(stack->storage_);
}

u16 STACK_length(Stack* stack) {
  if (NULL == stack || NULL == stack->storage_)  {
    return 0;
  }
  return stack->storage_->ops_->length(stack->storage_);
}

bool STACK_isEmpty(Stack* stack) {
  if (NULL == stack || NULL == stack->storage_) {
    return false;
  }
  return stack->storage_->ops_->isEmpty(stack->storage_);
}

bool STACK_isFull(Stack* stack) {
  if (NULL == stack || NULL == stack->storage_) {
    return false;
  }
  return stack->storage_->ops_->isFull(stack->storage_);
}

void* STACK_top(Stack* stack) {

  if (NULL == stack) {
    return NULL;
  }
  if (NULL == stack->storage_) {
    return NULL;
  }

  return stack->storage_->ops_->last(stack->storage_);
  
}

s16 STACK_push(Stack* stack, void* data, u16 bytes) {

  if (NULL == stack) {
    return kErrorCode_Stack_Null;
  }
  if (NULL == stack->storage_) {
    return kErrorCode_Vector_Null;
  }
  /*if (NULL == data) {
    return kErrorCode_Data_Null;
  }*/
  if (stack->ops_->isFull(stack)) {
    return kErrorCode_Stack_Full;
  }
 /* if (bytes == 0) {
    return kErrorCode_ZeroSize;
  }*/
  
  return stack->storage_->ops_->insertLast(stack->storage_, data, bytes);
}

void* STACK_pop(Stack* stack) {
    
  if (NULL == stack) {
    return NULL;
  }
  if (NULL == stack->storage_) {
    return NULL;
  }
  if (stack->ops_->capacity(stack) == 0) {
    return NULL;
  }

  return stack->storage_->ops_->extractLast(stack->storage_);
}

s16 STACK_concat(Stack* stack, Stack* stack_src) {

  if (NULL == stack || NULL == stack_src) {
    return kErrorCode_Stack_Null;
  }
  if (NULL == stack->storage_ || NULL == stack_src->storage_) {
    return kErrorCode_Vector_Null;
  }
  
  return stack->storage_->ops_->concat(stack->storage_, stack_src->storage_);
}

void STACK_print(Stack* stack){

  if (NULL != stack) {
    printf("[Stack Info] Address: %p\n", stack);
    if (NULL != stack->storage_) {
      printf("[Stack Info] Storage Address: %p\n", stack->storage_);
      stack->storage_->ops_->print(stack->storage_);
    }
    else {
      printf("[Stack Info] Storage Address: NULL\n");
    }
  }
  else{
    printf("[Stack Info] Address: NULL\n");
    printf("[Stack Info] Storage Address: NULL\n");
  }
}

Stack* STACK_create(u16 capacity) {
  if (0 == capacity) {
    return NULL;
  }

  Stack* new_stack = MM->malloc(sizeof(Stack));
  if (new_stack == NULL) {
    return NULL;
  }
  new_stack->storage_ = VECTOR_create(capacity);
  if (NULL == new_stack->storage_) {
    return NULL;
  }
  new_stack->ops_ = &stack_ops_s;

  return new_stack;
}