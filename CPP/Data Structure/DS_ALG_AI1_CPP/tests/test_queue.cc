// test_queue.c
// Escuela Superior de Arte y Tecnologia
// Algoritmos & Inteligencia Artificial
// ESAT 2020-2021
//
// Test battery for queue ADT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "adt_queue.h"
#include "test_base.cc"

const u16 kCapacityQueue1 = 30;
const u16 kCapacityQueue2 = 5;
const u16 kCapacityQueue3 = 2;
int main()
{
	s16 error_type = 0;

	TESTBASE_generateDataForTest();


	// Queues to work with
	Queue *queue_1 = nullptr;
	Queue *queue_2 = nullptr;
	Queue *queue_3 = nullptr;
  queue_1 = new Queue();
  queue_1->init(kCapacityQueue1);
	if (nullptr == queue_1) {
		printf("\n create returned a null node in queue_1\n");
		return -1;
	}
  queue_2 = new Queue();
  queue_2->init(kCapacityQueue2);
	if (nullptr == queue_2) {
		printf("\n create returned a null node in queue_2\n");
		return -1;
	}
  queue_3 = new Queue();
  queue_3->init(kCapacityQueue3);
	if (nullptr == queue_3) {
		printf("\n create returned a null node in queue_3\n");
		return -1;
	}

	printf("Size of:\n");
	printf("  + Mem Node: %zu\n", sizeof(MemoryNode));
	printf("  + queue_1: %zu\n", sizeof(*queue_1));
	printf("  + queue_2: %zu\n", sizeof(*queue_2));
	printf("  + queue_3: %zu\n", sizeof(*queue_3));


	//FIRST BATTERY
	printf("---------------- FIRST BATTERY ----------------\n\n");
	// reset
	printf("\n\n# Test Reset\n");
	error_type = queue_1->reset();
	PrintFunctionError(queue_1, (u8 *)"reset queue_1", error_type);

	error_type = queue_1->enqueue(TestData.single_ptr_data_4, kSingleSizeData4);
	TestData.single_ptr_data_4 = nullptr;
	PrintFunctionError(queue_1, (u8 *)"enqueue queue_1", error_type);

	error_type = queue_1->reset();
	PrintFunctionError(queue_1, (u8 *)"reset queue_1", error_type);

	if (false == queue_1->isEmpty())
	{
		printf("ERROR: isEmpty doesn't work correctly (queue_1)\n");
	}

	printf("\n\n# Test Enqueue\n");
	printf("\t enqueue queue_1\n");
	for (u16 i = 0; i < kNumberOfStoragePtrTest_A; ++i)
	{
		error_type = queue_1->enqueue(TestData.storage_ptr_test_A[i], (strlen((char*)TestData.storage_ptr_test_A[i]) + 1));
		TestData.storage_ptr_test_A[i] = nullptr;
		PrintFunctionError(queue_1, (u8 *)"enqueue queue_1", error_type);
	}
	printf("queue_1:\n");
  queue_1->print();

	printf("\t enqueue queue_1\n");
	for (u16 i = 0; i < kNumberOfStoragePtrTest_B; ++i)
	{
		error_type = queue_1->enqueue(TestData.storage_ptr_test_B[i], (strlen((char*)TestData.storage_ptr_test_B[i]) + 1));
		TestData.storage_ptr_test_B[i] = nullptr;
		PrintFunctionError(queue_1, (u8 *)"enqueue queue_1", error_type);
	}
	printf("queue_1:\n");
  queue_1->print();

	printf("\t queue_2 enqueue\n");
	error_type = queue_1->enqueue(TestData.single_ptr_data_1, kSingleSizeData1);
	TestData.single_ptr_data_1 = nullptr;
	PrintFunctionError(queue_2, (u8 *)"enqueue queue_2", error_type);

	error_type = queue_2->enqueue(TestData.single_ptr_data_1, kSingleSizeData1);
	PrintFunctionError(queue_2, (u8 *)"enqueue queue_2 with NULL data", error_type);

	error_type = queue_2->enqueue(TestData.single_ptr_data_2, kSingleSizeData2);
	TestData.single_ptr_data_2 = nullptr;
	PrintFunctionError(queue_2, (u8 *)"enqueue queue_2", error_type);

	error_type = queue_2->enqueue(TestData.single_ptr_data_3, kSingleSizeData3);
	TestData.single_ptr_data_3 = NULL;
	PrintFunctionError(queue_2, (u8 *)"enqueue queue_2", error_type);

	printf("queue_2:\n");
  queue_2->print();

	printf("\t enqueue queue_3\n");
	u16 insert_errors = 0;
	for (u16 i = 0; i < kNumberOfStoragePtrTest_C; ++i)
	{
		if (false == queue_3->isFull())
		{
			error_type = queue_3->enqueue(
        TestData.storage_ptr_test_C[i], (strlen((char*)TestData.storage_ptr_test_C[i]) + 1));
			TestData.storage_ptr_test_C[i] = nullptr;
			PrintFunctionError(queue_3, (u8 *)"enqueue queue_3", error_type);
		}
		else
		{
			printf("ERROR: Trying to insert an element in a full vector (queue_3 capacity = %d)\n", queue_3->capacity());
			insert_errors++;
		}
	}
	if ((kNumberOfStoragePtrTest_C - kCapacityQueue3) != insert_errors)
	{
		printf("  ==> ERROR: isFull doesn't work correctly (queue_3)\n");
	}
	printf("queue_3:\n");
  queue_3->print();

	printf("\n\n# Test Resize\n");
	printf("\t resize queue_3\n");
	error_type = queue_3->resize(10);
	PrintFunctionError(queue_3, (u8 *)"resize queue_3", error_type);
	printf("queue_3:\n");
  queue_3->print();

	for (u16 i = 2; i < kNumberOfStoragePtrTest_C; ++i)
	{
		error_type = queue_3->enqueue(TestData.storage_ptr_test_C[i], (strlen((char*)TestData.storage_ptr_test_C[i]) + 1));
		TestData.storage_ptr_test_C[i] = nullptr;
		PrintFunctionError(queue_3, (u8 *)"enqueue queue_3", error_type);
	}
	printf("queue_3:\n");
  queue_3->print();

	printf("\n\n# Test Pop\n");
	printf("\t dequeue queue_3\n");
	u16 position = 2;
	TestData.storage_ptr_test_C[position] = queue_3->dequeue();
	if (nullptr == TestData.storage_ptr_test_C[position])
		printf("ERROR: NULL pointer extracted at front queue_3\n");
	else
		printf("extracted \"%s\" at front in queue_3\n", TestData.storage_ptr_test_C[position]);
	position++;
	TestData.storage_ptr_test_C[position] = queue_3->dequeue();
	if (nullptr == TestData.storage_ptr_test_C[position])
		printf("ERROR: NULL pointer extracted at front queue_3\n");
	else
		printf("extracted \"%s\" at front in queue_3\n", TestData.storage_ptr_test_C[position]);
	position++;
	TestData.storage_ptr_test_C[position] = queue_3->dequeue();
	if (nullptr == TestData.storage_ptr_test_C[position])
		printf("ERROR: NULL pointer extracted at front queue_3\n");
	else
		printf("extracted \"%s\" at front in queue_3\n", TestData.storage_ptr_test_C[position]);

	printf("queue_3:\n");
  queue_3->print();

	printf("\t dequeue queue_3\n");
	for (u16 i = 0; i < 2; ++i)
	{
		TestData.storage_ptr_test_C[i] = queue_3->dequeue();
		if (nullptr == TestData.storage_ptr_test_C[i])
			printf("ERROR: NULL pointer extracted at front position\n");
		else
			printf("extracted \"%s\" at front position in queue_3\n", TestData.storage_ptr_test_C[i]);
	}
	printf("queue_3:\n");
  queue_3->print();
	printf("\t dequeue queue_3\n");
	for (u16 i = 6; i > 4; --i)
	{
		TestData.storage_ptr_test_C[i] = queue_3->dequeue();
		if (nullptr  == TestData.storage_ptr_test_C[i])
			printf("ERROR: NULL pointer extracted at front position\n");
		else
			printf("extracted \"%s\" at front position in queue_3\n", TestData.storage_ptr_test_C[i]);
	}
	printf("queue_3:\n");
  queue_3->print();

	void *data = nullptr;
	u16 extract_errors = 0;
	for (u16 i = 0; i < kNumberOfStoragePtrTest_C; ++i)
	{
		if (false == queue_3->isEmpty())
		{
			data = queue_3->dequeue();
			//MM->free(data);
			printf("ERROR: Trying to extract an element in an empty stack (queue_3 length = %d)\n", queue_3->length());
			extract_errors++;
		}
	}
	if (extract_errors > 0)
	{
		printf("  ==> ERROR: isEmpty doesn't work correctly (queue_3)\n");
	}

	printf("\n\n# Test Push\n");
	printf("\t enqueue queue_3\n");
	for (u16 i = 0; i < kNumberOfStoragePtrTest_C; ++i)
	{
		error_type = queue_3->enqueue(TestData.storage_ptr_test_C[i], (strlen((char*)TestData.storage_ptr_test_C[i]) + 1));
		TestData.storage_ptr_test_C[i] = nullptr;
		PrintFunctionError(queue_3, (u8 *)"enqueue queue_3", error_type);
	}
	printf("queue_3:\n");
  queue_3->print();

	printf("\n\n# Test Front\n");
	printf("\t front queue_1\n");
	data = queue_1->front();
	if (nullptr == data)
		printf("Front in queue_1: NULL\n");
	else
		printf("Front in queue_1: \"%s\"\n", data);

	printf("\n\n# Test Back\n");
	printf("\t back queue_1\n");
	data = queue_1->back();
	if (nullptr == data)
		printf("Back in queue_1: NULL\n");
	else
		printf("Back in queue_1: \"%s\"\n", data);

	printf("\n\n# Test Length\n");
	printf("\t queue_1: [Capacity = %d] - [Length  = %d]\n", queue_1->capacity(),queue_1->length());
	printf("\t queue_2: [Capacity = %d] - [Length  = %d]\n", queue_2->capacity(), queue_2->length());
	printf("\t queue_3: [Capacity = %d] - [Length  = %d]\n", queue_3->capacity(), queue_3->length());

	printf("\n\n# Test Concat\n");
	printf("\t concat queue_1 + queue_3\n");
	error_type = queue_1->concat(queue_3);
	PrintFunctionError(queue_1, (u8 *)"concat queue_1 + queue_3", error_type);
	printf("queue_1:\n");
  queue_1->print();
	printf("queue_3:\n");
  queue_3->print();

	printf("\n\n# Test Length\n");
	printf("\t Length queue_1 = %d \n", queue_1->length());
	printf("\t Length queue_2 = %d \n", queue_2->length());
	printf("\t Length queue_3 = %d \n", queue_3->length());

	printf("\n\n# Test Reset\n");
	error_type = queue_3->reset();
	PrintFunctionError(queue_3, (u8 *)"reset queue_3", error_type);

	printf("\n\n# Test Length\n");
	printf("\t Length queue_1 = %d \n", queue_1->length());
	printf("\t Length queue_2 = %d \n", queue_2->length());
	printf("\t Length queue_3 = %d \n", queue_3->length());

	printf("\n\n# Test Front\n");
	printf("\t front queue_3\n");
	data = queue_3->front();
	if (nullptr == data)
		printf("Front in queue_3: NULL\n");
	else
		printf("ERROR: First in queue_3: \"%s\"\n", data);

	printf("\n\n# Test Back\n");
	printf("\t back queue_3\n");
	data = queue_3->back();
	if (nullptr == data)
		printf("Back in queue_3: NULL\n");
	else
		printf("Back in queue_3: \"%s\"\n", data);

	printf("\n\n# Test Concat\n");
	printf("\t concat queue_3 + queue_2\n");
	error_type = queue_3->concat(queue_2);
	PrintFunctionError(queue_3, (u8 *)"concat queue_3 + queue_2", error_type);
	printf("queue_3:\n");
  queue_3->print();

	printf("\n\n# Test Resize\n");
	printf("\t queue_1 resize\n");
	error_type = queue_1->resize(3);
	PrintFunctionError(queue_1, (u8 *)"resize queue_1", error_type);
	printf("queue_1:\n");
  queue_1->print();

 


  
 


	//MM->status();

	/*printf("\n\n---------------- NULL BATTERY ----------------\n\n");
	printf("\n\n# Test Insert\n");
	data = TestData.single_ptr_big_data;
	error_type = q->ops_->enqueue(queue_2, data, kSingleSizeBigData);
	PrintFunctionError(queue_2, (u8 *)"enqueue queue_2 (NOT VALID)", error_type);

	printf("\n\n# Test Front\n");
	printf("\t front queue_2\n");
	data = q->ops_->front(queue_2);
	if (nullptr != data)
		printf("ERROR: Front in queue_2 is not NULL\n");

	printf("\n\n# Test Back\n");
	printf("\t back queue_2\n");
	data = q->ops_->back(queue_2);
	if (nullptr == data)
		printf("Back in queue_2: NULL\n");
	else
		printf("Back in queue_2: \"%s\"\n", data);

	printf("\n\n# Test Extract\n");
	data = q->ops_->dequeue(queue_2, nullptr);
	if (nullptr != data)
	{
		printf("ERROR: trying to dequeue from an empty stack\n");
	}

	printf("\n\n# Test Concat\n");
	printf("\t concat queue_1 + queue_2\n");
	error_type = q->ops_->concat(queue_1, queue_2);
	PrintFunctionError(queue_1, (u8 *)"concat queue_1 + queue_2 (NOT VALID)", error_type);

	printf("\n\n# Test Resize\n");
	error_type = q->ops_->resize(queue_2, 5);
	PrintFunctionError(queue_2, (u8 *)"resize queue_2 (NOT VALID)", error_type);

	printf("\n\n# Test Reset\n");
	error_type = q->ops_->reset(&queue_2);
	PrintFunctionError(queue_2, (u8 *)"reset queue_2 (NOT VALID)", error_type);


	printf("\n\n# Test Destroy\n");
	error_type = q->ops_->destroy(queue_2);
	PrintFunctionError(queue_2, (u8 *)"destroy queue_2 (NOT VALID)", error_type);
	error_type = q->ops_->destroy(queue_3);
	PrintFunctionError(queue_3, (u8 *)"destroy queue_3 (NOT VALID)", error_type);


	// Work is done, clean the system
	error_type = q->ops_->destroy(queue_1);
	PrintFunctionError(queue_1, (u8 *)"destroy queue_1", error_type);
	error_type = q->ops_->destroy(q);
	PrintFunctionError(q, (u8 *)"destroy Queue Operations", error_type);

	printf("\nAt this point, it must be only one block of %d bytes waiting to be freed\n\n", kSingleSizeBigData);
	MM->status();
	TESTBASE_freeDataForTest();
	MM->status();*/

  printf("\n\n---------------- OPERATOR= COPY C ----------------\n\n");

  Queue *queue_1c = nullptr;
  Queue *queue_1i = new Queue();

  queue_1c = new Queue(*queue_1);
  *queue_1i = *queue_1;
  printf("Queue original\n\n");
  queue_1->print();
  printf("Queue copy constructor\n\n");
  queue_1c->print();
  printf("Queue operator = \n\n");
  queue_1i->print();


  delete queue_1;
  delete queue_2;
  delete queue_3;
  printf("Press ENTER to continue\n");
	getchar();
	
	return 0;
}
