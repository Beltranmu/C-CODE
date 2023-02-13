#include "adt_vector.h"
#include "common_def.h"
#include <stdio.h>

Vector::Vector(){

  head_ = 0;
  tail_ = 0;
  capacity_ = 0;
  storage_ = nullptr;

}

Vector::Vector(Vector &copyvector){

  head_ = copyvector.head_;
  tail_ = copyvector.tail_;
  capacity_ = copyvector.capacity_;

  storage_ = new MemoryNode[capacity_];
  for (int i = 0; i < capacity_; ++i) {
    storage_[i].memCopy(copyvector.storage_[i].data(),
      copyvector.storage_[i].size());
  }

}

Vector& Vector::operator=(const Vector & other){

  head_ = other.head_;
  tail_ = other.tail_;
  capacity_ = other.capacity_;


  storage_ = new MemoryNode[capacity_];
  for (int i = 0; i < capacity_; ++i) {
    storage_[i].memCopy(other.storage_[i].data(),
      other.storage_[i].size());
  }

  return *this;

}

Vector::~Vector(){
  if (!isEmpty()) {
    reset();
    if (nullptr != storage_) {
      delete[] storage_;
    }
    storage_ = nullptr;
    head_ = 0;
    tail_ = 0;
    capacity_ = 0;
  }

}

s16 Vector::init(u16 capacity){
  if (0 == capacity) {
    return kErrorCode_Vector_Null;
  }
  
  if (nullptr == this) {
    return kErrorCode_Vector_Null;
  }
  
  capacity_ = capacity;
  storage_ = new MemoryNode[capacity];

  if (nullptr == storage_) {
    return KErrorCode_No_Memory;
  }


  return kErrorCode_Ok;
}

s16 Vector::softReset()
{
  if (nullptr == this) {
    return kErrorCode_Vector_Null;
  }

  if (nullptr == storage_) {
    return kErrorCode_MemoryNode_Null;
  }

  for (int i = head_; i < tail_; ++i) {
    storage_[i].softReset();
  }

  tail_ = 0;
  return kErrorCode_Ok;
}

s16 Vector::reset()
{
  if (nullptr == this) {
    return kErrorCode_Vector_Null;
  }
  if (nullptr == storage_) {
    return kErrorCode_MemoryNode_Null;
  }
  for (int i = head_; i < tail_; ++i){
    storage_[i].reset();
  }
  tail_ = 0;
  return kErrorCode_Ok;
}

s16 Vector::resize(u16 new_size){

  if (nullptr == this) {
    return kErrorCode_Vector_Null;
  }
  if (nullptr == storage_) {
    return kErrorCode_MemoryNode_Null;
  }
  if (0 == new_size) {
    return kErrorCode_ZeroSize;
  }
  s16 return_code = kErrorCode_Ok;
  if (new_size == capacity_) {

    return kErrorCode_Ok;

  }
  else if (new_size > capacity_) { //Bigger

    //MemoryNode* aux_storage = MM->malloc(sizeof(MemoryNode)*new_size);
    MemoryNode* aux_storage = new MemoryNode[new_size];

    if (NULL == aux_storage) {
      return KErrorCode_No_Memory;
    }

    /*for (int i = 0; i < new_size; ++i) {
      return_code = MEMNODE_createLite((aux_storage + i));
    }*/

    for (int i = 0; i < tail_; ++i) {
      return_code = aux_storage[i].setData(storage_[i].data(),
        storage_[i].size());
      storage_[i].softReset();
    }

   // MM->free(vector->storage_);

    delete[] storage_;
    storage_ = aux_storage;
    capacity_ = new_size;
    return return_code;
  }
  else if (new_size < capacity_) { //Smaller

   // MemoryNode* aux_storage = MM->malloc(sizeof(MemoryNode)*new_size);
    MemoryNode* aux_storage = new MemoryNode[new_size];
    if (nullptr == aux_storage) {
      return KErrorCode_No_Memory;
    }
    if (new_size < tail_) {
      tail_ = new_size;
    }

    for (int i = 0; i < new_size; ++i) {
      //return_code = MEMNODE_createLite((aux_storage + i));
      //if (return_code == kErrorCode_Ok) {
        return_code = aux_storage[i].setData(
          storage_[i].data(),
          storage_[i].size());
        storage_[i].softReset();
      //}
    }

    for (int i = capacity_ - 1; i >= new_size; --i) {
      return_code = storage_[i].reset();
    }


    //MM->free(vector->storage_);
    //delete storage_;
    storage_ = aux_storage;
    capacity_ = new_size;
    return return_code;
  }

}

u16 Vector::capacity()
{
  if (nullptr == this || 0 == capacity_) {
    return 0;
  }
  return capacity_;
}

u16 Vector::length()
{
  if (nullptr == this || 0 == capacity_) {
    return 0;
  }
  return (tail_ - head_);
}

bool Vector::isEmpty()
{
  if (nullptr == this) {
    return true;
  }
  return tail_ == head_;
  
}

bool Vector::isFull()
{
  if (nullptr == this) {
    return false;
  }
  return tail_ == capacity_;
 
}

void* Vector::first()
{
  if (nullptr == this) {
    return nullptr;
  }
  if (nullptr == storage_) {
    return nullptr;
  }

  return storage_[head_].data();
}

void* Vector::last()
{
  if (nullptr == this) {
    return nullptr;
  }
  if (tail_ == 0) {
    return nullptr;
  }
  if (nullptr == storage_) {
    return nullptr;
  }

  return storage_[tail_].data();
}

void* Vector::at(u16 position)
{
  if (nullptr == this) {
    return nullptr;
  }
  if (nullptr == storage_) {
    return nullptr;
  }
  if (position >= tail_) {
    return nullptr;
  }

  return storage_[position].data();
}


