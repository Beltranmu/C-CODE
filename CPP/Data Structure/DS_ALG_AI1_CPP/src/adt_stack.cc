#include <stdio.h>
#include <stdlib.h>


#include "common_def.h"

#include "adt_stack.h"
//#include "adt_memory_node.cc"
//#include "adt_vector.cc"


Stack::Stack(){
  storage_ = nullptr;
}

Stack::Stack(Stack & copystack){

  if (nullptr != copystack.storage_) {
    storage_ = new Vector(*copystack.storage_);
  }
}

Stack& Stack::operator=(const Stack &stack) {
  if (nullptr == storage_) {
    storage_ = new Vector();
  }

  if (nullptr != stack.storage_) {
    
    *storage_ = *stack.storage_;
  }
  return *this;
}

Stack::~Stack(){
  if (nullptr != storage_) {
    delete storage_;
    storage_ = nullptr;
  }

}

s16 Stack::init(u16 capacity){
 
  if (nullptr == this) {
    return kErrorCode_Stack_Null;
  }

  if (0 == capacity) {
    return kErrorCode_ZeroSize;
  }

  storage_ = new Vector();
  storage_->init(capacity);
  if (nullptr == storage_) {
    return KErrorCode_No_Memory;
  }
 
  return kErrorCode_Ok;
}


s16 Stack::softReset() {
  if (nullptr == this) {
    return kErrorCode_Stack_Null;
  }
  if (nullptr == storage_) {
    return kErrorCode_Vector_Null;
  }
  
  return storage_->softReset();
}

s16 Stack::reset(){
  if (nullptr == this) {
    return kErrorCode_Stack_Null;
  }
  if (nullptr == storage_) {
    return kErrorCode_Vector_Null;
  }
  
  return storage_->reset();
}

s16 Stack::resize(u16 new_size) {

  if (nullptr == this) {
    return kErrorCode_Stack_Null;
  }  
  if (nullptr == storage_) {
    return kErrorCode_Vector_Null;
  }
 

  return storage_->resize(new_size);
}

u16 Stack::capacity() {
  if (nullptr == this || nullptr == storage_) {
    return 0;
  }
  return storage_->capacity();
}

u16 Stack::length() {
  if (nullptr == this|| nullptr == storage_)  {
    return 0;
  }
  return storage_->length();
}

bool Stack::isEmpty() {
  if (nullptr == this|| nullptr == storage_) {
    return false;
  }
  return storage_->isEmpty();
}

bool Stack::isFull() {
  if (nullptr == this|| nullptr == storage_) {
    return false;
  }
  return storage_->isFull();
}

void* Stack::top() {

  if (nullptr == this) {
    return nullptr;
  }
  if (nullptr == storage_) {
    return nullptr;
  }

  return storage_->last();
  
}

s16 Stack::push(void* data, u16 bytes) {

  if (nullptr == this) {
    return kErrorCode_Stack_Null;
  }
  if (nullptr == storage_) {
    return kErrorCode_Vector_Null;
  }
  if (isFull()) {
    return kErrorCode_Stack_Full;
  }

  
  return storage_->insertLast(data, bytes);
}

void* Stack::pop() {
    
  if (nullptr == this) {
    return nullptr;
  }
  if (nullptr == storage_) {
    return nullptr;
  }
  if (capacity() == 0) {
    return nullptr;
  }

  return storage_->extractLast();
}

s16 Stack::concat(Stack* stack_src) {

  if (nullptr == this|| nullptr == stack_src) {
    return kErrorCode_Stack_Null;
  }
  if (nullptr == storage_ || nullptr == stack_src->storage_) {
    return kErrorCode_Vector_Null;
  }
  
  return storage_->concat(stack_src->storage_);
}

void Stack::print(){

  if (nullptr != this) {
    printf("[Stack Info] Address: %p\n", this);
    if (nullptr != storage_) {
      printf("[Stack Info] Storage Address: %p\n", storage_);
      storage_->print();
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



