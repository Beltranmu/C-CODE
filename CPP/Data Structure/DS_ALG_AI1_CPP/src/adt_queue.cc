#include <stdio.h>
#include <stdlib.h>


#include "common_def.h"
#include "adt_queue.h"

//#include "adt_list.cc"



Queue::Queue(){
  storage_ = nullptr;
}

Queue::Queue(Queue & copyQueue){
  if (nullptr != copyQueue.storage_) {
    storage_ = new List(*copyQueue.storage_);
  }
}

Queue& Queue::operator=(const Queue& queue){
  if (nullptr == storage_) {
    storage_ = new List();
  }

  if (nullptr != queue.storage_) {
    *storage_ = *queue.storage_;
  }
  return *this;
}

Queue::~Queue(){
  if (nullptr != storage_) {
    delete storage_;
    storage_ = nullptr;
  }
}


s16 Queue::init(u16 capacity) {

  if (nullptr == this) {
    return kErrorCode_Queue_Null;
  }

  if (0 == capacity) {
    return kErrorCode_ZeroSize;
  }

  storage_ = new List();
  storage_->init(capacity);
  if (nullptr == storage_) {
    return KErrorCode_No_Memory;
  }

  return kErrorCode_Ok;

}


s16 Queue::softReset() {

  if (nullptr == this) {
    return kErrorCode_Queue_Null;
  }
  if (nullptr == storage_) {
    return kErrorCode_List_Null;
  }
 
  return storage_->softReset();
}

s16 Queue::reset() {
  if (nullptr == this) {
    return kErrorCode_Queue_Null;
  }
  if (nullptr == storage_) {
    return kErrorCode_List_Null;
  }
  /*if (queue->storage_->ops_->isEmpty(queue)) {
    return kErrorCode_Queue_Empty;
  }*/

  return storage_->reset();
}

s16 Queue::resize(u16 new_size) {

  if (nullptr == this) {
    return kErrorCode_Queue_Null;
  }
  if (nullptr == storage_) {
    return kErrorCode_List_Null;
  }
  /*if (new_size <= 0) {
    return kErrorCode_ZeroSize;
  }*/

  return storage_->resize(new_size);;
}

u16 Queue::capacity() {
  if (nullptr == this) {
    return 0;
  }
  if (nullptr == storage_) {
    return 0;
  }
  return storage_->capacity();
}

u16 Queue::length() {
  if (nullptr == this) {
    return 0;
  }
  if (nullptr == storage_) {
    return 0;
  }
  return storage_->length();
}

bool Queue::isEmpty() {
  if (nullptr == this) {
    return false;
  }
  if (nullptr == storage_) {
    return false;;
  }
  return storage_->isEmpty();
}

bool Queue::isFull() {
  if (nullptr == this) {
    return false;
  }
  if (nullptr == storage_) {
    return false;
  }
  return storage_->isFull();
}

//First
void* Queue::front() {
  if (nullptr == this) {
    return nullptr;
  }
  if (isEmpty()) {
    return nullptr;
  }
  if (nullptr == storage_) {
    return nullptr;
  }
  return storage_->first();
}

//Last
void* Queue::back() {
  if (nullptr == this) {
    return nullptr;
  }
  if (isEmpty()) {
    return nullptr;
  }
  if (nullptr == storage_) {
    return nullptr;
  }
  return storage_->last();
}


// Insert Last
s16 Queue::enqueue(void* data, u16 bytes) {

  if (nullptr == this) {
    return kErrorCode_Queue_Null;
  }
 /* if (nullptr == data) {
    return kErrorCode_Data_Null;
  }*/
  if (nullptr == storage_) {
    return kErrorCode_List_Null;
  }
  /*if (bytes <= 0) {
    return kErrorCode_ZeroSize;
  }*/
  if (isFull()) {
    return kErrorCode_Queue_Full;
  }

  return storage_->insertLast(data, bytes);;
}

// Extract First
void* Queue::dequeue() {

  if (nullptr == this) {
    return nullptr;
  }
  if (nullptr == storage_) {
    return nullptr;
  }

  return storage_->extractFirst();
}

s16 Queue::concat(Queue* queue_src) {

  if (nullptr == this || nullptr == queue_src) {
    return kErrorCode_Queue_Null;
  }
  if (nullptr == storage_) {
    return kErrorCode_List_Null;
  }
  
  return storage_->concat(queue_src->storage_);
}



void Queue::print() {

  if (nullptr != this) {
    printf("[Queue Info] Address: %p\n", this);
    if (nullptr != storage_) {
      printf("[Queue Info] Storage Address: %p\n", storage_);
      storage_->print();
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
