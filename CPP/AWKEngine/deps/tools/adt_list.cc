#include <stdio.h>
#include <stdlib.h>


#include "common_def.h"
#include "adt_list.h"
#include "adt_memory_node.h"


List::List(){
  head_ = nullptr;
  tail_ = nullptr;
  capacity_ = 0;
  length_ = 0;
}

List::List(List & copylist){

  head_ = nullptr;
  tail_ = nullptr;
  capacity_ = copylist.capacity_;
  length_ = 0;

  if (copylist.head_ != nullptr) {

    MemoryNode* iterator_node;

    for (iterator_node = copylist.head_; iterator_node != nullptr; iterator_node = iterator_node->next()) {
      //insertLast(iterator_node->data(), iterator_node->size());
      MemoryNode *new_element = new MemoryNode(*iterator_node);
      //new_element->memCopy(i_list->data(), i_list->size());
      //new_element->setNext(nullptr);

      if (nullptr != tail_) {
        tail_->setNext(new_element);
      }

      tail_ = new_element;
      if (length() == 0) {
        head_ = new_element;
      }
      ++length_;
    }
  }

}

List& List::operator=(const List &list){

  head_ = nullptr;
  tail_ = nullptr;
  capacity_ = list.capacity_;
  length_ = 0;

  if (list.head_ != nullptr) {

    MemoryNode* iterator_node;

    for (iterator_node = list.head_; iterator_node != nullptr; iterator_node = iterator_node->next()) {
      MemoryNode *new_element = new MemoryNode(*iterator_node);
      //new_element->memCopy(i_list->data(), i_list->size());
      //new_element->setNext(nullptr);

      if (nullptr != tail_) {
        tail_->setNext(new_element);
      }

      tail_ = new_element;
      if (length() == 0) {
        head_ = new_element;
      }
      ++length_;
    }
  }
  return *this;
}

List::~List(){
  if (!isEmpty()) {
    MemoryNode* deleted_node = head_;

    MemoryNode* next_deleted_node = nullptr;

    while (deleted_node != nullptr) {
      next_deleted_node = deleted_node->next();
      deleted_node->free();
      deleted_node = next_deleted_node;
    }
  }
}

s16 List::init(u16 capacity){
  if (nullptr == this) {
    return kErrorCode_List_Null;
  }
  if (capacity == 0) {
    return kErrorCode_ZeroSize;
  }
  MemoryNode* test_node = new MemoryNode[capacity];
  if (nullptr == test_node) {
    return KErrorCode_No_Memory;
  }
  delete[]test_node;
  head_ = nullptr;
  tail_ = nullptr;
  length_ = 0;
  capacity_ = capacity;
  

}

s16 List::softReset() {

  if (nullptr == this) {
    return kErrorCode_List_Null;
  }
  if (nullptr == head_) {
    return kErrorCode_MemoryNode_Null;
  }
  if (true == isEmpty()) {
    return kErrorCode_List_Empty;
  }
  MemoryNode* i_list = head_;
  for (i_list; i_list != nullptr; i_list = i_list->next()) {
    i_list->softFree();
  }
  length_ = 0;
  head_ = nullptr;
  tail_ = nullptr;

  return kErrorCode_Ok;
}

s16 List::reset() {
  if (this == nullptr) {
    return kErrorCode_List_Null;
  }
  if (nullptr == head_) {
    return kErrorCode_MemoryNode_Null;
  }
  if (true == isEmpty()) {
    return kErrorCode_List_Empty;
  }
  //Recorrer la lista

  MemoryNode* deleted_node = head_;

  MemoryNode* next_deleted_node = nullptr;

  while (deleted_node != nullptr) {
    next_deleted_node = deleted_node->next();
    deleted_node->free();
    deleted_node = next_deleted_node;
  }

  length_ = 0;
  head_ = nullptr;
  tail_ = nullptr;
  return kErrorCode_Ok;
}

s16 List::resize(u16 new_size) {

  if (nullptr == this) {
    return kErrorCode_List_Null;
  }
  if (new_size <= 0) {
    return kErrorCode_ZeroSize;
  }

  if (capacity_ > new_size) {

    MemoryNode* i_list = head_;
    MemoryNode* i_list_next = nullptr;
    int i = 0;
    for (i_list; i_list != nullptr; i_list = i_list_next) {
      i_list_next = i_list->next();
      //New Tail
      if (i == new_size - 1) {
        tail_ = i_list;
        tail_->setNext(nullptr);
      }

      if (i >= new_size) {
        i_list->free();
      }
      i_list = i_list_next;
      ++i;
    }
    length_ = new_size;
  }


  capacity_ = new_size;

   return kErrorCode_Ok;
}

