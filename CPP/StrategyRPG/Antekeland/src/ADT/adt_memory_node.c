// adt_memory_node.c:
// Toni Barella, Ivan Sancho
// Algoritmos & Inteligencia Artificial
// ESAT 2020-2021
//

#include <stdio.h>
#include <stdlib.h>

#include "common_def.h"
#include "adt_memory_node.h"

#include "ABGS_MemoryManager/abgs_memory_manager.h"


// Memory Node Declarations
static s16 MEMNODE_initWithoutCheck(MemoryNode *node);	// inits a MN with no checks
static void* MEMNODE_data(MemoryNode *node);	// returns a reference to data_
static s16 MEMNODE_setData(MemoryNode *node, void* src, u16 bytes);
static u16 MEMNODE_size(MemoryNode *node);		// returns data size

static s16 MEMNODE_reset(MemoryNode *node);
static s16 MEMNODE_softReset(MemoryNode *node);
static s16 MEMNODE_free(MemoryNode *node);
static s16 MEMNODE_softFree(MemoryNode *node);

static s16 MEMNODE_memSet (MemoryNode *node, u8 value);
static s16 MEMNODE_memCopy (MemoryNode *node, void *src, u16 bytes);
static s16 MEMNODE_memConcat(MemoryNode *node, void *src, u16 bytes);
static s16 MEMNODE_memMask (MemoryNode *node, u8 mask);
static void MEMNODE_print (MemoryNode *node);


// Memory Node's API Definitions
struct memory_node_ops_s memory_node_ops = { .data = MEMNODE_data,
												                     .setData = MEMNODE_setData,
                                             .size = MEMNODE_size,
                                             .reset = MEMNODE_reset,
                                             .softReset = MEMNODE_softReset,
                                             .free = MEMNODE_free,
                                             .softFree = MEMNODE_softFree,
                                             .memSet = MEMNODE_memSet,
                                             .memCopy = MEMNODE_memCopy,
                                             .memConcat = MEMNODE_memConcat,
                                             .memMask = MEMNODE_memMask,
                                             .print = MEMNODE_print
};

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

s16 MEMNODE_createFromRef(MemoryNode **node) {
  if (NULL == node){
    return kErrorCode_Memory;
  }
  *node = MEMNODE_create();
  if (NULL == *node) {
#ifdef VERBOSE_
    printf("Error: [%s] not enough memory available\n", __FUNCTION__);
#endif
    return kErrorCode_Memory;
  }
  return kErrorCode_Ok;
}

s16 MEMNODE_initWithoutCheck(MemoryNode *node) {
  node->data_ = NULL;
  node->size_ = 0;
  node->ops_ = &memory_node_ops;
  return kErrorCode_Ok;
}

s16 MEMNODE_createLite(MemoryNode* node) {
  if (NULL == node) {
    return kErrorCode_Memory;
  }
  MEMNODE_initWithoutCheck(node);
  return kErrorCode_Ok;
}


void* MEMNODE_data(MemoryNode *node) { // returns a reference to data_
	if (NULL == node) {
		return NULL;
	}
  return node->data_;
}

s16 MEMNODE_setData(MemoryNode *node, void* src, u16 bytes) {
	if (NULL == node) {
		return kErrorCode_Memory;
	}
	if (NULL == src) {
		return kErrorCode_Memory;
	}
	if (0 == bytes) {
		return kErrorCode_ZeroSize;
	}
 
  node->data_ = src;
  node->size_ = bytes;

  return kErrorCode_Ok;
}

u16	MEMNODE_size(MemoryNode *node) { // returns data size
	if (NULL == node) {
		return kErrorCode_ZeroSize;
	}
  return node->size_;
}


s16 MEMNODE_reset(MemoryNode *node) {
  if (NULL == node) {
    return kErrorCode_Memory;
  }
  if (NULL == node->data_) {
    return kErrorCode_Data;
  }
  MM->free(node->data_);
  node->size_ = 0; 
  node->data_ = NULL;
  
  return kErrorCode_Ok;
}

 s16 MEMNODE_softReset(MemoryNode *node) {
  if (NULL == node) {
    return kErrorCode_Memory;
  }
  node->data_ = NULL;
  node->size_ = 0;
  return kErrorCode_Ok;
}

