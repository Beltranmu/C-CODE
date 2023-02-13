#include <stdio.h>
#include <stdlib.h>


#include "common_def.h"
#include "adt_dl_list.h"
#include "ABGS_MemoryManager/abgs_memory_manager.h"

//Vector Declaratoon
static s16 DL_LIST_destroy(DL_List* list);
static s16 DL_LIST_softReset(DL_List* list);
static s16 DL_LIST_reset(DL_List* list);
static s16 DL_LIST_resize(DL_List* list, u16 new_size);

static u16 DL_LIST_capacity(DL_List* list);
static u16 DL_LIST_length(DL_List* list);
static bool DL_LIST_isEmpty(DL_List* list);
static bool DL_LIST_isFull(DL_List* list);

static void* DL_LIST_first(DL_List* list);
static void* DL_LIST_last(DL_List* list);
static void* DL_LIST_at(DL_List* list, u16 position);

static s16 DL_LIST_insertFirst(DL_List* list, void* data, u16 bytes);
static s16 DL_LIST_insertLast(DL_List* list, void* data, u16 bytes);
static s16 DL_LIST_insertAt(DL_List* list, void* data, u16 bytes, u16 position);

static void* DL_LIST_extractFirst(DL_List* list);
static void* DL_LIST_extractLast(DL_List* list);
static void* DL_LIST_extractAt(DL_List* list, u16 position);

static s16 DL_LIST_concat(DL_List* list, DL_List*  list_src);
static s16 DL_LIST_traverse(DL_List* list, void(*callback)(MemoryNode *));
static void DL_LIST_print(DL_List* list);




// Vector's API Definitions
struct dl_list_ops_s dl_list_ops_s = { .destroy = DL_LIST_destroy,
                                       .softReset = DL_LIST_softReset,
                                       .reset = DL_LIST_reset,
                                       .resize = DL_LIST_resize,
                                       .capacity = DL_LIST_capacity,
                                       .length = DL_LIST_length, 
                                       .isEmpty = DL_LIST_isEmpty,
                                       .isFull = DL_LIST_isFull,
                                       .first = DL_LIST_first,
                                       .last = DL_LIST_last,
                                       .at = DL_LIST_at,
                                       .insertFirst = DL_LIST_insertFirst,
                                       .insertLast = DL_LIST_insertLast,
                                       .insertAt = DL_LIST_insertAt,
                                       .extractFirst = DL_LIST_extractFirst,
                                       .extractLast = DL_LIST_extractLast,
                                       .extractAt = DL_LIST_extractAt,
                                       .concat = DL_LIST_concat,
                                       .traverse = DL_LIST_traverse,
                                       .print = DL_LIST_print             
};

// List Definitions

s16 DL_LIST_destroy(DL_List* list){
  
  if (list == NULL) {
    return kErrorCode_List_Null;
  }

  s16 return_code = list->ops_->reset(list);

  MM->free(list);

  return return_code;
}

s16 DL_LIST_softReset(DL_List* list) {

  if (list == NULL) {
    return kErrorCode_List_Null;
  }
  if (NULL == list->head_) {
    return kErrorCode_MemoryNode_Null;
  }
  if (list->ops_->isEmpty(list)) {
    return kErrorCode_List_Empty;
  }

  MemoryNode* i_list = list->head_;
  for (i_list; i_list != NULL; i_list = i_list->ops_->next(i_list)) {
    i_list->ops_->softFree(i_list);
  }
  list->length_ = 0;
  list->head_ = NULL;
  list->tail_ = NULL;

  return kErrorCode_Ok;
}

s16 DL_LIST_reset(DL_List* list) {
  if (list == NULL) {
    return kErrorCode_List_Null;
  }
  if (NULL == list->head_) {
    return kErrorCode_MemoryNode_Null;
  }
  if (list->ops_->isEmpty(list)) {
    return kErrorCode_List_Empty;
  }
  //Recorrer la lista

  MemoryNode* deleted_node = list->head_;

  MemoryNode* next_deleted_node = NULL;

  while (deleted_node != NULL) {
    next_deleted_node = deleted_node->next_;
    deleted_node->ops_->free(deleted_node);
    deleted_node = next_deleted_node;
  }

  list->length_ = 0;
  list->head_ = NULL;
  list->tail_ = NULL;
  return kErrorCode_Ok;
}

