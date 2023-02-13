#include <stdio.h>
#include <stdlib.h>


#include "common_def.h"
#include "adt_dl_list.h"



DL_List::DL_List() {
  head_ = nullptr;
  tail_ = nullptr;
  capacity_ = 0;
  length_ = 0;
}

DL_List::DL_List(DL_List & copydllist) {

  head_ = nullptr;
  tail_ = nullptr;
  capacity_ = copydllist.capacity_;
  length_ =   0;

  if (copydllist.head_ != nullptr) {

    MemoryNode* iterator_node;

    for (iterator_node = copydllist.head_; iterator_node != nullptr; iterator_node = iterator_node->next()) {
      MemoryNode *new_element = new MemoryNode(*iterator_node);
      if (nullptr != tail_) {
        new_element->setPrevious(tail_);
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

DL_List& DL_List::operator=(const DL_List &dl_list) {

  head_ = nullptr;
  tail_ = nullptr;
  capacity_ = dl_list.capacity_;
  length_ =  0;

  if (dl_list.head_ != nullptr) {

    MemoryNode* iterator_node;

    for (iterator_node = dl_list.head_; iterator_node != nullptr; iterator_node = iterator_node->next()) {
      MemoryNode *new_element = new MemoryNode(*iterator_node);
      if (nullptr != tail_) {
        new_element->setPrevious(tail_);
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

DL_List::~DL_List() {
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

s16 DL_List::init(u16 capacity) {

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



s16 DL_List::softReset() {

  if (nullptr == this) {
    return kErrorCode_List_Null;
  }
  if (nullptr == head_) {
    return kErrorCode_MemoryNode_Null;
  }
  if (isEmpty()) {
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

s16 DL_List::reset() {
  if (nullptr == this) {
    return kErrorCode_List_Null;
  }
  if (nullptr == head_) {
    return kErrorCode_MemoryNode_Null;
  }
  if (isEmpty()) {
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

s16 DL_List::resize(u16 new_size) {

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

u16 DL_List::capacity() {
  if (nullptr == this) {
    return 0;
  }
  return capacity_;
}

u16 DL_List::length() {
  if (nullptr == this) {
    return 0;
  }
  return length_;
}

bool DL_List::isEmpty() {
  if (nullptr == this) {
    return false;
  }
  return length() == 0;
}

bool DL_List::isFull() {
  if (nullptr == this) {
    return false;
  }
  return length_ == capacity_;
}

void* DL_List::first() {
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

void* DL_List::last() {
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

void* DL_List::at(u16 position) {
  if (nullptr == this) {
    return nullptr;
  }
  if (position > length_) {
    return nullptr;
  }
  if (isEmpty()) {
    return nullptr;
  }
  if (nullptr == head_ || nullptr == tail_) {
    return nullptr;
  }

  MemoryNode* i_list;
  if ((position >> 1) > capacity()) {
    i_list = tail_;
    for (int i = length(); i >= position; --i) {
      i_list = i_list->previous();
    }
  }
  else {
    i_list = head_;
    for (int i = 0; i < position; ++i) {
      i_list = i_list->next();
    }
  }

  return i_list->data();
}

s16 DL_List::insertFirst(void* data, u16 bytes) {

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
  //init memory node

  new_element->setData(data, bytes);
  new_element->setNext(head_);
  head_ = new_element;

  if (nullptr != new_element->next()) {
    new_element->next()->setPrevious(new_element); // puede que este  mal
  }

  if (length() == 0) {
    tail_ = new_element;
  }
  ++length_;

  return kErrorCode_Ok;

}

s16 DL_List::insertLast(void* data, u16 bytes) {

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
  // init memory node

  new_element->setData(data, bytes);
  new_element->setNext(nullptr);


  if (nullptr != tail_) {
    new_element->setPrevious(tail_);
    tail_->setNext(new_element);
  }

  tail_ = new_element;
  if (length() == 0) {
    head_ = new_element;
  }
  ++length_;

  return kErrorCode_Ok;
}

s16 DL_List::insertAt(void* data, u16 bytes, u16 position) {
  
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
  if (position > length()) {
    insertLast(data, bytes);
  }
  else if (position <= 0) {
    insertFirst(data, bytes);
  }
  else {
    /*if (nullptr == head_ || nullptr == tail_) {
      return kErrorCode_MemoryNode_Null;
    }*/

    MemoryNode* i_list;
    if (position * 2 > capacity()) {
      i_list = tail_;
      for (int i = length(); i >= position; --i) {
        i_list = i_list->previous();
      }
    }
    else {
      i_list = head_;
      for (int i = 0; i < position; ++i) {
        i_list = i_list->next();
      }
    }

    MemoryNode *new_element = new MemoryNode();
    if (nullptr == new_element) {
      return KErrorCode_No_Memory;
    }
    //init memory node

    new_element->setData(data, bytes);
    i_list->previous()->setNext(new_element);
    new_element->setNext(i_list);
    new_element->setPrevious(i_list->previous());
    i_list->setPrevious(new_element);

    if (length() == 0) {
      tail_ = new_element;
    }
    ++length_;
  }
  return kErrorCode_Ok;
}



void* DL_List::extractFirst() {
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
  //head_->softFree();

  head_ = head_->next();
  head_->setPrevious(nullptr);
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

void* DL_List::extractLast() {
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

  MemoryNode* i_list;
 
  if (length() == 1) {

     i_list = head_;
  }
  else {
    i_list = tail_->previous();
  }


  i_list->setNext(nullptr);
  tail_->softFree();
  tail_ = i_list;
  --length_;

  if (length() == 1) {
    tail_ = head_;
  }
  if(isEmpty()) {
    head_ = nullptr;
    tail_ = nullptr;
  }

  return aux_node;
}

void* DL_List::extractAt(u16 position) {
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
    if (nullptr == head_ || nullptr == tail_) {
      return nullptr;
    }

    MemoryNode* i_list;
    if (position * 2 > capacity()) {
      i_list = tail_;
      for (int i = length(); i >= position; --i) {
        i_list = i_list->previous();
      }
    }
    else {
      i_list = head_;
      for (int i = 0; i < position; ++i) {
        i_list = i_list->next();
      }
    }

    void *aux_node = i_list->data();
    MemoryNode* tmp = i_list;
    //i_list->softFree();

    i_list->previous()->setNext(i_list->next());
    i_list->next()->setPrevious(i_list->previous());
    i_list->softFree();

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


s16 DL_List::concat(DL_List* list_src) {

  if (nullptr == this || nullptr == list_src) {
    return kErrorCode_List_Null;
  }
  if (nullptr == head_ || list_src->head_) {
    return kErrorCode_MemoryNode_Null;
  }

  capacity_ += list_src->capacity_;
  MemoryNode *i_list = list_src->head_;


  for (i_list; i_list != nullptr; i_list = i_list->next()) {
    
    MemoryNode *new_element = new MemoryNode();
    //init memory ndoe

    new_element->memCopy(i_list->data(),
                         i_list->size());
    new_element->setNext(nullptr);

    if (nullptr != tail_) {
      new_element->setPrevious(tail_);
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

s16 DL_List::traverse(void(*callback)(MemoryNode *)) {
 
  if (nullptr == this) {
    return kErrorCode_Vector_Null;
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

void DL_List::print(){

  if (nullptr != this) {

    printf("[DL_List Info] Address: %p\n", this);
    printf("[DL_List Info] Length: %d\n", length_);
    printf("[DL_List Info] Capacity: %d\n", capacity_);
    if (nullptr == head_) {
      printf("[DL_List Info] First address: NULL\n");
    }
    else {
      printf("[DL_List Info] First address: %p\n", head_);
    }

    if (nullptr == tail_) {
      printf("[DL_List Info] Last address: NULL\n");
    }
    else {
      printf("[DL_List Info] Last address: %p\n", tail_);
    }

    MemoryNode *tmp_node = head_;
    int i = 0;
    for (tmp_node; tmp_node != nullptr; tmp_node = tmp_node->next()) {
      printf("  [DL_List Info] Storage: #%d\n", i);
      tmp_node->print();
      ++i;
    }
  }
  else {
    printf("[DL_List Info] Address NULL");
  }
}
