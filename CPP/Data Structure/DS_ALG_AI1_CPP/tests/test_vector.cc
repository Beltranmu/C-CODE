// test_vector.c
// Escuela Superior de Arte y Tecnologia
// Algoritmos & Inteligencia Artificial
// ESAT 2020-2021
//
// Test battery for vector ADT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "adt_vector.h"
#include "test_base.cc"

const unsigned int kCapacityVector1 = 30;
const unsigned int kCapacityVector2 = 5;
const unsigned int kCapacityVector3 = 2;


int main() {

  // vector to work with
  Vector *vector_1 = nullptr;
  Vector *vector_2 = nullptr;
  Vector *vector_3 = nullptr;
  int error_type = 0;
	//s16 error_type = 0;

  vector_1 = new Vector();
  vector_1->init(kCapacityVector1);

  vector_2 = new Vector();
  vector_2->init(kCapacityVector2);

  vector_3 = new Vector();
  vector_3->init(kCapacityVector3);
	TESTBASE_generateDataForTest();


	//FIRST BATTERY
	printf("---------------- FIRST BATTERY ----------------\n\n");
	// reset
	printf("\n\n# Test Reset\n");
	error_type = vector_1->reset();
	PrintFunctionError(vector_1, (u8 *)"reset vector_1", error_type);

	error_type = vector_1->insertFirst(TestData.single_ptr_data_4, kSingleSizeData4);
	TestData.single_ptr_data_4 = nullptr;
	PrintFunctionError(vector_1, (u8 *)"insertFirst vector_1", error_type);

	error_type = vector_1->reset();
	PrintFunctionError(vector_1, (u8 *)"reset vector_1", error_type);

	if (false == vector_1->isEmpty())
	{
		printf("ERROR: isEmpty doesn't work correctly (vector_1)\n");
	}

	printf("\n\n# Test Insert\n");
	printf("\t insertFirst vector_1\n");
	for (u16 i = 0; i < (kNumberOfStoragePtrTest_A / 2); ++i)
	{
		error_type = vector_1->insertFirst(TestData.storage_ptr_test_A[i], (strlen((char*)TestData.storage_ptr_test_A[i]) + 1));
		TestData.storage_ptr_test_A[i] = nullptr;
		PrintFunctionError(vector_1, (u8 *)"insertFirst vector_1", error_type);
	}
	printf("vector_1:\n");
	vector_1->print();

	printf("\t insertLast vector_1\n");
	for (u16 i = (kNumberOfStoragePtrTest_A / 2); i < kNumberOfStoragePtrTest_A; ++i)
	{
		error_type = vector_1->insertLast(TestData.storage_ptr_test_A[i], (strlen((char*)TestData.storage_ptr_test_A[i]) + 1));
		TestData.storage_ptr_test_A[i] = nullptr;
		PrintFunctionError(vector_1, (u8 *)"insertLast vector_1", error_type);
	}
	printf("vector_1:\n");
	vector_1->print();

	printf("\t insertAt vector_1\n");
	u16 position = (kNumberOfStoragePtrTest_A / 2);
	for (u16 i = 0; i < kNumberOfStoragePtrTest_B; ++i)
	{
    printf("position :%d",position);
		error_type = vector_1->insertAt(TestData.storage_ptr_test_B[i], (strlen((char*)TestData.storage_ptr_test_B[i]) + 1), position);
		TestData.storage_ptr_test_B[i] = nullptr;
		PrintFunctionError(vector_1, (u8 *)"insertAt vector_1", error_type);
		position++;
	}
	printf("vector_1:\n");
	vector_1->print();

	printf("\t vector_2 insertAt\n");
	error_type = vector_2->insertAt(TestData.single_ptr_data_1, kSingleSizeData1, 0);
	TestData.single_ptr_data_1 = nullptr;
	PrintFunctionError(vector_2, (u8 *)"insertAt vector_2", error_type);

	error_type = vector_2->insertAt(TestData.single_ptr_data_1, kSingleSizeData1, 0);
	PrintFunctionError(vector_2, (u8 *)"insertAt vector_2 with NULL data", error_type);

	error_type = vector_2->insertAt(TestData.single_ptr_data_2, kSingleSizeData2, 0);
	TestData.single_ptr_data_2 = nullptr;
	PrintFunctionError(vector_2, (u8 *)"insertAt vector_2", error_type);

	error_type = vector_2->insertAt(TestData.single_ptr_data_3, kSingleSizeData3, 4);
	TestData.single_ptr_data_3 = nullptr;
	PrintFunctionError(vector_2, (u8 *)"insertAt vector_2", error_type);

	printf("vector_2:\n");
	vector_2->print();
  
	printf("\n\n# Test Insert\n");
	printf("\t insertLast vector_3\n");
	u16 insert_errors = 0;
	for (u16 i = 0; i < kNumberOfStoragePtrTest_C; ++i)
	{
		if (false == vector_3->isFull())
		{
			error_type =vector_3->insertLast(TestData.storage_ptr_test_C[i], (strlen((char*)TestData.storage_ptr_test_C[i]) + 1));
			TestData.storage_ptr_test_C[i] = nullptr;
			PrintFunctionError(vector_3, (u8 *)"insertLast vector_3", error_type);
		}
		else
		{
			printf("ERROR: Trying to insert an element in a full vector (vector_3 capacity = %d)\n", vector_3->capacity());
			insert_errors++;
		}
	}
	if ((kNumberOfStoragePtrTest_C - kCapacityVector3) != insert_errors)
	{
		printf("  ==> ERROR: isFull doesn't work correctly (vector_3)\n");
	}
	printf("vector_3:\n");
	vector_3->print();
  
	printf("\n\n# Test Resize\n");
	printf("\t resize vector_3\n");
	error_type = vector_3->resize(10);
	PrintFunctionError(vector_3, (u8 *)"resize vector_3", error_type);
	printf("vector_3:\n");
	vector_3->print();

	for (u16 i = 2; i < kNumberOfStoragePtrTest_C; ++i)
	{
		error_type = vector_3->insertLast(TestData.storage_ptr_test_C[i], (strlen((char*)TestData.storage_ptr_test_C[i]) + 1));
		TestData.storage_ptr_test_C[i] = nullptr;
		PrintFunctionError(vector_3, (u8 *)"insertLast vector_3", error_type);
	}
	printf("vector_3:\n");
	vector_3->print();
  
	printf("\n\n# Test Extract\n");
	printf("\t extractAt vector_3\n");
	position = 2;
	TestData.storage_ptr_test_C[position] = vector_3->extractAt(position);
	if (nullptr == TestData.storage_ptr_test_C[position])
		printf("ERROR: NULL pointer extracted at position %d in vector_3\n", position);
	else
		printf("extracted \"%s\" at position %d in vector_3\n", TestData.storage_ptr_test_C[position], position);
	position++;
	TestData.storage_ptr_test_C[position] = vector_3->extractAt(2);
	if (nullptr == TestData.storage_ptr_test_C[position])
		printf("ERROR: NULL pointer extracted at position %d in vector_3\n", 2);
	else
		printf("extracted \"%s\" at position %d in vector_3\n", TestData.storage_ptr_test_C[position], 2);
	position++;
	TestData.storage_ptr_test_C[position] = vector_3->extractAt(2);
	if (nullptr == TestData.storage_ptr_test_C[position])
		printf("ERROR: NULL pointer extracted at position %d in vector_3\n", 2);
	else
		printf("extracted \"%s\" at position %d in vector_3\n", TestData.storage_ptr_test_C[position], 2);

	printf("vector_3:\n");
	vector_3->print();

	printf("\t extractFirst vector_3\n");
	for (u16 i = 0; i < 2; ++i)
	{
		TestData.storage_ptr_test_C[i] = vector_3->extractFirst();
		if (nullptr == TestData.storage_ptr_test_C[i])
			printf("ERROR: NULL pointer extracted at first position\n");
		else
			printf("extracted \"%s\" at first position in vector_3\n", TestData.storage_ptr_test_C[i]);
	}
	printf("vector_3:\n");
	vector_3->print();
	printf("\t extractLast vector_3\n");
	for (u16 i = 6; i > 4; --i)
	{
		TestData.storage_ptr_test_C[i] = vector_3->extractLast();
		if (nullptr == TestData.storage_ptr_test_C[i])
			printf("ERROR: NULL pointer extracted at Last position\n");
		else
			printf("extracted \"%s\" at last position in vector_3\n", TestData.storage_ptr_test_C[i]);
	}
	printf("vector_3:\n");
	vector_3->print();
 
	void *data = nullptr;
	u16 extract_errors = 0;
	for (u16 i = 0; i < kNumberOfStoragePtrTest_C; ++i)
	{
		if (false == vector_3->isEmpty())
		{
			data = vector_3->extractLast();
			free(data);
			printf("ERROR: Trying to extract an element in an empty vector (vector_3 length = %d)\n", vector_3->length());
			extract_errors++;
		}
	}
	if (extract_errors > 0)
	{
		printf("  ==> ERROR: isEmpty doesn't work correctly (vector_3)\n");
	}

	printf("\n\n# Test Insert\n");
	printf("\t insertLast vector_3\n");
	for (u16 i = 0; i < kNumberOfStoragePtrTest_C; ++i)
	{
		error_type = vector_3->insertLast(TestData.storage_ptr_test_C[i], (strlen((char*)TestData.storage_ptr_test_C[i]) + 1));
		TestData.storage_ptr_test_C[i] = nullptr;
		PrintFunctionError(vector_3, (u8 *)"insertLast vector_3", error_type);
	}
	printf("vector_3:\n");
	vector_3->print();

	printf("\n\n# Test First\n");
	printf("\t first vector_1\n");
	data = vector_1->first();
	if (nullptr == data)
		printf("First in vector_1: NULL\n");
	else
		printf("First in vector_1: \"%s\"\n", data);

	printf("\n\n# Test At\n");
	position = 1;
	printf("\t at vector_3 (position %d)\n", position);
	data = vector_3->at(1);
	if (nullptr == data)
		printf("At in vector_3: NULL\n");
	else
		printf("At in vector_3 (position %d): \"%s\"\n", position, data);

	printf("\n\n# Test Last\n");
	printf("\t last vector_1\n");
  data = vector_1->last();
	if (nullptr == data)
		printf("Last in vector_1: NULL\n");
	else
		printf("Last in vector_1: \"%s\"\n", data);

	printf("\n\n# Test Length\n");
	printf("\t vector_1: [Capacity = %d] - [Length  = %d]\n", vector_1->capacity(), vector_1->length());
	printf("\t vector_2: [Capacity = %d] - [Length  = %d]\n", vector_2->capacity(), vector_2->length());
	printf("\t vector_3: [Capacity = %d] - [Length  = %d]\n", vector_3->capacity(), vector_3->length());

	printf("\n\n# Test Concat\n");
	printf("\t concat vector_1 + vector_3\n");
	error_type = vector_1->concat(vector_3);
	PrintFunctionError(vector_1, (u8 *)"concat vector_1 + vector_3", error_type);
	printf("vector_1:\n");
	vector_1->print();
	printf("vector_3:\n");
	vector_3->print();
  
	printf("\n\n# Test Length\n");
	printf("\t Length vector_1 = %d \n", vector_1->length());
  printf("\t Length vector_2 = %d \n", vector_2->length());
  printf("\t Length vector_3 = %d \n", vector_3->length());
  
	printf("\n\n# Test Reset\n");
	error_type = vector_3->reset();
	PrintFunctionError(vector_3, (u8 *)"reset vector_3", error_type);

	printf("\n\n# Test Length\n");
	printf("\t Length vector_1 = %d \n", vector_1->length());
	printf("\t Length vector_2 = %d \n", vector_2->length());
	printf("\t Length vector_3 = %d \n", vector_3->length());


	printf("\n\n# Test First\n");
	printf("\t first vector_3\n");
	data = vector_3->first();
	if (nullptr == data)
		printf("First in vector_3: NULL\n");
	else
		printf("ERROR: First in vector_3: \"%s\"\n", data);

	printf("\n\n# Test At\n");
	position = 1;
	printf("\t at vector_3 (position %d)\n", position);
	data = vector_3->at(1);
	if (nullptr == data)
		printf("At in vector_3: NULL\n");
	else
		printf("ERROR: At in vector_3 (position %d): \"%s\"\n", position, data);

	printf("\n\n# Test Last\n");
	printf("\t last vector_3\n");
	data = vector_3->last();
	if (nullptr == data)
		printf("Last in vector_3: NULL\n");
	else
		printf("ERROR: Last in vector_3: \"%s\"\n", data);


	printf("\n\n# Test Concat\n");
	printf("\t concat vector_3 + vector_2\n");
	error_type = vector_3->concat(vector_2);
	PrintFunctionError(vector_3, (u8 *)"concat vector_3 + vector_2", error_type);
	printf("vector_3:\n");
  vector_3->print();

	printf("\n\n# Test Destroy\n");
	printf("\t vector_2 Destroy\n");
	//error_type = vector_2->destroy();
	vector_2 = nullptr;
	PrintFunctionError(vector_2, (u8 *)"destroy vector_2", error_type);
	printf("vector_2:\n");
  vector_2->print();

	//printf("\t vector_3 Destroy\n");
	//error_type = vector_3->destroy();
	vector_3 = nullptr;
	PrintFunctionError(vector_3, (u8 *)"destroy vector_3", error_type);
	printf("vector_3:\n");
  vector_3->print();
  
	printf("\n\n# Test Resize\n");
	printf("\t vector_1 resize\n");
	error_type = vector_1->resize(3);
	PrintFunctionError(vector_1, (u8 *)"resize vector_1", error_type);
	printf("vector_1:\n");
  vector_1->print();
  

  /*
	printf("\n\n---------------- NULL BATTERY ----------------\n\n");
	printf("\n\n# Test Create\n");
	vector_2 = VECTOR_create(0);
	if (NULL != vector_2) {
		printf("ERROR: trying to create vector_2 with 0 capacity\n");
		return -1;
	}

	printf("\n\n# Test Insert\n");
	data = TestData.single_ptr_big_data;
	error_type = v->ops_->insertFirst(vector_2, data, kSingleSizeBigData);
	PrintFunctionError(vector_2, (u8 *)"insertFirst vector_2 (NOT VALID)", error_type);
	error_type = v->ops_->insertLast(vector_2, data, kSingleSizeBigData);
	PrintFunctionError(vector_2, (u8 *)"insertLast vector_2 (NOT VALID)", error_type);
	error_type = v->ops_->insertAt(vector_2, data, kSingleSizeBigData, 1);
	PrintFunctionError(vector_2, (u8 *)"insertAt vector_2 (NOT VALID)", error_type);

	printf("\n\n# Test First\n");
	printf("\t first vector_2\n");
	data = v->ops_->first(vector_2);
	if (NULL != data)
		printf("ERROR: First in vector_2 is not NULL\n");

	printf("\n\n# Test At\n");
	printf("\t at vector_2 (position 1)\n");
	data = v->ops_->at(vector_2, 1);
	if (NULL != data)
		printf("ERROR: At in vector_2 is not NULL\n");

	printf("\n\n# Test Last\n");
	printf("\t last vector_2\n");
	data = v->ops_->last(vector_2);
	if (NULL != data)
		printf("ERROR: Last in vector_2 is not NULL\n");

    
	printf("\n\n# Test Extract\n");
	data = v->ops_->extractFirst(vector_2);
	if (NULL != data)
	{
		printf("ERROR: trying to extractFirst from an empty vector\n");
	}
	data = v->ops_->extractLast(vector_2);
	if (NULL != data)
	{
		printf("ERROR: trying to extractLast from an empty vector\n");
	}
	data = v->ops_->extractAt(vector_2, 1);
	if (NULL != data)
	{
		printf("ERROR: trying to extractAt from an empty vector\n");
	}
  
	printf("\n\n# Test Concat\n");
	printf("\t concat vector_1 + vector_2\n");
  
  error_type = v->ops_->concat(vector_1, vector_2);
	PrintFunctionError(vector_1, (u8 *)"concat vector_1 + vector_2 (NOT VALID)", error_type);
  
  
  
	printf("\n\n# Test Resize\n");
	error_type = v->ops_->resize(vector_2, 5);
	PrintFunctionError(vector_2, (u8 *)"resize vector_2 (NOT VALID)", error_type);

	printf("\n\n# Test Reset\n");
	error_type = v->ops_->reset(vector_2);
	PrintFunctionError(vector_2, (u8 *)"reset vector_2 (NOT VALID)", error_type);


	printf("\n\n# Test Destroy\n");
	error_type = v->ops_->destroy(vector_2);
	PrintFunctionError(vector_2, (u8 *)"destroy vector_2 (NOT VALID)", error_type);
	error_type = v->ops_->destroy(vector_3);
	PrintFunctionError(vector_3, (u8 *)"destroy vector_3 (NOT VALID)", error_type);


	// Work is done, clean the system
	error_type = v->ops_->destroy(vector_1);
	PrintFunctionError(vector_1, (u8 *)"destroy vector_1", error_type);
	error_type = v->ops_->destroy(v);
	PrintFunctionError(v, (u8 *)"destroy Vector Operations", error_type);

	printf("\nAt this point, it must be only one block of %d bytes waiting to be freed\n\n", kSingleSizeBigData);
	MM->status();
	TESTBASE_freeDataForTest();
	MM->status();*/

  printf("\n\n---------------- OPERATOR= COPY C ----------------\n\n");

  Vector *vector_1c = nullptr;
  Vector *vector_1i = new Vector();

  vector_1c = new Vector(*vector_1);
  *vector_1i = *vector_1;

  printf("Vecto1 original\n\n");
  vector_1->print();
  printf("Vecto1 copy constructor\n\n");
  vector_1c->print();
  printf("Vecto1 operator =\n\n");
  vector_1i->print();

 


  delete vector_1;
  delete vector_2;
  delete vector_3;
	printf("Press ENTER to continue\n");
	getchar();

	return 0;
}