s16 DL_LIST_resize(DL_List* list, u16 new_size) {

  if (NULL == list) {
    return kErrorCode_List_Null;
  }

  if (new_size <= 0) {
    return kErrorCode_ZeroSize;
  }

  if (list->capacity_ > new_size) {

    MemoryNode* i_list = list->head_;
    MemoryNode* i_list_next = NULL;
    int i = 0;
    for (i_list; i_list != NULL; i_list = i_list_next) {
      i_list_next = i_list->ops_->next(i_list);
      //New Tail
      if (i == new_size - 1) {
        list->tail_ = i_list;
        list->tail_->ops_->setNext(list->tail_, NULL);
      }

      if (i >= new_size) {
        i_list->ops_->free(i_list);
      }
      i_list = i_list_next;
      ++i;
    }
    list->length_ = new_size;
  }


  list->capacity_ = new_size;

   return kErrorCode_Ok;
}

u16 DL_LIST_capacity(DL_List* list) {
  if (NULL == list) {
    return 0;
  }
  return list->capacity_;
}

u16 DL_LIST_length(DL_List* list) {
  if (NULL == list) {
    return 0;
  }
  return (list->length_);
}

bool DL_LIST_isEmpty(DL_List* list) {
  if (NULL == list) {
    return false;
  }
  return list->ops_->length(list) == 0;
}

bool DL_LIST_isFull(DL_List* list) {
  if (NULL == list) {
    return false;
  }
  return list->length_ == list->capacity_;
}

void* DL_LIST_first(DL_List* list) {
  if (NULL == list) {
    return NULL;
  }
  if (list->ops_->isEmpty(list)) {
    return NULL;
  }

  if (NULL == list->head_) {
    return NULL;
  }

  return list->head_->ops_->data(list->head_);
}

void* DL_LIST_last(DL_List* list) {
  if (NULL == list) {
    return NULL;
  }
  if (list->ops_->isEmpty(list)) {
    return NULL;
  }
  if (NULL == list->tail_) {
    return NULL;
  }

  return list->tail_->ops_->data(list->tail_);
}

void* DL_LIST_at(DL_List* list, u16 position) {
  if (NULL == list) {
    return NULL;
  }
  if (position > list->length_) {
    return NULL;
  }
  if (list->ops_->isEmpty(list)) {
    return NULL;
  }
  if (NULL == list->head_ || NULL == list->tail_) {
    return NULL;
  }

  MemoryNode* i_list;
  if (position * 2 > list->ops_->capacity(list)) {
    i_list = list->tail_;
    for (int i = list->ops_->length(list); i >= position; --i) {
      i_list = i_list->ops_->previous(i_list);
    }
  }
  else {
    i_list = list->head_;
    for (int i = 0; i < position; ++i) {
      i_list = i_list->ops_->next(i_list);
    }
  }

 
  return i_list->ops_->data(i_list);
}

s16 DL_LIST_insertFirst(DL_List* list, void* data, u16 bytes) {

  if (NULL == list) {
    return kErrorCode_List_Null;
  }
  if (NULL == data) {
    return kErrorCode_Data_Null;
  }
  if (bytes <= 0) {
    return kErrorCode_ZeroSize;
  }
  if (list->ops_->isFull(list)) {
    return kErrorCode_List_Full;
  }

  MemoryNode *new_element = MM->malloc(sizeof(MemoryNode));
  if (NULL == new_element) {
    return KErrorCode_No_Memory;
  }
  MEMNODE_createLite(new_element);

  new_element->ops_->setData(new_element, data, bytes);
  new_element->ops_->setNext(new_element, list->head_);
  list->head_ = new_element;

  if (NULL != new_element->ops_->next(new_element)) {
    new_element->ops_->setPrevious(new_element->ops_->next(new_element), new_element);
  }

  if (list->ops_->length(list) == 0) {
    list->tail_ = new_element;
  }
  ++list->length_;

  return kErrorCode_Ok;

}

s16 DL_LIST_insertLast(DL_List* list, void* data, u16 bytes) {

  if (NULL == list) {
    return kErrorCode_List_Null;
  }
  if (NULL == data) {
    return kErrorCode_Data_Null;
  }
  if (bytes <= 0) {
    return kErrorCode_ZeroSize;
  }

  if (list->ops_->isFull(list)) {
    return kErrorCode_List_Full;
  }

  MemoryNode *new_element = MM->malloc(sizeof(MemoryNode));
  if (NULL == new_element) {
    return KErrorCode_No_Memory;
  }
  MEMNODE_createLite(new_element);

  new_element->ops_->setData(new_element, data, bytes);
  new_element->ops_->setNext(new_element, NULL);


  if (NULL != list->tail_) {
    new_element->ops_->setPrevious(new_element, list->tail_);
    list->tail_->ops_->setNext(list->tail_, new_element);
  }

  list->tail_ = new_element;
  if (list->ops_->length(list) == 0) {
    list->head_ = new_element;
  }
  ++list->length_;


  return kErrorCode_Ok;
}

