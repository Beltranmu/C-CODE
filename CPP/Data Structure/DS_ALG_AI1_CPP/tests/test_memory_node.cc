// test_memory_node.c
// Toni Barella, Ivan Sancho
// Algoritmos & Inteligencia Artificial
// ESAT 2020-2021
//
//Test battery for memory_node

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "adt_memory_node.h"

const int kSizeData1 = 5;
const int kSizeData2 = 15;
const int kSizeData3 = 10;


void PrintFunctionError(void *address, u8 *msg, s16 error_type) {
  u8 *error_msg = (u8 *)malloc(sizeof(u8) * 100); //If error in line 250, perhaps more than 100.
  if (NULL == error_msg) {
    printf(" [TEST] Error allocating error mesage\n");
    return;
  }

  char*  error_code[21];
  error_code[0] = "Everything OK";
  error_code[1] = "Memory Error";
  error_code[2] = "File Error";
  error_code[3] = "Error, Size 0";
  error_code[4] = "Error, no data";
  error_code[5] = "Vector NULL, Error";
  error_code[6] = "Error, Vector is full :D";
  error_code[7] = "Error, Non fuction";
  error_code[8] = "Circular Vector NULL, Error";
  error_code[9] = "Movable Head Vector NULL, Error";
  error_code[10] = "List NULL, Error";
  error_code[11] = "List FULL, Error";
  error_code[12] = "List Empty, Error";
  error_code[13] = "Stack NULL, Error";
  error_code[14] = "Stack FULL, Error";
  error_code[15] = "Queue NULL, Error";
  error_code[16] = "Queue FULL, Error";
  error_code[17] = "Queue Empty, Error";
  error_code[18] = "Logger NULL, Error";
  error_code[19] = "Logger Empty, Error";
  error_code[20] = "No Memory Available";




  strcpy((char *)error_msg, error_code[error_type / (-10)]);
  printf(" [TEST] Function %s returns (%s) ", msg, error_msg);

  printf(" in address = %p", address);
  printf("\n");
  free(error_msg);
}

