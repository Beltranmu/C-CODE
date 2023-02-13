#include <stdio.h>
#include <stdlib.h>


#include "common_def.h"
#include "adt_list.h"
#include "ABGS_MemoryManager/abgs_memory_manager.h"

//Vector Declaratoon
static s16 LIST_destroy(List* list);
static s16 LIST_softReset(List* list);
static s16 LIST_reset(List* list);
static s16 LIST_resize(List* list, u16 new_size);

static u16 LIST_capacity(List* list);
static u16 LIST_length(List* list);
static bool LIST_isEmpty(List* list);
static bool LIST_isFull(List* list);

static void* LIST_first(List* list);
static void* LIST_last(List* list);
static void* LIST_at(List* list, u16 position);

static s16 LIST_insertFirst(List* list, void* data, u16 bytes);
static s16 LIST_insertLast(List* list, void* data, u16 bytes);
static s16 LIST_insertAt(List* list, void* data, u16 bytes, u16 position);

static void* LIST_extractFirst(List* list);
static void* LIST_extractLast(List* list);
static void* LIST_extractAt(List* list, u16 position);

static s16 LIST_concat(List* list, List*  list_src);
static s16 LIST_traverse(List* list, void(*callback)(MemoryNode *));
static void LIST_print(List* list);




// Vector's API Definitions
struct list_ops_s list_ops_s = { .destroy = LIST_destroy,
                                 .softReset = LIST_softReset,
                                 .reset = LIST_reset,
                                 .resize = LIST_resize,
                                 .capacity = LIST_capacity,
                                 .length = LIST_length, 
                                 .isEmpty = LIST_isEmpty,
                                 .isFull = LIST_isFull,
                                 .first = LIST_first,
                                 .last = LIST_last,
                                 .at = LIST_at,
                                 .insertFirst = LIST_insertFirst,
                                 .insertLast = LIST_insertLast,
                                 .insertAt = LIST_insertAt,
                                 .extractFirst = LIST_extractFirst,
                                 .extractLast = LIST_extractLast,
                                 .extractAt = LIST_extractAt,
                                 .concat = LIST_concat,
                                 .traverse = LIST_traverse,
                                 .print = LIST_print             
};


// List Definitions

s16 LIST_destroy(List* list){
  
  if (list == NULL) {
    return kErrorCode_List_Null;
  }
  //MemoryNode* i_list = list->head_;
  //MemoryNode* delete_node = i_list;
  s16 return_code = list->ops_->reset(list);

  MM->free(list);

  return return_code;
}

