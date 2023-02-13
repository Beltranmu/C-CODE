// test_stack.c
// Escuela Superior de Arte y Tecnologia
// Algoritmos & Inteligencia Artificial
// ESAT 2020-2021
//
// Test battery for stack ADT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <adt_stack.h>

#include "test_base.cc"

const u16 kCapacityStack1 = 30;
const u16 kCapacityStack2 = 5;
const u16 kCapacityStack3 = 2;
int main()
{
  s16 error_type = 0;

  TESTBASE_generateDataForTest();


  // Stacks to work with
  Stack *stack_1 = nullptr;
  Stack *stack_2 = nullptr;
  Stack *stack_3 = nullptr;
  stack_1 = new Stack();
  stack_1->init(kCapacityStack1);
  if (nullptr == stack_1) {
    printf("\n create returned a null node in stack_1\n");
    return -1;
  }
  stack_2 = new Stack();
  stack_2->init(kCapacityStack2);
  if (nullptr == stack_2) {
    printf("\n create returned a null node in stack_2\n");
    return -1;
  }
  stack_3 = new Stack();
  stack_3->init(kCapacityStack3);
  if (nullptr == stack_3) {
    printf("\n create returned a null node in stack_3\n");
    return -1;
  }


  //FIRST BATTERY
  printf("---------------- FIRST BATTERY ----------------\n\n");
  // reset
  printf("\n\n# Test Reset\n");
  error_type = stack_1->reset();
  PrintFunctionError(stack_1, (u8 *)"reset stack_1", error_type);

  error_type = stack_1->push(TestData.single_ptr_data_4, kSingleSizeData4);
  TestData.single_ptr_data_4 = nullptr;
  PrintFunctionError(stack_1, (u8 *)"push stack_1", error_type);

  error_type = stack_1->reset();
  PrintFunctionError(stack_1, (u8 *)"reset stack_1", error_type);

  if (false == stack_1->isEmpty()){
    printf("ERROR: isEmpty doesn't work correctly (stack_1)\n");
  }

  printf("\n\n# Test Push\n");
  printf("\t push stack_1\n");
  for (u16 i = 0; i < kNumberOfStoragePtrTest_A; ++i)
  {
    error_type = stack_1->push(TestData.storage_ptr_test_A[i], (strlen((char*)TestData.storage_ptr_test_A[i]) + 1));
    TestData.storage_ptr_test_A[i] = nullptr;
    PrintFunctionError(stack_1, (u8 *)"push stack_1", error_type);
  }
  printf("stack_1:\n");
  stack_1->print();

  printf("\t push stack_1\n");
  for (u16 i = 0; i < kNumberOfStoragePtrTest_B; ++i)
  {
    error_type = stack_1->push(TestData.storage_ptr_test_B[i], (strlen((char*)TestData.storage_ptr_test_B[i]) + 1));
    TestData.storage_ptr_test_B[i] = nullptr;
    PrintFunctionError(stack_1, (u8 *)"push stack_1", error_type);
  }
  printf("stack_1:\n");
  stack_1->print();

  printf("\t stack_2 push\n");
  error_type = stack_2->push(TestData.single_ptr_data_1, kSingleSizeData1);
  TestData.single_ptr_data_1 = nullptr;
  PrintFunctionError(stack_2, (u8 *)"push stack_2", error_type);

  error_type = stack_2->push(TestData.single_ptr_data_1, kSingleSizeData1);
  PrintFunctionError(stack_2, (u8 *)"push stack_2 with NULL data", error_type);

  error_type = stack_2 ->push(TestData.single_ptr_data_2, kSingleSizeData2);
  TestData.single_ptr_data_2 = nullptr;
  PrintFunctionError(stack_2, (u8 *)"push stack_2", error_type);

  error_type = stack_2->push(TestData.single_ptr_data_3, kSingleSizeData3);
  TestData.single_ptr_data_3 = nullptr;
  PrintFunctionError(stack_2, (u8 *)"push stack_2", error_type);

  printf("stack_2:\n");
  stack_2->print();

  printf("\t push stack_3\n");
  u16 insert_errors = 0;
  for (u16 i = 0; i < kNumberOfStoragePtrTest_C; ++i)
  {
    if (false == stack_3->isFull())
    {
      error_type = stack_3->push(TestData.storage_ptr_test_C[i], (strlen((char*)TestData.storage_ptr_test_C[i]) + 1));
      TestData.storage_ptr_test_C[i] = nullptr;
      PrintFunctionError(stack_3, (u8 *)"push stack_3", error_type);
    }
    else
    {
      printf("ERROR: Trying to insert an element in a full stackvector (stack_3 capacity = %d)\n", stack_3->capacity());
      insert_errors++;
    }
  }
  if ((kNumberOfStoragePtrTest_C - kCapacityStack3) != insert_errors)
  {
    printf("  ==> ERROR: isFull doesn't work correctly (stack_3)\n");
  }
  printf("stack_3:\n");
  stack_3->print();

  printf("\n\n# Test Resize\n");
  printf("\t resize stack_3\n");
  error_type = stack_3->resize(10);
  PrintFunctionError(stack_3, (u8 *)"resize stack_3", error_type);
  printf("stack_3:\n");
  stack_3->print();

  for (u16 i = 2; i < kNumberOfStoragePtrTest_C; ++i)
  {
    error_type = stack_3->push(TestData.storage_ptr_test_C[i], (strlen((char*)TestData.storage_ptr_test_C[i]) + 1));
    TestData.storage_ptr_test_C[i] = nullptr;
    PrintFunctionError(stack_3, (u8 *)"push stack_3", error_type);
  }
  printf("stack_3:\n");
  stack_3->print();

  printf("\n\n# Test Pop\n");
  printf("\t pop stack_3\n");
  u16 position = 2;
  TestData.storage_ptr_test_C[position] = stack_3->pop();
  if (nullptr == TestData.storage_ptr_test_C[position])
    printf("ERROR: NULL pointer extracted at top stack_3\n");
  else
    printf("extracted \"%s\" at top in stack_3\n", TestData.storage_ptr_test_C[position]);
  position++;
  TestData.storage_ptr_test_C[position] = stack_3->pop();
  if (nullptr == TestData.storage_ptr_test_C[position])
    printf("ERROR: NULL pointer extracted at top stack_3\n");
  else
    printf("extracted \"%s\" at top in stack_3\n", TestData.storage_ptr_test_C[position]);
  position++;
  TestData.storage_ptr_test_C[position] = stack_3->pop();
  if (nullptr == TestData.storage_ptr_test_C[position])
    printf("ERROR: NULL pointer extracted at top stack_3\n");
  else
    printf("extracted \"%s\" at top in stack_3\n", TestData.storage_ptr_test_C[position]);

  printf("stack_3:\n");
  stack_3->print();

  printf("\t pop stack_3\n");
  for (u16 i = 0; i < 2; ++i)
  {
    TestData.storage_ptr_test_C[i] = stack_3->pop();
    if (nullptr == TestData.storage_ptr_test_C[i])
      printf("ERROR: NULL pointer extracted at top position\n");
    else
      printf("extracted \"%s\" at top position in stack_3\n", TestData.storage_ptr_test_C[i]);
  }
  printf("stack_3:\n");
  stack_3->print();
  printf("\t pop stack_3\n");
  for (u16 i = 6; i > 4; --i)
  {
    TestData.storage_ptr_test_C[i] = stack_3->pop();
    if (nullptr == TestData.storage_ptr_test_C[i])
      printf("ERROR: NULL pointer extracted at top position\n");
    else
      printf("extracted \"%s\" at top position in stack_3\n", TestData.storage_ptr_test_C[i]);
  }
  printf("stack_3:\n");
  stack_3->print();

  void *data = nullptr;
  u16 extract_errors = 0;
  for (u16 i = 0; i < kNumberOfStoragePtrTest_C; ++i)
  {
    if (false == stack_3->isEmpty())
    {
      data = stack_3->pop();
      //MM->free(data);
      printf("ERROR: Trying to extract an element in an empty stack (stack_3 length = %d)\n", stack_3->length());
      extract_errors++;
    }
  }
  if (extract_errors > 0)
  {
    printf("  ==> ERROR: isEmpty doesn't work correctly (stack_3)\n");
  }

  printf("\n\n# Test Push\n");
  printf("\t push stack_3\n");
  for (u16 i = 0; i < kNumberOfStoragePtrTest_C; ++i)
  {
    error_type = stack_3->push(TestData.storage_ptr_test_C[i], (strlen((char*)TestData.storage_ptr_test_C[i]) + 1));
    TestData.storage_ptr_test_C[i] = nullptr;
    PrintFunctionError(stack_3, (u8 *)"push stack_3", error_type);
  }
  printf("stack_3:\n");
  stack_3->print();

  printf("\n\n# Test Top\n");
  printf("\t top stack_1\n");
  data = stack_1->top();
  if (nullptr == data)
    printf("Top in stack_1: NULL\n");
  else
    printf("Top in stack_1: \"%s\"\n", data);

  printf("\n\n# Test Length\n");
  printf("\t stack_1: [Capacity = %d] - [Length  = %d]\n", stack_1->capacity(), stack_1->length());
  printf("\t stack_2: [Capacity = %d] - [Length  = %d]\n", stack_2->capacity(), stack_2->length());
  printf("\t stack_3: [Capacity = %d] - [Length  = %d]\n", stack_3->capacity(), stack_3->length());

  printf("\n\n# Test Concat\n");
  printf("\t concat stack_1 + stack_3\n");
  error_type = stack_1->concat(stack_3);
  PrintFunctionError(stack_1, (u8 *)"concat stack_1 + stack_3", error_type);
  printf("stack_1:\n");
  stack_1->print();
  printf("stack_3:\n");
  stack_3->print();

  printf("\n\n# Test Length\n");
  printf("\t Length stack_1 = %d \n", stack_1->length());
  printf("\t Length stack_2 = %d \n", stack_2->length());
  printf("\t Length stack_3 = %d \n", stack_3->length());

  printf("\n\n# Test Reset\n");
  error_type = stack_3->reset();
  PrintFunctionError(stack_3, (u8 *)"reset stack_3", error_type);

  printf("\n\n# Test Length\n");
  printf("\t Length stack_1 = %d \n", stack_1->length());
  printf("\t Length stack_2 = %d \n", stack_2->length());
  printf("\t Length stack_3 = %d \n", stack_3->length());

  printf("\n\n# Test Top\n");
  printf("\t top stack_3\n");
  data = stack_3->top();
  if (NULL == data)
    printf("Top in stack_3: NULL\n");
  else
    printf("ERROR: First in stack_3: \"%s\"\n", data);

  printf("\n\n# Test Concat\n");
  printf("\t concat stack_3 + stack_2\n");
  error_type = stack_3->concat(stack_2);
  PrintFunctionError(stack_3, (u8 *)"concat stack_3 + stack_2", error_type);
  printf("stack_3:\n");
  stack_3->print();

  /*printf("\n\n# Test Destroy\n");
  printf("\t stack_2 Destroy\n");
  error_type = stack_2->destroy();
  stack_2 = NULL;
  PrintFunctionError(stack_2, (u8 *)"destroy stack_2", error_type);
  printf("stack_2:\n");
  stack_2->print(stack_2);

  printf("\t stack_3 Destroy\n");
  error_type = stack_3->destroy();
  stack_3 = NULL;
  PrintFunctionError(stack_3, (u8 *)"destroy stack_3", error_type);
  printf("stack_3:\n");
  stack_3->print();*/

  printf("\n\n# Test Resize\n");
  printf("\t stack_1 resize\n");
  error_type = stack_1->resize(3);
  PrintFunctionError(stack_1, (u8 *)"resize stack_1", error_type);
  printf("stack_1:\n");
  stack_1->print();

  //MM->status();

  /*printf("\n\n---------------- NULL BATTERY ----------------\n\n");
  printf("\n\n# Test Create\n");
  stack_2 = STACK_create(0);
  if (NULL != stack_2) {
    printf("ERROR: trying to create stack_2 with 0 capacity\n");
    return -1;
  }

  printf("\n\n# Test Insert\n");
  data = TestData.single_ptr_big_data;
  error_type = s->ops_->push(stack_2, data, kSingleSizeBigData);
  PrintFunctionError(stack_2, (u8 *)"push stack_2 (NOT VALID)", error_type);

  printf("\n\n# Test Top\n");
  printf("\t top stack_2\n");
  data = s->ops_->top(stack_2);
  if (NULL != data)
    printf("ERROR: Top in stack_2 is not NULL\n");

  printf("\n\n# Test Extract\n");
  data = s->ops_->pop(stack_2, NULL);
  if (NULL != data)
  {
    printf("ERROR: trying to pop from an empty stack\n");
  }

  printf("\n\n# Test Concat\n");
  printf("\t concat stack_1 + stack_2\n");
  error_type = s->ops_->concat(stack_1, stack_2);
  PrintFunctionError(stack_1, (u8 *)"concat stack_1 + stack_2 (NOT VALID)", error_type);

  printf("\n\n# Test Resize\n");
  error_type = s->ops_->resize(stack_2, 5);
  PrintFunctionError(stack_2, (u8 *)"resize stack_2 (NOT VALID)", error_type);

  printf("\n\n# Test Reset\n");
  error_type = s->ops_->reset(&stack_2);
  PrintFunctionError(stack_2, (u8 *)"reset stack_2 (NOT VALID)", error_type);


  printf("\n\n# Test Destroy\n");
  error_type = s->ops_->destroy(stack_2);
  PrintFunctionError(stack_2, (u8 *)"destroy stack_2 (NOT VALID)", error_type);
  error_type = s->ops_->destroy(stack_3);
  PrintFunctionError(stack_3, (u8 *)"destroy stack_3 (NOT VALID)", error_type);


  // Work is done, clean the system
  error_type = s->ops_->destroy(stack_1);
  PrintFunctionError(stack_1, (u8 *)"destroy stack_1", error_type);
  error_type = s->ops_->destroy(s);
  PrintFunctionError(s, (u8 *)"destroy Stack Operations", error_type);

  printf("\nAt this point, it must be only one block of %d bytes waiting to be freed\n\n", kSingleSizeBigData);
  MM->status();
  TESTBASE_freeDataForTest();
  MM->status();*/

  printf("\n\n---------------- OPERATOR= COPY C ----------------\n\n");

  Stack *stack_1c = nullptr;
  Stack *stack_1i = new Stack();

  stack_1c = new Stack(*stack_1);
  *stack_1i = *stack_1;

  printf("Stack 1 original\n\n");
  stack_1->print();
  printf("Stack 1 copy constructor\n\n");
  stack_1c->print();
  printf("Stack 1 operator = \n\n");
  stack_1i->print();



  delete stack_1;
  delete stack_2;
  delete stack_3;
  printf("Press ENTER to continue\n");
  getchar();
  
  return 0;
}
