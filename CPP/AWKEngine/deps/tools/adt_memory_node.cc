// adt_memory_node.c:
// Toni Barella, Ivan Sancho
// Algoritmos & Inteligencia Artificial
// ESAT 2020-2021
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common_def.h"
#include "adt_memory_node.h"

// Añadir operador igual y constructor de copia
MemoryNode::MemoryNode() {
  data_ = nullptr;
  size_ = 0;
  next_ = nullptr;
  previous_ = nullptr;
}
// Memory Node Definitions



MemoryNode::MemoryNode(MemoryNode& copynode) {
  size_ = copynode.size_;
  next_ = copynode.next_;
  previous_ = copynode.previous_;

  data_ = new u8[size_];
  memcpy(data_, copynode.data_, copynode.size_);
 // *data_ = *copynode.data_
  
}

MemoryNode::~MemoryNode() {
  size_ = 0;
  next_ = nullptr;
  previous_ = nullptr;
  if (nullptr != data_) {
    delete[] data_;
  }
  data_ = nullptr;
}


MemoryNode& MemoryNode::operator=(const MemoryNode& other){
  size_ = other.size_;
  next_ = other.next_;
  previous_ = other.previous_;

  data_ = new u8[size_];
  memcpy(data_, other.data_, other.size_);

  return *this;
}

void MemoryNode::init() {


}

void* MemoryNode::data() { // returns a reference to data_
  if (nullptr == this) {
    return nullptr;
  }
  return data_;
}

s16 MemoryNode::setData(void* src, u16 bytes) {
  if (nullptr == this) {
    return kErrorCode_MemoryNode_Null;
  }
  if (nullptr == src) {
    return kErrorCode_MemoryNode_Null;
  }
  if (0 == bytes) {
    return kErrorCode_ZeroSize;
  }
  data_ = src;
  size_ = bytes;
  return kErrorCode_Ok;
}

MemoryNode* MemoryNode::next() {
  if (nullptr == this) {
    return nullptr;
  }
  return next_;
}
s16 MemoryNode::setNext(MemoryNode *next_node) {
  if (nullptr == this) {
    return kErrorCode_MemoryNode_Null;
  }
  next_ = next_node;
  return kErrorCode_Ok;
}
MemoryNode* MemoryNode::previous() {
  if (nullptr == this) {
    return nullptr;
  }
  return previous_;
}

s16 MemoryNode::setPrevious(MemoryNode *preivous_node) {
  if (nullptr == this) {
    return kErrorCode_MemoryNode_Null;
  }
  previous_ = preivous_node;
  return kErrorCode_Ok;
}

u16	MemoryNode::size() { // returns data size
  if (nullptr == this) {
    return kErrorCode_ZeroSize;
  }
  return size_;
}


s16 MemoryNode::reset() {
  if (nullptr == this ) {
    return kErrorCode_MemoryNode_Null;
  }
  if (nullptr == data_) {
    return kErrorCode_Data_Null;
  }

  
  u8* men_to_delete = (u8*)data_;
  delete[] men_to_delete;

  //operator delete (data_);
  //delete data_;
  size_ = 0;
  data_ = nullptr;
  next_ = nullptr;
  previous_ = nullptr;

  return kErrorCode_Ok;
}

s16 MemoryNode::softReset() {
  if (nullptr == this) {
    return kErrorCode_MemoryNode_Null;
  }
  data_ = nullptr;
  size_ = 0;
  next_ = nullptr;
  previous_ = nullptr;
  return kErrorCode_Ok;
}



s16 MemoryNode::free() {
  if (nullptr == this) {
    return kErrorCode_MemoryNode_Null;
  }
  if (nullptr == data_) {
    delete this;
    return kErrorCode_Data_Null;
  }

  //SoftFree
  //MM->free(node->data_);
  //delete[] data_;
  u8* men_to_delete = (u8*)data_;
  delete[] men_to_delete;

  size_ = 0;
  data_ = nullptr;
  next_ = nullptr;
  previous_ = nullptr;
  //Reset
  delete this;

  return kErrorCode_Ok;
}



s16 MemoryNode::softFree() {
  if (nullptr == this) {
    return kErrorCode_MemoryNode_Null;
  }
  data_ = nullptr;
  next_ = nullptr;
  previous_ = nullptr;
  /*
  MM->free(node);
  node = NULL;*/
  delete this;
  return kErrorCode_Ok;
}


