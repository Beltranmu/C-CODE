#include <stdio.h>
#include <stdlib.h>


#include "common_def.h"
#include "adt_logger.h"

#include "ABGS_MemoryManager/abgs_memory_manager.h"

//Vector Declaratoon
static s16 LOGGER_destroy(Logger* logger);
static s16 LOGGER_write(Logger* logger, char* text);
static s16 LOGGER_flush(Logger* logger);



// Vector's API Definitions
struct logger_ops_s logger_ops_s = {.destroy = LOGGER_destroy,
                                    .write = LOGGER_write,
                                    .flush = LOGGER_flush,       
};




// Logger Definitions

s16 LOGGER_destroy(Logger* logger){
  if (NULL == logger) {
    return kErrorCode_Logger_Null;
  }
  if (NULL == logger->storage_) {
    MM->free(logger);
    return kErrorCode_Queue_Null;
  }

  int return_code = logger->storage_->ops_->destroy(logger->storage_);

  MM->free(logger);
  return return_code;

}

s16 LOGGER_write(Logger* logger, char* text) {
  if (NULL == logger) {
    return kErrorCode_Logger_Null;
  }
  if (NULL == logger->storage_) {
    return kErrorCode_Queue_Null;
  }

  //size del data
  int len = strlen(text);

  return logger->storage_->ops_->enqueue(logger->storage_, (void*)text, len);;
}

s16 LOGGER_flush(Logger* logger) {
  if (NULL == logger) {
    return kErrorCode_Logger_Null;
  }
  if (NULL == logger->storage_) {
    return kErrorCode_Queue_Null;
  }
  if (logger->storage_->ops_->isEmpty(logger->storage_)) {
    return KErrorCode_Logger_Empty;
  }
  char* output_text;
  while (!logger->storage_->ops_->isEmpty(logger->storage_)) {
    output_text = (char*)logger->storage_->ops_->dequeue(logger->storage_);

    u8* ptr_iterator = (u8*)output_text;
    for (u16 i = 0; i < strlen(output_text); ++i)
    {
      printf("%c", *(ptr_iterator++));
    }
    printf("\n");
  }
  
  return kErrorCode_Ok;
}

Logger* LOGGER_create(u16 capacity) {
  
  if (capacity == 0) {
    return NULL;
  }
  Logger *new_logger = MM->malloc(sizeof(Logger));
  
  if (NULL == new_logger) {
    return NULL;
  }
  new_logger->storage_ = QUEUE_create(capacity);
  if (NULL == new_logger->storage_) {
    return NULL;
  }
  new_logger->ops_ = &logger_ops_s;
  return new_logger;
}