u16 List::capacity() {
  if (nullptr == this) {
    return 0;
  }
  return capacity_;
}

u16 List::length() {
  if (nullptr == this) {
    return 0;
  }
  return length_;
}

bool List::isEmpty() {
  if (nullptr == this) {
    return false;
  }
  return length() == 0;
}

bool List::isFull() {
  if (nullptr == this) {
    return false;
  }
  return length_ == capacity_;
}

void* List::first() {
  if (nullptr == this) {
    return nullptr;
  }
  if (isEmpty()) {
    return nullptr;
  }
  if (nullptr == head_) {
    return nullptr;
  }
  
  return head_->data();
}

void* List::last() {
  if (nullptr == this) {
    return nullptr;
  }
  if (isEmpty()) {
    return nullptr;
  }
  if (nullptr == tail_) {
    return nullptr;
  }

  return tail_->data();
}

void* List::at(u16 position) {

  if (nullptr == this) {
    return nullptr;
  }
  if (nullptr == head_) {
    return nullptr;
  }
  if (position > length_) {
    return nullptr;
  }
  if (isEmpty()) {
    return nullptr;
  }
  if (position == length_) {
    return last();
  }
  MemoryNode* i_list = head_;
  //Puede un bug, creoq ue esta solucionado
  for (int i = 0; i < position; ++i) {
    i_list = i_list->next();
  }
  return i_list->data();
}

s16 List::insertFirst(void* data, u16 bytes) {
  
  if (nullptr== this) {
    return kErrorCode_List_Null;
  }
  if (nullptr == data) {
    return kErrorCode_Data_Null;
  }
  if (bytes <= 0) {
    return kErrorCode_ZeroSize;
  }
  if (isFull()) {
    return kErrorCode_List_Full;
  }

  MemoryNode *new_element = new MemoryNode();
  if (nullptr == new_element) {
    return KErrorCode_No_Memory;
  }
  
  new_element->setData(data, bytes);
  new_element->setNext(head_);
  head_ = new_element;

  if (length() == 0) {
    tail_ = new_element;
  }
  ++length_;

  return kErrorCode_Ok;
}

s16 List::insertLast(void* data, u16 bytes) {

  if (nullptr == this) {
    return kErrorCode_List_Null;
  }
  if (nullptr == data) {
    return kErrorCode_Data_Null;
  }
  if (bytes <= 0) {
    return kErrorCode_ZeroSize;
  }
  if (isFull()) {
    return kErrorCode_List_Full;
  }

  MemoryNode *new_element = new MemoryNode();
  if (nullptr == new_element) {
    return KErrorCode_No_Memory;
  }

  new_element->setData(data, bytes);
  new_element->setNext(nullptr);

  if (nullptr != tail_) {
    tail_->setNext(new_element);
  }

  tail_ = new_element;
  if (length() == 0) {
    head_ = new_element;
  }
  ++length_;
  return kErrorCode_Ok;
}

s16 List::insertAt(void* data, u16 bytes, u16 position) {
  
  if (nullptr == this) {
    return kErrorCode_List_Full;
  }
  if (nullptr == data) {
    return kErrorCode_Data_Null;
  }
  if (bytes <= 0) {
    return kErrorCode_ZeroSize;
  }
  if (isFull()) {
    return kErrorCode_List_Full;
  }
  if (position >= length()) {
    insertLast(data, bytes);
  }
  else if (position <= 0) {
    insertFirst(data, bytes);
  }
  else {
    MemoryNode* i_list = head_;

    for (int i = 0; i < (position - 1); ++i) {
      i_list = i_list->next();
    }

    MemoryNode *new_element = new MemoryNode();
    if (nullptr == new_element) {
      return KErrorCode_No_Memory;
    }
    
    new_element->setData (data, bytes);
    new_element->setNext(i_list->next());
    i_list->setNext(new_element);

    if (length() == 0) {
      tail_ = new_element;
    }
    ++length_;
  }
  return kErrorCode_Ok;

  
}

void* List::extractFirst() {  

  if (nullptr == this) {
    return nullptr;
  }
  if (isEmpty()) {
    return nullptr;
  }

  if (nullptr == head_) {
    return nullptr;
  }
  void *aux_node = head_->data();

 
  MemoryNode* tmp = head_;
  head_ = head_->next();
  tmp->softFree();
  --length_;
  if (length() == 1) {
    tail_ = head_;
  }

  if (isEmpty()) {
    head_ = nullptr;
    tail_ = nullptr;
  }
  return aux_node;
}

