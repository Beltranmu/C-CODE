// test_list.c
// Escuela Superior de Arte y Tecnologia
// Algoritmos & Inteligencia Artificial
// ESAT 2020-2021
//
// Test battery for list ADT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "adt_list.h"
#include "test_base.cc"


const u16 kCapacityList1 = 30;
const u16 kCapacityList2 = 5;
const u16 kCapacityList3 = 2;

int main() {
	 // vector to work with
  List *list_1 = nullptr;
  List *list_2 = nullptr;
  List *list_3 = nullptr;
  int error_type = 0;
  //s16 error_type = 0;

  list_1 = new List();
  list_1->init(kCapacityList1);

  list_2 = new List();
  list_2->init(kCapacityList2);

  list_3 = new List();
  list_3->init(kCapacityList3);
  TESTBASE_generateDataForTest();


  //FIRST BATTERY
  printf("---------------- FIRST BATTERY ----------------\n\n");
  // reset
  printf("\n\n# Test Reset\n");
  error_type = list_1->reset();
  PrintFunctionError(list_1, (u8 *)"reset list_1", error_type);

  error_type = list_1->insertFirst(TestData.single_ptr_data_4, kSingleSizeData4);
  TestData.single_ptr_data_4 = nullptr;
  PrintFunctionError(list_1, (u8 *)"insertFirst list_1", error_type);

  error_type = list_1->reset();
  PrintFunctionError(list_1, (u8 *)"reset list_1", error_type);

  if (false == list_1->isEmpty())
  {
    printf("ERROR: isEmpty doesn't work correctly (list_1)\n");
  }

  printf("\n\n# Test Insert\n");
  printf("\t insertFirst list_1\n");
  for (u16 i = 0; i < (kNumberOfStoragePtrTest_A / 2); ++i)
  {
    error_type = list_1->insertFirst(TestData.storage_ptr_test_A[i], (strlen((char*)TestData.storage_ptr_test_A[i]) + 1));
    TestData.storage_ptr_test_A[i] = nullptr;
    PrintFunctionError(list_1, (u8 *)"insertFirst list_1", error_type);
  }
  printf("list_1:\n");
  list_1->print();

  printf("\t insertLast list_1\n");
  for (u16 i = (kNumberOfStoragePtrTest_A / 2); i < kNumberOfStoragePtrTest_A; ++i)
  {
    error_type = list_1->insertLast(TestData.storage_ptr_test_A[i], (strlen((char*)TestData.storage_ptr_test_A[i]) + 1));
    TestData.storage_ptr_test_A[i] = nullptr;
    PrintFunctionError(list_1, (u8 *)"insertLast list_1", error_type);
  }
  printf("list_1:\n");
  list_1->print();

  printf("\t insertAt list_1\n");
  u16 position = (kNumberOfStoragePtrTest_A / 2);
  for (u16 i = 0; i < kNumberOfStoragePtrTest_B; ++i)
  {
    printf("position :%d", position);
    error_type = list_1->insertAt(TestData.storage_ptr_test_B[i], (strlen((char*)TestData.storage_ptr_test_B[i]) + 1), position);
    TestData.storage_ptr_test_B[i] = nullptr;
    PrintFunctionError(list_1, (u8 *)"insertAt list_1", error_type);
    position++;
  }
  printf("list_1:\n");
  list_1->print();

  printf("\t list_2 insertAt\n");
  error_type = list_2->insertAt(TestData.single_ptr_data_1, kSingleSizeData1, 0);
  TestData.single_ptr_data_1 = nullptr;
  PrintFunctionError(list_2, (u8 *)"insertAt list_2", error_type);

  error_type = list_2->insertAt(TestData.single_ptr_data_1, kSingleSizeData1, 0);
  PrintFunctionError(list_2, (u8 *)"insertAt list_2 with NULL data", error_type);

  error_type = list_2->insertAt(TestData.single_ptr_data_2, kSingleSizeData2, 0);
  TestData.single_ptr_data_2 = nullptr;
  PrintFunctionError(list_2, (u8 *)"insertAt list_2", error_type);

  error_type = list_2->insertAt(TestData.single_ptr_data_3, kSingleSizeData3, 4);
  TestData.single_ptr_data_3 = nullptr;
  PrintFunctionError(list_2, (u8 *)"insertAt list_2", error_type);

  printf("list_2:\n");
  list_2->print();

  printf("\n\n# Test Insert\n");
  printf("\t insertLast list_3\n");
  u16 insert_errors = 0;
  for (u16 i = 0; i < kNumberOfStoragePtrTest_C; ++i)
  {
    if (false == list_3->isFull())
    {
      error_type = list_3->insertLast(TestData.storage_ptr_test_C[i], (strlen((char*)TestData.storage_ptr_test_C[i]) + 1));
      TestData.storage_ptr_test_C[i] = nullptr;
      PrintFunctionError(list_3, (u8 *)"insertLast list_3", error_type);
    }
    else
    {
      printf("ERROR: Trying to insert an element in a full vector (list_3 capacity = %d)\n", list_3->capacity());
      insert_errors++;
    }
  }
  if ((kNumberOfStoragePtrTest_C - kCapacityList3) != insert_errors)
  {
    printf("  ==> ERROR: isFull doesn't work correctly (list_3)\n");
  }
  printf("list_3:\n");
  list_3->print();

  printf("\n\n# Test Resize\n");
  printf("\t resize list_3\n");
  error_type = list_3->resize(10);
  PrintFunctionError(list_3, (u8 *)"resize list_3", error_type);
  printf("list_3:\n");
  list_3->print();

  for (u16 i = 2; i < kNumberOfStoragePtrTest_C; ++i)
  {
    error_type = list_3->insertLast(TestData.storage_ptr_test_C[i], (strlen((char*)TestData.storage_ptr_test_C[i]) + 1));
    TestData.storage_ptr_test_C[i] = nullptr;
    PrintFunctionError(list_3, (u8 *)"insertLast list_3", error_type);
  }
  printf("list_3:\n");
  list_3->print();

  printf("\n\n# Test Extract\n");
  printf("\t extractAt list_3\n");
  position = 2;
  TestData.storage_ptr_test_C[position] = list_3->extractAt(position);
  if (nullptr == TestData.storage_ptr_test_C[position])
    printf("ERROR: NULL pointer extracted at position %d in list_3\n", position);
  else
    printf("extracted \"%s\" at position %d in list_3\n", TestData.storage_ptr_test_C[position], position);
  position++;
  TestData.storage_ptr_test_C[position] = list_3->extractAt(2);
  if (nullptr == TestData.storage_ptr_test_C[position])
    printf("ERROR: NULL pointer extracted at position %d in list_3\n", 2);
  else
    printf("extracted \"%s\" at position %d in list_3\n", TestData.storage_ptr_test_C[position], 2);
  position++;
  TestData.storage_ptr_test_C[position] = list_3->extractAt(2);
  if (nullptr == TestData.storage_ptr_test_C[position])
    printf("ERROR: NULL pointer extracted at position %d in list_3\n", 2);
  else
    printf("extracted \"%s\" at position %d in list_3\n", TestData.storage_ptr_test_C[position], 2);

  printf("list_3:\n");
  list_3->print();

  printf("\t extractFirst list_3\n");
  for (u16 i = 0; i < 2; ++i)
  {
    TestData.storage_ptr_test_C[i] = list_3->extractFirst();
    if (nullptr == TestData.storage_ptr_test_C[i])
      printf("ERROR: NULL pointer extracted at first position\n");
    else
      printf("extracted \"%s\" at first position in list_3\n", TestData.storage_ptr_test_C[i]);
  }
  printf("list_3:\n");
  list_3->print();
  printf("\t extractLast list_3\n");
  for (u16 i = 6; i > 4; --i)
  {
    TestData.storage_ptr_test_C[i] = list_3->extractLast();
    if (nullptr == TestData.storage_ptr_test_C[i])
      printf("ERROR: NULL pointer extracted at Last position\n");
    else
      printf("extracted \"%s\" at last position in list_3\n", TestData.storage_ptr_test_C[i]);
  }
  printf("list_3:\n");
  list_3->print();

  void *data = nullptr;
  u16 extract_errors = 0;
  for (u16 i = 0; i < kNumberOfStoragePtrTest_C; ++i)
  {
    if (false == list_3->isEmpty())
    {
      data = list_3->extractLast();
      free(data);
      printf("ERROR: Trying to extract an element in an empty vector (list_3 length = %d)\n", list_3->length());
      extract_errors++;
    }
  }
  if (extract_errors > 0)
  {
    printf("  ==> ERROR: isEmpty doesn't work correctly (list_3)\n");
  }

  printf("\n\n# Test Insert\n");
  printf("\t insertLast list_3\n");
  for (u16 i = 0; i < kNumberOfStoragePtrTest_C; ++i)
  {
    error_type = list_3->insertLast(TestData.storage_ptr_test_C[i], (strlen((char*)TestData.storage_ptr_test_C[i]) + 1));
    TestData.storage_ptr_test_C[i] = nullptr;
    PrintFunctionError(list_3, (u8 *)"insertLast list_3", error_type);
  }
  printf("list_3:\n");
  list_3->print();

  printf("\n\n# Test First\n");
  printf("\t first list_1\n");
  data = list_1->first();
  if (nullptr == data)
    printf("First in list_1: NULL\n");
  else
    printf("First in list_1: \"%s\"\n", data);

  printf("\n\n# Test At\n");
  position = 1;
  printf("\t at list_3 (position %d)\n", position);
  data = list_3->at(1);
  if (nullptr == data)
    printf("At in list_3: NULL\n");
  else
    printf("At in list_3 (position %d): \"%s\"\n", position, data);

  printf("\n\n# Test Last\n");
  printf("\t last list_1\n");
  data = list_1->last();
  if (nullptr == data)
    printf("Last in list_1: NULL\n");
  else
    printf("Last in list_1: \"%s\"\n", data);

  printf("\n\n# Test Length\n");
  printf("\t list_1: [Capacity = %d] - [Length  = %d]\n", list_1->capacity(), list_1->length());
  printf("\t list_2: [Capacity = %d] - [Length  = %d]\n", list_2->capacity(), list_2->length());
  printf("\t list_3: [Capacity = %d] - [Length  = %d]\n", list_3->capacity(), list_3->length());

  printf("\n\n# Test Concat\n");
  printf("\t concat list_1 + list_3\n");
  error_type = list_1->concat(list_3);
  PrintFunctionError(list_1, (u8 *)"concat list_1 + list_3", error_type);
  printf("list_1:\n");
  list_1->print();
  printf("list_3:\n");
  list_3->print();

  printf("\n\n# Test Length\n");
  printf("\t Length list_1 = %d \n", list_1->length());
  printf("\t Length list_2 = %d \n", list_2->length());
  printf("\t Length list_3 = %d \n", list_3->length());

  printf("\n\n# Test Reset\n");
  error_type = list_3->reset();
  PrintFunctionError(list_3, (u8 *)"reset list_3", error_type);

  printf("\n\n# Test Length\n");
  printf("\t Length list_1 = %d \n", list_1->length());
  printf("\t Length list_2 = %d \n", list_2->length());
  printf("\t Length list_3 = %d \n", list_3->length());


  printf("\n\n# Test First\n");
  printf("\t first list_3\n");
  data = list_3->first();
  if (nullptr == data)
    printf("First in list_3: NULL\n");
  else
    printf("ERROR: First in list_3: \"%s\"\n", data);

  printf("\n\n# Test At\n");
  position = 1;
  printf("\t at list_3 (position %d)\n", position);
  data = list_3->at(1);
  if (nullptr == data)
    printf("At in list_3: NULL\n");
  else
    printf("ERROR: At in list_3 (position %d): \"%s\"\n", position, data);

  printf("\n\n# Test Last\n");
  printf("\t last list_3\n");
  data = list_3->last();
  if (nullptr == data)
    printf("Last in list_3: NULL\n");
  else
    printf("ERROR: Last in list_3: \"%s\"\n", data);


  printf("\n\n# Test Concat\n");
  printf("\t concat list_3 + list_2\n");
  error_type = list_3->concat(list_2);
  PrintFunctionError(list_3, (u8 *)"concat list_3 + list_2", error_type);
  printf("list_3:\n");
  list_3->print();

 

  printf("\n\n# Test Resize\n");
  printf("\t list_1 resize\n");
  error_type = list_1->resize(3);
  PrintFunctionError(list_1, (u8 *)"resize list_1", error_type);
  printf("list_1:\n");
  list_1->print();


  /*
  printf("\n\n---------------- NULL BATTERY ----------------\n\n");
  printf("\n\n# Test Create\n");
  list_2 = VECTOR_create(0);
  if (NULL != list_2) {
    printf("ERROR: trying to create list_2 with 0 capacity\n");
    return -1;
  }

  printf("\n\n# Test Insert\n");
  data = TestData.single_ptr_big_data;
  error_type = v->ops_->insertFirst(list_2, data, kSingleSizeBigData);
  PrintFunctionError(list_2, (u8 *)"insertFirst list_2 (NOT VALID)", error_type);
  error_type = v->ops_->insertLast(list_2, data, kSingleSizeBigData);
  PrintFunctionError(list_2, (u8 *)"insertLast list_2 (NOT VALID)", error_type);
  error_type = v->ops_->insertAt(list_2, data, kSingleSizeBigData, 1);
  PrintFunctionError(list_2, (u8 *)"insertAt list_2 (NOT VALID)", error_type);

  printf("\n\n# Test First\n");
  printf("\t first list_2\n");
  data = v->ops_->first(list_2);
  if (NULL != data)
    printf("ERROR: First in list_2 is not NULL\n");

  printf("\n\n# Test At\n");
  printf("\t at list_2 (position 1)\n");
  data = v->ops_->at(list_2, 1);
  if (NULL != data)
    printf("ERROR: At in list_2 is not NULL\n");

  printf("\n\n# Test Last\n");
  printf("\t last list_2\n");
  data = v->ops_->last(list_2);
  if (NULL != data)
    printf("ERROR: Last in list_2 is not NULL\n");


  printf("\n\n# Test Extract\n");
  data = v->ops_->extractFirst(list_2);
  if (NULL != data)
  {
    printf("ERROR: trying to extractFirst from an empty vector\n");
  }
  data = v->ops_->extractLast(list_2);
  if (NULL != data)
  {
    printf("ERROR: trying to extractLast from an empty vector\n");
  }
  data = v->ops_->extractAt(list_2, 1);
  if (NULL != data)
  {
    printf("ERROR: trying to extractAt from an empty vector\n");
  }

  printf("\n\n# Test Concat\n");
  printf("\t concat list_1 + list_2\n");

  error_type = v->ops_->concat(list_1, list_2);
  PrintFunctionError(list_1, (u8 *)"concat list_1 + list_2 (NOT VALID)", error_type);



  printf("\n\n# Test Resize\n");
  error_type = v->ops_->resize(list_2, 5);
  PrintFunctionError(list_2, (u8 *)"resize list_2 (NOT VALID)", error_type);

  printf("\n\n# Test Reset\n");
  error_type = v->ops_->reset(list_2);
  PrintFunctionError(list_2, (u8 *)"reset list_2 (NOT VALID)", error_type);


  printf("\n\n# Test Destroy\n");
  error_type = v->ops_->destroy(list_2);
  PrintFunctionError(list_2, (u8 *)"destroy list_2 (NOT VALID)", error_type);
  error_type = v->ops_->destroy(list_3);
  PrintFunctionError(list_3, (u8 *)"destroy list_3 (NOT VALID)", error_type);


  // Work is done, clean the system
  error_type = v->ops_->destroy(list_1);
  PrintFunctionError(list_1, (u8 *)"destroy list_1", error_type);
  error_type = v->ops_->destroy(v);
  PrintFunctionError(v, (u8 *)"destroy Vector Operations", error_type);

  printf("\nAt this point, it must be only one block of %d bytes waiting to be freed\n\n", kSingleSizeBigData);
  MM->status();
  TESTBASE_freeDataForTest();
  MM->status();*/


  printf("\n\n---------------- OPERATOR= COPY C ----------------\n\n");

  List *list_1c = nullptr;
  List *list_1i = new List();

  list_1c = new List(*list_1);
  *list_1i = *list_1;

  printf("List1 original\n\n");
  list_1->print();
  printf("list 1 copy constructor\n\n");
  list_1c->print();
  printf("List 1 operator =\n\n");
  list_1i->print();


  delete list_1;
  delete list_2;
  delete list_3;





  printf("Press ENTER to continue\n");
  getchar();
}