s16 Vector::insertFirst(void* data, u16 bytes) {

  if (nullptr == this) {
    return kErrorCode_Vector_Null;
  }
  if (nullptr == storage_) {
    return kErrorCode_MemoryNode_Null;
  }
  if (nullptr == data) {
    return kErrorCode_Data_Null;
  }
  if (capacity_ <= tail_) {
    return kErrorCode_Vector_Full;
  }
  if (bytes == 0) {
    return kErrorCode_ZeroSize;
  }

  for (int i = tail_; i > head_; --i) {
    storage_[i].setData(storage_[i-1].data(),
                        storage_[i-1].size());
  }
  storage_[head_].setData(data, bytes);
  ++tail_;
  return kErrorCode_Ok;

}

s16 Vector::insertLast(void* data, u16 bytes) {

  if (nullptr == this) {
    return kErrorCode_Vector_Null;
  }
  if (nullptr == storage_) {
    return kErrorCode_MemoryNode_Null;
  }
  if (nullptr == data) {
    return kErrorCode_Data_Null;
  }
  if (capacity_ <= tail_) {
    return kErrorCode_Vector_Full;
  }
  if (bytes == 0) {
    return kErrorCode_ZeroSize;
  }

  storage_[tail_].setData(data, bytes);
  ++tail_;
  return kErrorCode_Ok;
}

s16 Vector::insertAt(void* data, u16 bytes, u16 position) {
  if (nullptr == this) {
    return kErrorCode_Vector_Null;
  }
  if (nullptr == storage_) {
    return kErrorCode_MemoryNode_Null;
  }
  if (nullptr == data) {
    return kErrorCode_Data_Null;
  }
  if (capacity_ <= tail_) {
    return kErrorCode_Vector_Full;
  }
  if (bytes == 0) {
    return kErrorCode_ZeroSize;
  }
  if (position > tail_) {
    position = tail_;
  }
  for (int i = tail_; i > position; --i) {
    storage_[i].setData(
      storage_[i-1].data(),
      storage_[i-1].size());
  }
  storage_[head_+position].setData(data, bytes);
  ++tail_;

  return kErrorCode_Ok;

}

void* Vector::extractFirst() {

  if (nullptr == this) {
    return nullptr;
  }
  if (nullptr == storage_) {
    return nullptr;
  }
  if (tail_ == 0) {
    return nullptr;
  }

  void* aux_node = storage_[head_].data();

  for (int i = head_; i < tail_ - 1; ++i) {
    storage_[i].setData(
      storage_[i+1].data(),
      storage_[i+1].size());
  }

  --tail_;
  storage_[tail_].softReset();
  return aux_node;

}

void* Vector::extractLast() {

  if (nullptr == this) {
    return nullptr;
  }
  if (NULL == storage_) {
    return nullptr;
  }
  if (tail_ == 0) {
    return NULL;
  }

  void* aux_node = storage_[tail_-1].data();

  storage_[tail_].softReset();
  --tail_;
  return aux_node;

}

void* Vector::extractAt(u16 position) {

  if (nullptr == this) {
    return nullptr;
  }
  if (nullptr == storage_) {
    return nullptr;
  }
  if (position > tail_) {
    return nullptr;
  }
  if (tail_ == 0) {
    return nullptr;
  }

  void* aux_node = storage_[head_ + position].data();

  for (int i = position; i < tail_; ++i) {
    storage_[i].setData(
      storage_[i+1].data(),
      storage_[i+1].size());
  }

  storage_[tail_].softReset();
  --tail_;
  return aux_node;

}

s16 Vector::concat(Vector* vector_src) {

  if (nullptr == this|| nullptr == vector_src) {
    return kErrorCode_Vector_Null;
  }
  if (nullptr == storage_ || nullptr == vector_src->storage_) {
    return kErrorCode_MemoryNode_Null;
  }
  capacity_ += vector_src->capacity_;

  //MemoryNode* aux_node = MM->malloc(sizeof(MemoryNode) * vector->capacity_);
  MemoryNode* aux_node = new MemoryNode[capacity_];
  if (nullptr == aux_node) {
    return KErrorCode_No_Memory;
  }

  for (int i = head_; i < tail_; ++i) {
    aux_node[i].memCopy(storage_[i].data(),
                        storage_[i].size());
  }

  for (int i = vector_src->head_; i < vector_src->tail_; ++i) {
    aux_node[i+tail_].memCopy(vector_src->storage_[i].data(),
                              vector_src->storage_[i].size());
  }
  int aux_tail = tail_;
  reset();
  tail_ = vector_src->tail_ + aux_tail;

  //MM->free(vector->storage_);
  delete[] storage_;
  storage_ = aux_node;

  return kErrorCode_Ok;

}


s16 Vector::traverse(void(*callback)(MemoryNode *)){
  if (nullptr == this) {
    return kErrorCode_Vector_Null;
  }
  if (nullptr == callback) {
    return kErrorCode_NonFuction;
  }

  //Go to all storage of the vector not null
  for (int i = head_; i < tail_; ++i) {

  }
  return kErrorCode_Ok;
}

void Vector::print()
{
  if (nullptr != this) {
    printf("[Vector Info] Address: %p\n", this);
    printf("[Vector Info] Head: %d\n", head_);
    printf("[Vector Info] Tail: %d\n", tail_);
    printf("[Vector Info] Length: %d\n", length());
    printf("[Vector Info] Capaciy: %d\n", capacity_);
    printf("[Vector Info] Data Address: %p\n", storage_);

    for (int i = head_; i < tail_; ++i) {
      printf("  ");
      printf("[Vector Info] Storage %d#\n", i);

      storage_[i].print();
    }
  }
  else {
    printf("[Vector Info] Address: NULL");
  }
}
