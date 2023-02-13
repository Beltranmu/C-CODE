// test_list.c
// Escuela Superior de Arte y Tecnologia
// Algoritmos & Inteligencia Artificial
// ESAT 2020-2021
//
// Test battery for list ADT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "adt_dl_list.h"
#include "test_base.cc"


const u16 kCapacityDLList1 = 30;
const u16 kCapacityDLList2 = 5;
const u16 kCapacityDLList3 = 2;

int main() {
  // vector to work with
  DL_List *dl_list_1 = nullptr;
  DL_List *dl_list_2 = nullptr;
  DL_List *dl_list_3 = nullptr;
  int error_type = 0;
  //s16 error_type = 0;

  dl_list_1 = new DL_List();
  dl_list_1->init(kCapacityDLList1);

  dl_list_2 = new DL_List();
  dl_list_2->init(kCapacityDLList2);

  dl_list_3 = new DL_List();
  dl_list_3->init(kCapacityDLList3);
  TESTBASE_generateDataForTest();


  //FIRST BATTERY
  printf("---------------- FIRST BATTERY ----------------\n\n");
  // reset
  printf("\n\n# Test Reset\n");
  error_type = dl_list_1->reset();
  PrintFunctionError(dl_list_1, (u8 *)"reset dl_list_1", error_type);

  error_type = dl_list_1->insertFirst(TestData.single_ptr_data_4, kSingleSizeData4);
  TestData.single_ptr_data_4 = nullptr;
  PrintFunctionError(dl_list_1, (u8 *)"insertFirst dl_list_1", error_type);

  error_type = dl_list_1->reset();
  PrintFunctionError(dl_list_1, (u8 *)"reset dl_list_1", error_type);

  if (false == dl_list_1->isEmpty())
  {
    printf("ERROR: isEmpty doesn't work correctly (dl_list_1)\n");
  }

  printf("\n\n# Test Insert\n");
  printf("\t insertFirst dl_list_1\n");
  for (u16 i = 0; i < (kNumberOfStoragePtrTest_A / 2); ++i)
  {
    error_type = dl_list_1->insertFirst(TestData.storage_ptr_test_A[i], (strlen((char*)TestData.storage_ptr_test_A[i]) + 1));
    TestData.storage_ptr_test_A[i] = nullptr;
    PrintFunctionError(dl_list_1, (u8 *)"insertFirst dl_list_1", error_type);
  }
  printf("dl_list_1:\n");
  dl_list_1->print();

  printf("\t insertLast dl_list_1\n");
  for (u16 i = (kNumberOfStoragePtrTest_A / 2); i < kNumberOfStoragePtrTest_A; ++i)
  {
    error_type = dl_list_1->insertLast(TestData.storage_ptr_test_A[i], (strlen((char*)TestData.storage_ptr_test_A[i]) + 1));
    TestData.storage_ptr_test_A[i] = nullptr;
    PrintFunctionError(dl_list_1, (u8 *)"insertLast dl_list_1", error_type);
  }
  printf("dl_list_1:\n");
  dl_list_1->print();

  printf("\t insertAt dl_list_1\n");
  u16 position = (kNumberOfStoragePtrTest_A / 2);
  for (u16 i = 0; i < kNumberOfStoragePtrTest_B; ++i)
  {
    printf("position :%d", position);
    error_type = dl_list_1->insertAt(TestData.storage_ptr_test_B[i], (strlen((char*)TestData.storage_ptr_test_B[i]) + 1), position);
    TestData.storage_ptr_test_B[i] = nullptr;
    PrintFunctionError(dl_list_1, (u8 *)"insertAt dl_list_1", error_type);
    position++;
  }
  printf("dl_list_1:\n");
  dl_list_1->print();

  printf("\t dl_list_2 insertAt\n");
  error_type = dl_list_2->insertAt(TestData.single_ptr_data_1, kSingleSizeData1, 0);
  TestData.single_ptr_data_1 = nullptr;
  PrintFunctionError(dl_list_2, (u8 *)"insertAt dl_list_2", error_type);

  error_type = dl_list_2->insertAt(TestData.single_ptr_data_1, kSingleSizeData1, 0);
  PrintFunctionError(dl_list_2, (u8 *)"insertAt dl_list_2 with NULL data", error_type);

  error_type = dl_list_2->insertAt(TestData.single_ptr_data_2, kSingleSizeData2, 0);
  TestData.single_ptr_data_2 = nullptr;
  PrintFunctionError(dl_list_2, (u8 *)"insertAt dl_list_2", error_type);

  error_type = dl_list_2->insertAt(TestData.single_ptr_data_3, kSingleSizeData3, 4);
  TestData.single_ptr_data_3 = nullptr;
  PrintFunctionError(dl_list_2, (u8 *)"insertAt dl_list_2", error_type);

  printf("dl_list_2:\n");
  dl_list_2->print();

  printf("\n\n# Test Insert\n");
  printf("\t insertLast dl_list_3\n");
  u16 insert_errors = 0;
  for (u16 i = 0; i < kNumberOfStoragePtrTest_C; ++i)
  {
    if (false == dl_list_3->isFull())
    {
      error_type = dl_list_3->insertLast(TestData.storage_ptr_test_C[i], (strlen((char*)TestData.storage_ptr_test_C[i]) + 1));
      TestData.storage_ptr_test_C[i] = nullptr;
      PrintFunctionError(dl_list_3, (u8 *)"insertLast dl_list_3", error_type);
    }
    else
    {
      printf("ERROR: Trying to insert an element in a full vector (dl_list_3 capacity = %d)\n", dl_list_3->capacity());
      insert_errors++;
    }
  }
  if ((kNumberOfStoragePtrTest_C - kCapacityDLList3) != insert_errors)
  {
    printf("  ==> ERROR: isFull doesn't work correctly (dl_list_3)\n");
  }
  printf("dl_list_3:\n");
  dl_list_3->print();

  printf("\n\n# Test Resize\n");
  printf("\t resize dl_list_3\n");
  error_type = dl_list_3->resize(10);
  PrintFunctionError(dl_list_3, (u8 *)"resize dl_list_3", error_type);
  printf("dl_list_3:\n");
  dl_list_3->print();

  for (u16 i = 2; i < kNumberOfStoragePtrTest_C; ++i)
  {
    error_type = dl_list_3->insertLast(TestData.storage_ptr_test_C[i], (strlen((char*)TestData.storage_ptr_test_C[i]) + 1));
    TestData.storage_ptr_test_C[i] = nullptr;
    PrintFunctionError(dl_list_3, (u8 *)"insertLast dl_list_3", error_type);
  }
  printf("dl_list_3:\n");
  dl_list_3->print();

  printf("\n\n# Test Extract\n");
  printf("\t extractAt dl_list_3\n");
  position = 2;
  TestData.storage_ptr_test_C[position] = dl_list_3->extractAt(position);
  if (nullptr == TestData.storage_ptr_test_C[position])
    printf("ERROR: NULL pointer extracted at position %d in dl_list_3\n", position);
  else
    printf("extracted \"%s\" at position %d in dl_list_3\n", TestData.storage_ptr_test_C[position], position);
  position++;
  TestData.storage_ptr_test_C[position] = dl_list_3->extractAt(2);
  if (nullptr == TestData.storage_ptr_test_C[position])
    printf("ERROR: NULL pointer extracted at position %d in dl_list_3\n", 2);
  else
    printf("extracted \"%s\" at position %d in dl_list_3\n", TestData.storage_ptr_test_C[position], 2);
  position++;
  TestData.storage_ptr_test_C[position] = dl_list_3->extractAt(2);
  if (nullptr == TestData.storage_ptr_test_C[position])
    printf("ERROR: NULL pointer extracted at position %d in dl_list_3\n", 2);
  else
    printf("extracted \"%s\" at position %d in dl_list_3\n", TestData.storage_ptr_test_C[position], 2);

  printf("dl_list_3:\n");
  dl_list_3->print();

  printf("\t extractFirst dl_list_3\n");
  for (u16 i = 0; i < 2; ++i)
  {
    TestData.storage_ptr_test_C[i] = dl_list_3->extractFirst();
    if (nullptr == TestData.storage_ptr_test_C[i])
      printf("ERROR: NULL pointer extracted at first position\n");
    else
      printf("extracted \"%s\" at first position in dl_list_3\n", TestData.storage_ptr_test_C[i]);
  }
  printf("dl_list_3:\n");
  dl_list_3->print();
  printf("\t extractLast dl_list_3\n");
  for (u16 i = 6; i > 4; --i)
  {
    TestData.storage_ptr_test_C[i] = dl_list_3->extractLast();
    if (nullptr == TestData.storage_ptr_test_C[i])
      printf("ERROR: NULL pointer extracted at Last position\n");
    else
      printf("extracted \"%s\" at last position in dl_list_3\n", TestData.storage_ptr_test_C[i]);
  }
  printf("dl_list_3:\n");
  dl_list_3->print();

  void *data = nullptr;
  u16 extract_errors = 0;
  for (u16 i = 0; i < kNumberOfStoragePtrTest_C; ++i)
  {
    if (false == dl_list_3->isEmpty())
    {
      data = dl_list_3->extractLast();
      free(data);
      printf("ERROR: Trying to extract an element in an empty vector (dl_list_3 length = %d)\n", dl_list_3->length());
      extract_errors++;
    }
  }
  if (extract_errors > 0)
  {
    printf("  ==> ERROR: isEmpty doesn't work correctly (dl_list_3)\n");
  }

  printf("\n\n# Test Insert\n");
  printf("\t insertLast dl_list_3\n");
  for (u16 i = 0; i < kNumberOfStoragePtrTest_C; ++i)
  {
    error_type = dl_list_3->insertLast(TestData.storage_ptr_test_C[i], (strlen((char*)TestData.storage_ptr_test_C[i]) + 1));
    TestData.storage_ptr_test_C[i] = nullptr;
    PrintFunctionError(dl_list_3, (u8 *)"insertLast dl_list_3", error_type);
  }
  printf("dl_list_3:\n");
  dl_list_3->print();

  printf("\n\n# Test First\n");
  printf("\t first dl_list_1\n");
  data = dl_list_1->first();
  if (nullptr == data)
    printf("First in dl_list_1: NULL\n");
  else
    printf("First in dl_list_1: \"%s\"\n", data);

  printf("\n\n# Test At\n");
  position = 1;
  printf("\t at dl_list_3 (position %d)\n", position);
  data = dl_list_3->at(1);
  if (nullptr == data)
    printf("At in dl_list_3: NULL\n");
  else
    printf("At in dl_list_3 (position %d): \"%s\"\n", position, data);

  printf("\n\n# Test Last\n");
  printf("\t last dl_list_1\n");
  data = dl_list_1->last();
  if (nullptr == data)
    printf("Last in dl_list_1: NULL\n");
  else
    printf("Last in dl_list_1: \"%s\"\n", data);

  printf("\n\n# Test Length\n");
  printf("\t dl_list_1: [Capacity = %d] - [Length  = %d]\n", dl_list_1->capacity(), dl_list_1->length());
  printf("\t dl_list_2: [Capacity = %d] - [Length  = %d]\n", dl_list_2->capacity(), dl_list_2->length());
  printf("\t dl_list_3: [Capacity = %d] - [Length  = %d]\n", dl_list_3->capacity(), dl_list_3->length());

  printf("\n\n# Test Concat\n");
  printf("\t concat dl_list_1 + dl_list_3\n");
  error_type = dl_list_1->concat(dl_list_3);
  PrintFunctionError(dl_list_1, (u8 *)"concat dl_list_1 + dl_list_3", error_type);
  printf("dl_list_1:\n");
  dl_list_1->print();
  printf("dl_list_3:\n");
  dl_list_3->print();

  printf("\n\n# Test Length\n");
  printf("\t Length dl_list_1 = %d \n", dl_list_1->length());
  printf("\t Length dl_list_2 = %d \n", dl_list_2->length());
  printf("\t Length dl_list_3 = %d \n", dl_list_3->length());

  printf("\n\n# Test Reset\n");
  error_type = dl_list_3->reset();
  PrintFunctionError(dl_list_3, (u8 *)"reset dl_list_3", error_type);

  printf("\n\n# Test Length\n");
  printf("\t Length dl_list_1 = %d \n", dl_list_1->length());
  printf("\t Length dl_list_2 = %d \n", dl_list_2->length());
  printf("\t Length dl_list_3 = %d \n", dl_list_3->length());


  printf("\n\n# Test First\n");
  printf("\t first dl_list_3\n");
  data = dl_list_3->first();
  if (nullptr == data)
    printf("First in dl_list_3: NULL\n");
  else
    printf("ERROR: First in dl_list_3: \"%s\"\n", data);

  printf("\n\n# Test At\n");
  position = 1;
  printf("\t at dl_list_3 (position %d)\n", position);
  data = dl_list_3->at(1);
  if (nullptr == data)
    printf("At in dl_list_3: NULL\n");
  else
    printf("ERROR: At in dl_list_3 (position %d): \"%s\"\n", position, data);

  printf("\n\n# Test Last\n");
  printf("\t last dl_list_3\n");
  data = dl_list_3->last();
  if (nullptr == data)
    printf("Last in dl_list_3: NULL\n");
  else
    printf("ERROR: Last in dl_list_3: \"%s\"\n", data);


  printf("\n\n# Test Concat\n");
  printf("\t concat dl_list_3 + dl_list_2\n");
  error_type = dl_list_3->concat(dl_list_2);
  PrintFunctionError(dl_list_3, (u8 *)"concat dl_list_3 + dl_list_2", error_type);
  printf("dl_list_3:\n");
  dl_list_3->print();

  printf("\n\n# Test Destroy\n");
  printf("\t dl_list_2 Destroy\n");
  //error_type = dl_list_2->destroy();
  dl_list_2 = nullptr;
  PrintFunctionError(dl_list_2, (u8 *)"destroy dl_list_2", error_type);
  printf("dl_list_2:\n");
  dl_list_2->print();

  //printf("\t dl_list_3 Destroy\n");
  //error_type = dl_list_3->destroy();
  dl_list_3 = nullptr;
  PrintFunctionError(dl_list_3, (u8 *)"destroy dl_list_3", error_type);
  printf("dl_list_3:\n");
  dl_list_3->print();

  printf("\n\n# Test Resize\n");
  printf("\t dl_list_1 resize\n");
  error_type = dl_list_1->resize(3);
  PrintFunctionError(dl_list_1, (u8 *)"resize dl_list_1", error_type);
  printf("dl_list_1:\n");
  dl_list_1->print();


  /*
  printf("\n\n---------------- NULL BATTERY ----------------\n\n");
  printf("\n\n# Test Create\n");
  dl_list_2 = VECTOR_create(0);
  if (NULL != dl_list_2) {
    printf("ERROR: trying to create dl_list_2 with 0 capacity\n");
    return -1;
  }

  printf("\n\n# Test Insert\n");
  data = TestData.single_ptr_big_data;
  error_type = v->ops_->insertFirst(dl_list_2, data, kSingleSizeBigData);
  PrintFunctionError(dl_list_2, (u8 *)"insertFirst dl_list_2 (NOT VALID)", error_type);
  error_type = v->ops_->insertLast(dl_list_2, data, kSingleSizeBigData);
  PrintFunctionError(dl_list_2, (u8 *)"insertLast dl_list_2 (NOT VALID)", error_type);
  error_type = v->ops_->insertAt(dl_list_2, data, kSingleSizeBigData, 1);
  PrintFunctionError(dl_list_2, (u8 *)"insertAt dl_list_2 (NOT VALID)", error_type);

  printf("\n\n# Test First\n");
  printf("\t first dl_list_2\n");
  data = v->ops_->first(dl_list_2);
  if (NULL != data)
    printf("ERROR: First in dl_list_2 is not NULL\n");

  printf("\n\n# Test At\n");
  printf("\t at dl_list_2 (position 1)\n");
  data = v->ops_->at(dl_list_2, 1);
  if (NULL != data)
    printf("ERROR: At in dl_list_2 is not NULL\n");

  printf("\n\n# Test Last\n");
  printf("\t last dl_list_2\n");
  data = v->ops_->last(dl_list_2);
  if (NULL != data)
    printf("ERROR: Last in dl_list_2 is not NULL\n");


  printf("\n\n# Test Extract\n");
  data = v->ops_->extractFirst(dl_list_2);
  if (NULL != data)
  {
    printf("ERROR: trying to extractFirst from an empty vector\n");
  }
  data = v->ops_->extractLast(dl_list_2);
  if (NULL != data)
  {
    printf("ERROR: trying to extractLast from an empty vector\n");
  }
  data = v->ops_->extractAt(dl_list_2, 1);
  if (NULL != data)
  {
    printf("ERROR: trying to extractAt from an empty vector\n");
  }

  printf("\n\n# Test Concat\n");
  printf("\t concat dl_list_1 + dl_list_2\n");

  error_type = v->ops_->concat(dl_list_1, dl_list_2);
  PrintFunctionError(dl_list_1, (u8 *)"concat dl_list_1 + dl_list_2 (NOT VALID)", error_type);



  printf("\n\n# Test Resize\n");
  error_type = v->ops_->resize(dl_list_2, 5);
  PrintFunctionError(dl_list_2, (u8 *)"resize dl_list_2 (NOT VALID)", error_type);

  printf("\n\n# Test Reset\n");
  error_type = v->ops_->reset(dl_list_2);
  PrintFunctionError(dl_list_2, (u8 *)"reset dl_list_2 (NOT VALID)", error_type);


  printf("\n\n# Test Destroy\n");
  error_type = v->ops_->destroy(dl_list_2);
  PrintFunctionError(dl_list_2, (u8 *)"destroy dl_list_2 (NOT VALID)", error_type);
  error_type = v->ops_->destroy(dl_list_3);
  PrintFunctionError(dl_list_3, (u8 *)"destroy dl_list_3 (NOT VALID)", error_type);


  // Work is done, clean the system
  error_type = v->ops_->destroy(dl_list_1);
  PrintFunctionError(dl_list_1, (u8 *)"destroy dl_list_1", error_type);
  error_type = v->ops_->destroy(v);
  PrintFunctionError(v, (u8 *)"destroy Vector Operations", error_type);

  printf("\nAt this point, it must be only one block of %d bytes waiting to be freed\n\n", kSingleSizeBigData);
  MM->status();
  TESTBASE_freeDataForTest();
  MM->status();*/

  printf("\n\n---------------- OPERATOR= COPY C ----------------\n\n");

  DL_List *dl_list_1c = nullptr;
  DL_List *dl_list_1i = new DL_List();

  dl_list_1c = new DL_List(*dl_list_1);
  *dl_list_1i = *dl_list_1;

  printf("DL_List1 original\n\n");
  dl_list_1->print();
  printf("DL_list 1 copy constructor\n\n");
  dl_list_1c->print();
  printf("DL_List 1 operator =\n\n");
  dl_list_1i->print();



  delete dl_list_1;
  delete dl_list_2;
  delete dl_list_3;
  printf("Press ENTER to continue\n");
  getchar();
}