int main(){
	
  printf("\n\n___STARTING MEMORY NODE TEST___\n");

  //void *data_ = (void*) new u8[bytes];


  //DATA
  u8 *ptr_data_1 = new u8[kSizeData1]; // abcde
  for (u8 i = 0; i < kSizeData1; ++i){
    *(ptr_data_1 + i) = 'a' + i;
  }
  u8 *ptr_data_2 = new u8[kSizeData2]; // fghijklmnopqrst
  for (u8 i = 0; i < kSizeData2; ++i){
    *(ptr_data_2 + i) = 'f' + i;
  }
  u8 *ptr_data_3 = new u8[kSizeData3]; // 0123456789
  for (u8 i = 0; i < kSizeData3; ++i){
    *(ptr_data_3 + i) = '0' + i;
  }

  printf("\n");
  printf("ptr_data_1 = ");
  for (u8 i = 0; i < kSizeData1; ++i)
  {
    printf("%c", *(ptr_data_1 + i));
  }
  printf("\n");
  printf("ptr_data_2 = ");
  for (u8 i = 0; i < kSizeData2; ++i)
  {
    printf("%c", *(ptr_data_2 + i));
  }
  printf("\n");
  printf("ptr_data_3 = ");
  for (u8 i = 0; i < kSizeData3; ++i)
  {
    printf("%c", *(ptr_data_3 + i));
  }
  printf("\n");

	// nodes to work with
  MemoryNode *node_1 = nullptr;
  MemoryNode *node_2 = nullptr;
  MemoryNode *node_3 = nullptr;
  int error_type;
  //Creation

    node_1 = new MemoryNode();
  if (nullptr == node_1) {
    printf("\n create returned a null node in node_1");
    return -1;
  }
    node_2 = new MemoryNode();
  if (nullptr == node_2) {
    printf("\n create returned a null node in node_2");
    return -1;
  }
    node_3 = new MemoryNode();
  if (nullptr == node_3) {
    printf("\n create returned a null node in node_3");
    return -1;
  }

  //Test reset and free
  
  error_type = node_1->reset();
  PrintFunctionError(node_1, (u8 *)"reset node_1", error_type);
  

 //Data

  error_type = node_1->setData(ptr_data_1, kSizeData1);
  PrintFunctionError(node_1, (u8 *)"setData Node 1", error_type);
  printf("Node 1:\n");
  node_1->print();
	
  printf("\t setData Node 2\n");
  error_type = node_2->setData(ptr_data_3, kSizeData3);
  PrintFunctionError(node_2, (u8 *)"setData Node 2", error_type);
  printf("Node 2:\n");
  node_2->print();


  // memSet
  printf("\n\n# Test memSet in Node 2 to '.'\n");
  error_type = node_2->memSet (0x2E);
  PrintFunctionError(node_2, (u8 *)"memSet", error_type);
  printf("Node 2:\n");
  node_2->print();

  // reset
  printf("\n# Test reset Node 2\n");
  error_type = node_2->reset();
  PrintFunctionError(node_2, (u8 *)"reset", error_type);
  printf("Node 2:\n");
  node_2->print();

  // setData
  printf("\n# setData Node 2\n");
  error_type = node_2->setData(ptr_data_2, kSizeData2);
  PrintFunctionError(node_2, (u8 *)"setData", error_type);
  printf("Node 2:\n");
  node_2->print();
  

  // memCopy
  printf("\n\n# Test memCopy Node 2 to Node 2\n");
  error_type = node_2->memCopy (node_2->data(), node_2->size());
  PrintFunctionError(node_2, (u8 *)"memCopy", error_type);
  printf("Node 2:\n");
  node_2->print();

  // memConcat
  printf("\n\n# Test memConcat Node 1 and Node 2\n\nBefore:\n");
  printf("Node 1:\n");
  node_1->print();
  printf("Node 2:\n");
  node_2->print();
  printf("\n");
  error_type = node_1->memConcat(node_2->data(),node_2->size());
  PrintFunctionError(node_1, (u8 *)"memConcat Node 1 and Node 2", error_type);
  printf("After:\n");
  printf("Node 1:\n");
  node_1->print();
  printf("Node 2:\n");
  node_2->print();

  // memMask
  printf("\n\n# Test memMask in Node 1 (mask to Capitals)\n");
  error_type = node_1->memMask(0x5F);
  PrintFunctionError(node_1, (u8 *)"memMask", error_type);
  printf("Node 1:\n");
  node_1->print();
 

  printf("\n\n---------------- OPERATOR= COPY C ----------------\n\n");

  MemoryNode *node_1c = nullptr;
  MemoryNode *node_1i = new MemoryNode();

  node_1c = new MemoryNode(*node_1);
  printf("Node 1 original\n");
  node_1->print();
  printf("Node 1 Copy constructor\n");
  node_1c->print();
  
  *node_1i = *node_1;
  printf("Node 1 operator = \n");
  node_1i->print();



  // free
  printf("\n# Test Free Node 2\n");
  error_type = node_2->free();
  PrintFunctionError(node_2, (u8 *)"free", error_type);

  // setData
  printf("\n# setData Node 3 (sharing with Node 1)\n");
  error_type = node_3->setData(node_1->data(), node_1->size());
  PrintFunctionError(node_3, (u8 *)"setData", error_type);
  printf("Node 3:\n");
  node_3->print();
  

  // softFree
  printf("\n# softFree Node 1 (shared with Node 3)\n");
  error_type = node_1->softFree();
  node_1 = nullptr;
  PrintFunctionError(node_1, (u8 *)"softFree", error_type);
  printf("Node 1:\n");
  node_1->print();
  printf("Node 3:\n");
  node_3->print();

  // free
  printf("\n# Test Free\n");
  error_type = node_3->free();
  node_3 = nullptr;
  PrintFunctionError(node_3, (u8 *)"free Node 3", error_type);


 
  delete node_1;
  //delete node_2;



	printf("Press ENTER to continue\n");
	getchar();
	return 0;
}