s16 MemoryNode::memSet(u8 value) {
  if (nullptr == this){
    return kErrorCode_MemoryNode_Null;
  }
  memset(data_, value, size_);
  return kErrorCode_Ok;
}

s16 MemoryNode::memCopy(void *src, u16 bytes) {

  if (nullptr == src) {
    return kErrorCode_MemoryNode_Null;
  }
  if (0 == bytes) {
    return kErrorCode_ZeroSize;
  }

  //reset();
  void *aux_mem = (void*) new u8[bytes];

  //para eliminarlo 
  /*
    void *data_ = (void*) new u8[bytes];

    u8* men_to_delete = (u8*) data_
    delete [] men_to_delete
  */

  if (nullptr == aux_mem) {
    return KErrorCode_No_Memory;
  }

  memcpy(aux_mem, src, bytes);
  data_ = aux_mem;

  size_ = bytes;
  return kErrorCode_Ok;
}

s16 MemoryNode::memConcat(void* src, u16 bytes) {
  if (nullptr== this) {
    return kErrorCode_MemoryNode_Null;
  }
  if (nullptr == data_ || nullptr == src) {
    return kErrorCode_Data_Null;
  }
  if (bytes < 0 || size_ < 0) {
    return kErrorCode_ZeroSize;;
  }

  u16 tmp_size = size_;
  void *aux_mem = new void*[tmp_size + bytes];

  if (nullptr == aux_mem) {
    return KErrorCode_No_Memory;
  }
  memcpy(aux_mem, data_, tmp_size);

  //Reset node
  //MM->free(node->data_);
  delete[] data_;
  size_ = 0;
  data_ = NULL;

  aux_mem = (void*)((u8*)aux_mem + tmp_size);
  memcpy(aux_mem, src, bytes);
  aux_mem = (void*)((u8*)aux_mem - tmp_size);

  size_ += bytes;
  size_ += tmp_size;

  data_ = aux_mem;
  return kErrorCode_Ok;
}

s16 MemoryNode::memMask(u8 mask) {
  if (nullptr== this) {
    return kErrorCode_MemoryNode_Null;
  }

  if (nullptr== data_) {
    return kErrorCode_Data_Null;
  }
  if (size_ == 0) {
    return kErrorCode_ZeroSize;
  }

  u8* aux_data = (u8*)data_;
  for (int i = 0; i < size_; ++i) {
    *aux_data = (u8)*aux_data & mask;
    ++aux_data;
  }

  return kErrorCode_Ok;
}

void MemoryNode::print() {
  if (nullptr != this) {
    printf("     ");
    printf("[Node Info] Address: %p\n", this);
    printf("     ");
    printf("[Node Info] Size: %d\n", size_);
    printf("     ");
    printf("[Node Info] Data addres: %p\n", data_);
    printf("     ");


    // ir byte por byte

    printf("[Node Info] Data content: ");
    if (nullptr != data_) {

      u8* ptr_iterator = (u8*)data();
      for (u16 i = 0; i < size(); ++i){
        printf("%c", *(ptr_iterator++));
      }

      printf("\n");
    }
    else {
      printf("NULL\n");
    }
    printf("     ");
    printf("[Node Info] Next address: ");
    if (nullptr == next_) {
      printf("NULL\n");
    }
    else {
      printf("%p\n", next_);
    }

    printf("     ");
    printf("[Node Info] Previous address: ");
    if (nullptr == previous_) {
      printf("NULL\n");
    }
    else {
      printf("%p\n", previous_);
    }

    printf("\n");

  }
  else {
    printf("[Node Info] Address: NULL ");
  }
}




/*

// Memory Node Definitions
MemoryNode* MEMNODE_create() {
  MemoryNode *node = MM->malloc(sizeof(MemoryNode));
  if (NULL == node) {
#ifdef VERBOSE_
    printf("Error: [%s] not enough memory available\n", __FUNCTION__);
#endif
    return NULL;
  }
  MEMNODE_initWithoutCheck(node);
  return node;
}



s16 MEMNODE_initWithoutCheck(MemoryNode *node) {
  node->data_ = NULL;
  node->size_ = 0;
  node->next_ = NULL;
  node->previous_ = NULL;
  node->ops_ = &memory_node_ops;
  return kErrorCode_Ok;
}

s16 MEMNODE_createLite(MemoryNode* node) {
  if (NULL == node) {
    return kErrorCode_Memory_Null;
  }
  MEMNODE_initWithoutCheck(node);
  return kErrorCode_Ok;
}



*/