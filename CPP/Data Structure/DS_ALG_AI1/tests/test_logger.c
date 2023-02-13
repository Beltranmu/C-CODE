// test_queue.c
// Escuela Superior de Arte y Tecnologia
// Algoritmos & Inteligencia Artificial
// ESAT 2020-2021
//
// Test battery for queue ADT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "adt_logger.h"
#include "ABGS_MemoryManager\abgs_memory_manager.h"

#include ".\..\tests\test_base.c"

const u16 kCapacityQueue1 = 30;
const u16 kCapacityQueue2 = 5;
const u16 kCapacityQueue3 = 2;
int main()
{
	s16 error_type = 0;



	// vector created just to have a reference to the operations
	Logger *l = NULL;
	l = LOGGER_create(1);
	if (NULL == l) {
		printf("\n create returned a null node in vector for ops");
		return -1;
	}
	// vectors to work with
	Logger *logger_1 = NULL;
	logger_1 = LOGGER_create(kCapacityQueue1);
	if (NULL == logger_1) {
		printf("\n create returned a null node in logger_1\n");
		return -1;
	}

	printf("Size of:\n");
	printf("  + Mem Node: %zu\n", sizeof(MemoryNode));
	printf("  + logger_1: %zu\n", sizeof(*logger_1));


	MM->status();

  // Flush
  error_type = l->ops_->flush(logger_1);
  TESTBASE_printFunctionResult(logger_1, (u8 *)"Flush empty logger_1", error_type);
  // Destroy
  error_type = l->ops_->destroy(logger_1);
  TESTBASE_printFunctionResult(logger_1, (u8 *)"Destroy empty logger_1", error_type);

  logger_1 = LOGGER_create(kCapacityQueue1);
  if (NULL == logger_1) {
    printf("\n create returned a null node in logger_1\n");
    return -1;
  }
  for (int i = 0; i < 10; ++i) {
    printf("Inserccion : %d\n", i);
    error_type = l->ops_->write(logger_1, "HOLA\0");
    TESTBASE_printFunctionResult(logger_1, (u8 *)"Write in logger_1", error_type);
  }
  MM->status();
  error_type = l->ops_->flush(logger_1);
  TESTBASE_printFunctionResult(logger_1, (u8 *)"Flush logger_1", error_type);

	MM->status();

  logger_1->ops_->destroy(logger_1);
  l->ops_->destroy(l);

	MM->status();
	printf("Press ENTER to continue\n");
	getchar();
	MM->destroy();
	return 0;
}