s16 DL_LIST_insertAt(DL_List* list, void* data, u16 bytes, u16 position) {
  
  if (NULL == list) {
    return kErrorCode_List_Full;
  }
  if (NULL == data) {
    return kErrorCode_Data_Null;
  }
  if (bytes <= 0) {
    return kErrorCode_ZeroSize;
  }
  if (list->ops_->isFull(list)) {
    return kErrorCode_List_Full;
  }
  if (position >= list->ops_->length(list)) {
    list->ops_->insertLast(list, data, bytes);
  }
  else if (position <= 0) {
    list->ops_->insertFirst(list, data, bytes);
  }
  else {
    /*if (NULL == list->head_ || NULL == list->tail_) {
      return NULL;
    }*/

    MemoryNode* i_list;
    if (position * 2 > list->ops_->capacity(list)) {
      i_list = list->tail_;
      for (int i = list->ops_->length(list); i >= position; --i) {
        i_list = i_list->ops_->previous(i_list);
      }
    }
    else {
      i_list = list->head_;
      for (int i = 0; i < position; ++i) {
        i_list = i_list->ops_->next(i_list);
      }
    }

    MemoryNode *new_element = MM->malloc(sizeof(MemoryNode));
    if (NULL == new_element) {
      return KErrorCode_No_Memory;
    }
    MEMNODE_createLite(new_element);

    new_element->ops_->setData(new_element, data, bytes);
    i_list->ops_->setNext(i_list->ops_->previous(i_list), new_element);
    i_list->ops_->setNext(new_element, i_list);
    i_list->ops_->setPrevious(new_element, i_list->ops_->previous(i_list));
    i_list->ops_->setPrevious(i_list, new_element);

    if (list->ops_->length(list) == 0) {
      list->tail_ = new_element;
    }

    ++list->length_;
    
  }
  return kErrorCode_Ok;
}


void* DL_LIST_extractFirst(DL_List* list) {
  if (NULL == list) {
    return NULL;
  }
  if (list->ops_->isEmpty(list)) {
    return NULL;
  }

  if (NULL == list->head_) {
    return NULL;
  }

  void *aux_node = list->head_->ops_->data(list->head_);

  list->head_->ops_->softFree(list->head_);

  list->head_ = list->head_->next_;
  if (NULL != list->head_) {

    list->head_->ops_->setPrevious(list->head_, NULL);
  }

  --list->length_;

  if (list->ops_->length(list) == 1) {
    list->tail_ = list->head_;
  }

  if (list->ops_->isEmpty(list)) {
    list->head_ = NULL;
    list->tail_ = NULL;
  }


  return aux_node;
}

void* DL_LIST_extractLast(DL_List* list) {
  if (NULL == list) {
    return NULL;
  }
  if (list->ops_->isEmpty(list)) {
    return NULL;
  }
  if (NULL == list->tail_ || NULL == list->head_) {
    return NULL;
  }

  void *aux_node = list->tail_->ops_->data(list->tail_);

  MemoryNode* i_list;
 
  if (list->ops_->length(list) == 1) {

     i_list = list->head_;
  }
  else {
    i_list = list->tail_->ops_->previous(list->tail_);
  }

  i_list->ops_->softFree(list->tail_);

  list->tail_ = i_list;
  i_list->ops_->setNext(i_list, NULL);
  --list->length_;

  if (list->ops_->length(list) == 1) {
    list->tail_ = list->head_;
  }

  if (list->ops_->isEmpty(list)) {
    list->head_ = NULL;
    list->tail_ = NULL;
  }

  return aux_node;
}