void* List::extractLast() {

  if (nullptr == this) {
    return nullptr;
  }
  if (isEmpty()) {
    return nullptr;
  }
  if (nullptr == tail_ || nullptr == head_) {
    return nullptr;
  }
  void *aux_node = tail_->data();

  MemoryNode* i_list = head_;

  //tail_->softFree();
  //MemoryNode *tmp = i_list->next();
  //i_list->setNext(i_list->next()->next());
  //tmp->softFree();


  for (int i = 0; i < length() - 2; ++i) {
    i_list = i_list->next();

  }
  i_list->setNext(nullptr);
  tail_->softFree();

  tail_ = i_list;
  --length_;
  if (length() == 1) {
    tail_ = head_;
  }
  if (isEmpty()) {
    head_ = nullptr;
    tail_ = nullptr;
  }
  return aux_node;
  
}

void* List::extractAt(u16 position) {

  if (nullptr == this) {
    return nullptr;
  }
  if (isEmpty()) {
    return nullptr;
  }
  if (position > length()) {
    return nullptr;
  }
  else if (position == length()) {
    return extractLast();
  }
  else if (position == 0) {
    return extractFirst();
  }
  else {
    if (nullptr == head_) {
      return nullptr;
    }
    MemoryNode *i_list = head_;
    for (int i = 0; i < position - 1; ++i) {
      i_list = i_list->next();
    }

    void *aux_node = i_list->next()->data();

    MemoryNode *tmp = i_list->next();
    i_list->setNext(i_list->next()->next());
    tmp->softFree();

    --length_;

    if (length() == 1) {
      tail_ = head_;
    }

    if (isEmpty()) {
      head_ = nullptr;
      tail_ = nullptr;
    }
    return aux_node;
  }
  
}

s16 List::concat(List* list_src) {

  if (nullptr == this || nullptr == list_src) {
    return kErrorCode_List_Null;
  }
  if (nullptr == head_) {
    return kErrorCode_MemoryNode_Null;
  }
  capacity_ += list_src->capacity_;
  MemoryNode *i_list = list_src->head_;

  for (i_list; i_list != nullptr; i_list = i_list->next()) {

    MemoryNode *new_element = new MemoryNode();
    if (nullptr == new_element) {
      return KErrorCode_No_Memory;
    }
    new_element->memCopy(i_list->data(), i_list->size());
    new_element->setNext(nullptr);

    if (nullptr != tail_) {
      tail_->setNext(new_element);
    }

    tail_ = new_element;
    if (length() == 0) {
      head_ = new_element;
    }
    ++length_;

  }
  return kErrorCode_Ok;

}
 

s16 List::traverse(void(*callback)(MemoryNode *)) {
 
  if (nullptr == this) {
    return kErrorCode_List_Null;
  }
  if (nullptr == head_) {
    return kErrorCode_MemoryNode_Null;
  }
  if (nullptr == callback) {
    return kErrorCode_NonFuction;
  }

  MemoryNode *tmp_node = head_;
  for (tmp_node; tmp_node != nullptr; tmp_node = tmp_node->next()) {
    callback(tmp_node);
  }
  return kErrorCode_Ok;
}

void List::print(){

  if (nullptr != this) {

    printf("[List Info] Address: %p\n", this);
    printf("[List Info] Length: %d\n", length_);
    printf("[List Info] Capacity: %d\n", capacity_);
    if (nullptr == head_) {
      printf("[List Info] First address: NULL\n");
    }
    else {
      printf("[List Info] First address: %p\n", head_);
    }

    if (nullptr == tail_) {
      printf("[List Info] Last address: NULL\n");
    }
    else {
      printf("[List Info] Last address: %p\n", tail_);
    }

    MemoryNode *tmp_node = head_;
    int i = 0;
    for (tmp_node; tmp_node != nullptr; tmp_node = tmp_node->next()) {
      printf("  [List Info] Storage: #%d\n", i);
      tmp_node->print();
      i++;
    }
  }
  else {
    printf("[List Info] Address NULL");
  }
}
/*
List* LIST_create(u16 capacity) {
  
  if (capacity == 0) {
    return NULL;
  }

  List *new_list = MM->malloc(sizeof(List));
  if (NULL == new_list) {
    return NULL;
  }

  MemoryNode* test_node = MM->malloc(sizeof(MemoryNode) * capacity);
  if (NULL == test_node){
    return NULL;
  }
  MM->free(test_node);
  new_list->head_ = NULL;
  new_list->tail_ = NULL;
  new_list->length_ = 0;
  new_list->capacity_ = capacity;
  new_list->ops_ = &list_ops_s;


  return new_list;
}*/