s16 LIST_softReset(List* list) {

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

s16 LIST_reset(List* list) {
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

s16 LIST_resize(List* list, u16 new_size) {

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

u16 LIST_capacity(List* list) {
  if (NULL == list) {
    return 0;
  }
  return list->capacity_;
}

u16 LIST_length(List* list) {
  if (NULL == list) {
    return 0;
  }
  return (list->length_);
}

bool LIST_isEmpty(List* list) {
  if (NULL == list) {
    return false;
  }
  return list->ops_->length(list) == 0;
}

bool LIST_isFull(List* list) {
  if (NULL == list) {
    return false;
  }
  return list->length_ == list->capacity_;
}


void* LIST_first(List* list) {
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

void* LIST_last(List* list) {
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

void* LIST_at(List* list, u16 position) {
  if (NULL == list) {
    return NULL;
  }
  if (NULL == list->head_) {
    return NULL;
  }
  if (position > list->length_) {
    return NULL;
  }
  if (list->ops_->isEmpty(list)) {
    return NULL;
  }
  if (position == list->length_) {
    return list->ops_->last(list);
  }
  MemoryNode* i_list = list->head_;
  //Puede un bug, creoq ue esta solucionado
  for (int i = 0; i < position; ++i) {
    
    i_list = i_list->ops_->next(i_list);
  }
  return i_list->ops_->data(i_list);
}


s16 LIST_insertFirst(List* list, void* data, u16 bytes) {

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

  if (list->ops_->length(list) == 0) {
    list->tail_ = new_element;
  }
  ++list->length_;

  return kErrorCode_Ok;

}

s16 LIST_insertLast(List* list, void* data, u16 bytes) {

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
    list->tail_->ops_->setNext(list->tail_, new_element);
  }

  list->tail_ = new_element;
  if (list->ops_->length(list) == 0) {
    list->head_ = new_element;
  }
  ++list->length_;


  return kErrorCode_Ok;
}

s16 LIST_insertAt(List* list, void* data, u16 bytes, u16 position) {
  
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
    MemoryNode* i_list = list->head_;

    for (int i = 0; i < (position - 1 ); ++i) {
      i_list = i_list->ops_->next(i_list);
    }

    MemoryNode *new_element = MM->malloc(sizeof(MemoryNode));
    if (NULL == new_element) {
      return KErrorCode_No_Memory;
    }
    MEMNODE_createLite(new_element);

    new_element->ops_->setData(new_element, data, bytes);
    new_element->ops_->setNext(new_element, i_list->ops_->next(i_list));
    i_list->ops_->setNext(i_list, new_element);

    if (list->ops_->length(list) == 0) {
      list->tail_ = new_element;
    }

    ++list->length_;
    
  }
  return kErrorCode_Ok;
}


void* LIST_extractFirst(List* list) {  
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
  --list->length_;

  if (list->ops_->isEmpty(list)) {
    list->head_ = NULL;
    list->tail_ = NULL;
  }
  
  if (list->ops_->length(list) == 1) {
    list->tail_ = list->head_;
  }
  return aux_node;
}

void* LIST_extractLast(List* list) {
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

  MemoryNode* i_list = list->head_;

  i_list->ops_->softFree(list->tail_);

  for (int i = 0; i < list->ops_->length(list) - 2; ++i) {
    i_list = i_list->ops_->next(i_list);

  }
  i_list->ops_->setNext(i_list, NULL);
  list->tail_ = i_list;
  list->length_--;

  if (list->ops_->isEmpty(list)) {
    list->head_ = NULL;
    list->tail_ = NULL;
  }
  if (list->ops_->length(list) == 1) {
    list->tail_ = list->head_;
  }
  return aux_node;
}

void* LIST_extractAt(List* list, u16 position) {
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
    if (NULL == list->head_) {
      return NULL;
    }
    MemoryNode *i_list = list->head_;
    for (int i = 0; i < position-1 ; ++i) {
      i_list = i_list->ops_->next(i_list);
    }

    void *aux_node = i_list->ops_->data(i_list->ops_->next(i_list));
    i_list->ops_->softFree(i_list->ops_->next(i_list));
    i_list->ops_->setNext(i_list, i_list->ops_->next(i_list->ops_->next(i_list)));

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


s16 LIST_concat(List* list, List* list_src) {

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

s16 LIST_traverse(List* list, void(*callback)(MemoryNode *)) {
 
  if (NULL == list) {
    return kErrorCode_List_Null;
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

void LIST_print(List* list){

  if (NULL != list) {

    printf("[List Info] Address: %p\n", &list);
    printf("[List Info] Length: %d\n", list->length_);
    printf("[List Info] Capacity: %d\n", list->capacity_);
    if (NULL == list->head_) {
      printf("[List Info] First address: NULL\n");
    }
    else {
      printf("[List Info] First address: %p\n", list->head_);
    }

    if (NULL == list->tail_) {
      printf("[List Info] Last address: NULL\n");
    }
    else {
      printf("[List Info] Last address: %p\n", list->tail_);
    }

    MemoryNode *tmp_node = list->head_;
    int i = 0;
    for (tmp_node; tmp_node != NULL; tmp_node = tmp_node->next_) {
      printf("  [List Info] Storage:#%d\n", i);
      tmp_node->ops_->print(tmp_node);
      i++;
    }
  }
  else {
    printf("[List Info] Address: NULL");
  }
}

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
}