void* DL_LIST_extractAt(DL_List* list, u16 position) {
  if (NULL == list) {
    return NULL;
  }
  if (list->ops_->isEmpty(list)) {
    return NULL;
  }
  if (position > list->ops_->length(list)) {
    return NULL;
  }
  else if (position == list->ops_->length(list)) {
    return list->ops_->extractLast(list);
  }
  else if (position == 0) {
    return list->ops_->extractFirst(list);
  }
  else {
    if (NULL == list->head_ || NULL == list->tail_) {
      return NULL;
    }

    MemoryNode* i_list;
    if (position * 2 > list->ops_->capacity(list)) {
      i_list = list->tail_;
      for (int i = list->ops_->length(list); i >= position; --i) {
        i_list = i_list->ops_->previous(i_list);
      }
    }
    else {
      i_list = list->head_;
      for (int i = 0; i < position; ++i) {
        i_list = i_list->ops_->next(i_list);
      }
    }

    void *aux_node = i_list->ops_->data(i_list);
    i_list->ops_->softFree(i_list);

    i_list->ops_->setNext(i_list->ops_->previous(i_list),
                          i_list->ops_->next(i_list));

    i_list->ops_->setPrevious(i_list->ops_->next(i_list),
                              i_list->ops_->previous(i_list));

    --list->length_;

    if (list->ops_->length(list) == 1) {
      list->tail_ = list->head_;
    }

    if (list->ops_->isEmpty(list)) {
      list->head_ = NULL;
      list->tail_ = NULL;
    }
    return aux_node;
  }

}


s16 DL_LIST_concat(DL_List* list, DL_List* list_src) {

  if (NULL == list || NULL == list_src) {
    return kErrorCode_List_Null;
  }
  if (NULL == list_src->head_) {
    return kErrorCode_MemoryNode_Null;
  }
  /*if (NULL == list->head_) {
    return kErrorCode_MemoryNode_Null;
  }*/

  list->capacity_ += list_src->capacity_;
  MemoryNode *i_list = list_src->head_;


  for (i_list; i_list != NULL; i_list = i_list->ops_->next(i_list)) {
    
    MemoryNode *new_element = MM->malloc(sizeof(MemoryNode));
    if (NULL == new_element) {
      return KErrorCode_No_Memory;
    }
    MEMNODE_createLite(new_element);

    new_element->ops_->memCopy(new_element,
                               i_list->ops_->data(i_list),
                               i_list->ops_->size(i_list));
    new_element->ops_->setNext(new_element, NULL);

    if (NULL != list->tail_) {
      list->tail_->ops_->setPrevious(new_element, list->tail_);
      list->tail_->ops_->setNext(list->tail_, new_element);
    }

    list->tail_ = new_element;
    if (list->ops_->length(list) == 0) {
      list->head_ = new_element;
    }
    ++list->length_;
  }


  return kErrorCode_Ok;

}

s16 DL_LIST_traverse(DL_List* list, void(*callback)(MemoryNode *)) {
 
  if (NULL == list) {
    return kErrorCode_Vector_Null;
  }
  if (NULL == list->head_) {
    return kErrorCode_MemoryNode_Null;
  }

  if (NULL == callback) {
    return kErrorCode_NonFuction;
  }

  MemoryNode *tmp_node = list->head_;
  for (tmp_node; tmp_node != NULL; tmp_node = tmp_node->next_) {
    callback(tmp_node);
  }
  return kErrorCode_Ok; 
}

void DL_LIST_print(DL_List* list){

  if (NULL != list) {

    printf("[DL_List Info] Address: %p\n", &list);
    printf("[DL_List Info] Length: %d\n", list->length_);
    printf("[DL_List Info] Capacity: %d\n", list->capacity_);
    if (NULL == list->head_) {
      printf("[DL_List Info] First address: NULL\n");
    }
    else {
      printf("[DL_List Info] First address: %p\n", list->head_);
    }

    if (NULL == list->tail_) {
      printf("[DL_List Info] Last address: NULL\n");
    }
    else {
      printf("[DL_List Info] Last address: %p\n", list->tail_);
    }

    MemoryNode *tmp_node = list->head_;
    int  i = 0;
    for (tmp_node; tmp_node != NULL; tmp_node = tmp_node->next_) {
      printf("  [DL_List Info] Storage:#%d\n", i);
      tmp_node->ops_->print(tmp_node);
      ++i;
    }
  }
  else {
    printf("[DL_List Info] Address: NULL");
  }
}

DL_List* DL_LIST_create(u16 capacity) {
  
  if (capacity == 0) {
    return NULL;
  }

  DL_List *new_list = MM->malloc(sizeof(DL_List));
  
  if (NULL == new_list) {
    return NULL;
  }

  MemoryNode* test_node = MM->malloc(sizeof(MemoryNode) * capacity);
  if (NULL == test_node) {
    return NULL;
  }
  MM->free(test_node);

  new_list->head_ = NULL;
  new_list->tail_ = NULL;
  new_list->length_ = 0;
  new_list->capacity_ = capacity;
  new_list->ops_ = &dl_list_ops_s;


  return new_list;
}