s16 MEMNODE_softFree(MemoryNode *node) {
  if (NULL == node) {
    return kErrorCode_Memory;
  }
  MM->free(node);
  node = NULL;
  return kErrorCode_Ok;
}

s16 MEMNODE_free(MemoryNode *node) {
  if (NULL == node) {
    return kErrorCode_Memory;
  }
  if (NULL == node->data_) {
    MM->free(node);
    node = NULL;
    return kErrorCode_Data;
  }

  //SoftFree
  MM->free(node->data_);
  node->size_ = 0;
  node->data_ = NULL;
  //Reset
  MM->free(node);
  node = NULL;
  return kErrorCode_Ok;
}

s16 MEMNODE_memSet(MemoryNode *node, u8 value) {
  if (NULL == node) {
    return kErrorCode_Memory;
  }

  memset(node->data_, value, node->size_);
  
  return kErrorCode_Ok;
}

s16 MEMNODE_memCopy(MemoryNode *node, void *src, u16 bytes) {
  if (NULL == node) {
    return kErrorCode_Memory;
  }
  if (NULL == src) {
    return kErrorCode_Memory;
  }
  if (0 == bytes) {
    return kErrorCode_ZeroSize;
  }
  
  node->ops_->reset(node);

  void * aux_mem = MM->malloc(bytes);
  memcpy(aux_mem, src, bytes);
  node->data_ = aux_mem;

  node->size_ = bytes;
  return kErrorCode_Ok;
}

s16 MEMNODE_memConcat(MemoryNode* node, void* src, u16 bytes) {
  if (NULL == node) {
    return kErrorCode_Memory;
  }
  if (NULL == node->data_ || NULL == src) {
    return kErrorCode_Data;
  }
  if (bytes < 0 || node->size_ < 0) {
    return kErrorCode_Data;
  }
  
  u16 tmp_size = node->size_;
  void*  aux_mem = MM->malloc(tmp_size + bytes);

  memcpy(aux_mem, node->data_, tmp_size);

  //Reset node
  MM->free(node->data_);
  node->size_ = 0;
  node->data_ = NULL;


  (u8)aux_mem+= tmp_size;
  memcpy(aux_mem , src, bytes);

 
  (u8)aux_mem -= tmp_size;
  node->size_ += bytes;
  node->size_ += tmp_size;

  node->data_ = aux_mem;
  return kErrorCode_Ok;
}

s16 MEMNODE_memMask(MemoryNode *node, u8 mask) {
  if (NULL == node) {
    return kErrorCode_Memory;
  }

  if (NULL == node->data_) {
    return kErrorCode_Data;
  }

  if (node->size_ == 0) {
    return kErrorCode_ZeroSize;
  }

  u8* aux_data = (u8*)node->data_;


  for (int i = 0; i < node->size_; ++i) {
    *aux_data = (u8)*aux_data & mask;
    ++aux_data;
  }
  
  return kErrorCode_Ok;
}

void MEMNODE_print(MemoryNode *node) {
  if(NULL != node){
    printf("     ");
    printf("[Node Info] Address: %p\n", node);
    printf("     ");
    printf("[Node Info] Size: %d\n", node->size_);
    printf("     ");
    printf("[Node Info] Data addres: %p\n", node->data_);
    printf("     ");
    
    // ir byte por byte
    
    printf("[Node Info] Data content: ");
    if (NULL != node->data_) {
 
      u8* ptr_iterator = (u8*)node->ops_->data(node);
      for (u16 i = 0; i < node->ops_->size(node); ++i)
      {
        printf("%c", *(ptr_iterator++));
      }


    }
    else {
      printf("NULL\n");
    }
    
    printf("\n");
   // printf("[Node Info] Next address: %d", node->size_);
   // printf("[Node Info] Prev address: %d", node->size_);

  }
}