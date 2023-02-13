// comparative.c
// Escuela Superior de Arte y Tecnologia
// Algoritmos & Inteligencia Artificial
// ESAT 2020-2021
//
// File for comparative of ADTs

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ABGS_MemoryManager/abgs_memory_manager.h"
#include "ABGS_MemoryManager/abgs_platform_types.h"
#include "common_def.h"
#include "adt_vector.h"
#include "adt_list.h"
#include "adt_dl_list.h"

//Comparatives vector List dl_List
const int knTimes = 10000;
void* data[10000];


Vector* c_vector = NULL;
List* c_list = NULL;
DL_List* c_dl_list = NULL;

Vector* concat_vector1 = NULL;
Vector* concat_vector2 = NULL;
List* concat_list = NULL;
DL_List* concat_dl_list = NULL;



void TESTBASE_generateDataForComparative() {

  for (int i = 0; i < knTimes; ++i) {
    data[i] = MM->malloc(16);
    sprintf(data[i], "Mollete>Paella");
  }

  concat_vector1 = VECTOR_create(1);
  concat_vector2 = VECTOR_create(1);
  concat_list = LIST_create(1);
  concat_dl_list = DL_LIST_create(1);

  if (NULL == concat_vector1) {
    printf("No memory to concat vector");
  }
  if (NULL == concat_vector2) {
    printf("No memory to concat vector");
  }
  //concat_vector1->ops_->insertLast(concat_vector1, data[0], 16);
  concat_vector2->ops_->insertLast(concat_vector2, data[0], 16);
  if (NULL == concat_list) {
    printf("No memory to concat list");
  }
  concat_list->ops_->insertLast(concat_list, data[0], 16);

  if (NULL == concat_dl_list) {
    printf("No memory to concat dl_list");
  }
  concat_dl_list->ops_->insertLast(concat_dl_list, data[0], 16);
  if (NULL == c_vector) {
    c_vector = VECTOR_create(knTimes);
  }
  if (NULL == c_list) {
    c_list = LIST_create(knTimes);
  }
  if (NULL == c_dl_list) {
    c_dl_list = DL_LIST_create(knTimes);
  }

}


//Vector

void VectorInsertFirst() {
  LARGE_INTEGER frequency;				// ticks per second
  LARGE_INTEGER  time_start, time_end;    // ticks in interval
  double elapsed_time = 0.0f;
  u32 repetitions = knTimes;

  ///////////////////////////////////////////////////////////////////////
  // Frequency: ticks per second
  QueryPerformanceFrequency(&frequency);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // Meassurement time
  // start timer
  QueryPerformanceCounter(&time_start);
  // execute function to meassure 'repetitions' times
  for (u32 rep = 0; rep < repetitions; ++rep) {
    c_vector->ops_->insertFirst(c_vector, data[rep], 14);
  }
  // stop timer
  QueryPerformanceCounter(&time_end);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("Vector ||| InsertFirst\n");
  printf("Elapsed time: %f ms\n", elapsed_time);
  ///////////////////////////////////////////////////////////////////////
  // compute the average time
  double average_time = elapsed_time / repetitions;
  printf("Average time: %f ms\n\n", average_time);
  ///////////////////////////////////////////////////////////////////////
  
}

void VectorExtractFirst() {
  LARGE_INTEGER frequency;				// ticks per second
  LARGE_INTEGER  time_start, time_end;    // ticks in interval
  double elapsed_time = 0.0f;
  u32 repetitions = knTimes;

  ///////////////////////////////////////////////////////////////////////
  // Frequency: ticks per second
  QueryPerformanceFrequency(&frequency);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // Meassurement time
  // start timer
  QueryPerformanceCounter(&time_start);
  // execute function to meassure 'repetitions' times
  for (u32 rep = 0; rep < repetitions; ++rep) {
    c_vector->ops_->extractFirst(c_vector);
  }
  // stop timer
  QueryPerformanceCounter(&time_end);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("Vector ||| ExtractFirst\n");
  printf("Elapsed time: %f ms\n", elapsed_time);
  ///////////////////////////////////////////////////////////////////////
  // compute the average time
  double average_time = elapsed_time / repetitions;
  printf("Average time: %f ms\n\n", average_time);
  ///////////////////////////////////////////////////////////////////////
}


void VectorInsertLast() {
  LARGE_INTEGER frequency;				// ticks per second
  LARGE_INTEGER  time_start, time_end;    // ticks in interval
  double elapsed_time = 0.0f;
  u32 repetitions = knTimes;

  ///////////////////////////////////////////////////////////////////////
  // Frequency: ticks per second
  QueryPerformanceFrequency(&frequency);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // Meassurement time
  // start timer
  QueryPerformanceCounter(&time_start);
  // execute function to meassure 'repetitions' times
  for (u32 rep = 0; rep < repetitions; ++rep) {
    c_vector->ops_->insertLast(c_vector, data[rep], 14);
  }
  // stop timer
  QueryPerformanceCounter(&time_end);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("Vector ||| InsertLast\n");
  printf("Elapsed time: %f ms\n", elapsed_time);
  ///////////////////////////////////////////////////////////////////////
  // compute the average time
  double average_time = elapsed_time / repetitions;
  printf("Average time: %f ms\n\n", average_time);
  ///////////////////////////////////////////////////////////////////////
}

void VectorExtractLast() {
  LARGE_INTEGER frequency;				// ticks per second
  LARGE_INTEGER  time_start, time_end;    // ticks in interval
  double elapsed_time = 0.0f;
  u32 repetitions = knTimes;

  ///////////////////////////////////////////////////////////////////////
  // Frequency: ticks per second
  QueryPerformanceFrequency(&frequency);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // Meassurement time
  // start timer
  QueryPerformanceCounter(&time_start);
  // execute function to meassure 'repetitions' times
  for (u32 rep = 0; rep < repetitions; ++rep) {
    c_vector->ops_->extractLast(c_vector);
  }
  // stop timer
  QueryPerformanceCounter(&time_end);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("Vector ||| ExtractLast\n");
  printf("Elapsed time: %f ms\n", elapsed_time);
  ///////////////////////////////////////////////////////////////////////
  // compute the average time
  double average_time = elapsed_time / repetitions;
  printf("Average time: %f ms\n\n", average_time);
  ///////////////////////////////////////////////////////////////////////
}


void VectorInsertAt() {
  LARGE_INTEGER frequency;				// ticks per second
  LARGE_INTEGER  time_start, time_end;    // ticks in interval
  double elapsed_time = 0.0f;
  u32 repetitions = knTimes;

  ///////////////////////////////////////////////////////////////////////
  // Frequency: ticks per second
  QueryPerformanceFrequency(&frequency);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // Meassurement time
  // start timer
  QueryPerformanceCounter(&time_start);
  // execute function to meassure 'repetitions' times
  for (u32 rep = 0; rep < repetitions; ++rep) {
    c_vector->ops_->insertAt(c_vector, data[rep], 14, rand() % knTimes);
  }
  // stop timer
  QueryPerformanceCounter(&time_end);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("Vector ||| InsertAt\n");
  printf("Elapsed time: %f ms\n", elapsed_time);
  ///////////////////////////////////////////////////////////////////////
  // compute the average time
  double average_time = elapsed_time / repetitions;
  printf("Average time: %f ms\n\n", average_time);
  ///////////////////////////////////////////////////////////////////////
}

void VectorExtractAt() {
  LARGE_INTEGER frequency;				// ticks per second
  LARGE_INTEGER  time_start, time_end;    // ticks in interval
  double elapsed_time = 0.0f;
  u32 repetitions = knTimes;

  ///////////////////////////////////////////////////////////////////////
  // Frequency: ticks per second
  QueryPerformanceFrequency(&frequency);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // Meassurement time
  // start timer
  QueryPerformanceCounter(&time_start);
  // execute function to meassure 'repetitions' times
  for (u32 rep = 0; rep < repetitions; ++rep) {
    c_vector->ops_->extractAt(c_vector, rand()%c_vector->ops_->length(c_vector));
  }
  // stop timer
  QueryPerformanceCounter(&time_end);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("Vector ||| ExtractAt\n");
  printf("Elapsed time: %f ms\n", elapsed_time);
  ///////////////////////////////////////////////////////////////////////
  // compute the average time
  double average_time = elapsed_time / repetitions;
  printf("Average time: %f ms\n", average_time);
  ///////////////////////////////////////////////////////////////////////
}


void VectorContac() {
  LARGE_INTEGER frequency;				// ticks per second
  LARGE_INTEGER  time_start, time_end;    // ticks in interval
  double elapsed_time = 0.0f;
  u32 repetitions = knTimes;

  ///////////////////////////////////////////////////////////////////////
  // Frequency: ticks per second
  QueryPerformanceFrequency(&frequency);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // Meassurement time
  // start timer
  QueryPerformanceCounter(&time_start);
  // execute function to meassure 'repetitions' times
  for (u32 rep = 0; rep < repetitions; ++rep) {
    concat_vector1->ops_->concat(concat_vector1, concat_vector2);
  }
  // stop timer
  QueryPerformanceCounter(&time_end);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("\nVector ||| Concat\n");
  printf("Elapsed time: %f ms\n", elapsed_time);
  ///////////////////////////////////////////////////////////////////////
  // compute the average time
  double average_time = elapsed_time / repetitions;
  printf("\nAverage time: %f ms\n", average_time);
  ///////////////////////////////////////////////////////////////////////
}

void VectorComp() {
  VectorInsertFirst();
  VectorExtractFirst();
  VectorInsertLast();
  VectorExtractLast();
  VectorInsertAt();
  VectorExtractAt();
  VectorContac();

}


//List

void ListInsertFirst() {
  LARGE_INTEGER frequency;				// ticks per second
  LARGE_INTEGER  time_start, time_end;    // ticks in interval
  double elapsed_time = 0.0f;
  u32 repetitions = knTimes;

  ///////////////////////////////////////////////////////////////////////
  // Frequency: ticks per second
  QueryPerformanceFrequency(&frequency);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // Meassurement time
  // start timer
  QueryPerformanceCounter(&time_start);
  // execute function to meassure 'repetitions' times
  for (u32 rep = 0; rep < repetitions; ++rep) {
    c_list->ops_->insertFirst(c_list, data[rep], 14);
  }
  // stop timer
  QueryPerformanceCounter(&time_end);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("List ||| InsertFirst\n");
  printf("Elapsed time: %f ms\n", elapsed_time);
  ///////////////////////////////////////////////////////////////////////
  // compute the average time
  double average_time = elapsed_time / repetitions;
  printf("Average time: %f ms\n\n", average_time);
  ///////////////////////////////////////////////////////////////////////
}

void ListExtractFirst() {
  LARGE_INTEGER frequency;				// ticks per second
  LARGE_INTEGER  time_start, time_end;    // ticks in interval
  double elapsed_time = 0.0f;
  u32 repetitions = knTimes;

  ///////////////////////////////////////////////////////////////////////
  // Frequency: ticks per second
  QueryPerformanceFrequency(&frequency);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // Meassurement time
  // start timer
  QueryPerformanceCounter(&time_start);
  // execute function to meassure 'repetitions' times
  for (u32 rep = 0; rep < repetitions; ++rep) {
    c_list->ops_->extractFirst(c_list);
  }
  // stop timer
  QueryPerformanceCounter(&time_end);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("List ||| ExctractFirst\n");
  printf("Elapsed time: %f ms\n", elapsed_time);
  ///////////////////////////////////////////////////////////////////////
  // compute the average time
  double average_time = elapsed_time / repetitions;
  printf("Average time: %f ms\n\n", average_time);
  ///////////////////////////////////////////////////////////////////////
}


void ListInsertLast() {
  LARGE_INTEGER frequency;				// ticks per second
  LARGE_INTEGER  time_start, time_end;    // ticks in interval
  double elapsed_time = 0.0f;
  u32 repetitions = knTimes;

  ///////////////////////////////////////////////////////////////////////
  // Frequency: ticks per second
  QueryPerformanceFrequency(&frequency);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // Meassurement time
  // start timer
  QueryPerformanceCounter(&time_start);
  // execute function to meassure 'repetitions' times
  for (u32 rep = 0; rep < repetitions; ++rep) {
    c_list->ops_->insertLast(c_list, data[rep], 14);
  }
  // stop timer
  QueryPerformanceCounter(&time_end);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("List ||| InsertLast\n");
  printf("Elapsed time: %f ms\n", elapsed_time);
  ///////////////////////////////////////////////////////////////////////
  // compute the average time
  double average_time = elapsed_time / repetitions;
  printf("Average time: %f ms\n\n", average_time);
  ///////////////////////////////////////////////////////////////////////
}
     
void ListExtractLast() {
  LARGE_INTEGER frequency;				// ticks per second
  LARGE_INTEGER  time_start, time_end;    // ticks in interval
  double elapsed_time = 0.0f;
  u32 repetitions = knTimes;

  ///////////////////////////////////////////////////////////////////////
  // Frequency: ticks per second
  QueryPerformanceFrequency(&frequency);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // Meassurement time
  // start timer
  QueryPerformanceCounter(&time_start);
  // execute function to meassure 'repetitions' times
  for (u32 rep = 0; rep < repetitions; ++rep) {
    c_list->ops_->extractLast(c_list);
  }
  // stop timer
  QueryPerformanceCounter(&time_end);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("List ||| ExctractLast\n");
  printf("Elapsed time: %f ms\n", elapsed_time);
  ///////////////////////////////////////////////////////////////////////
  // compute the average time
  double average_time = elapsed_time / repetitions;
  printf("Average time: %f ms\n\n", average_time);
  ///////////////////////////////////////////////////////////////////////
}


void ListInsertAt() {
  LARGE_INTEGER frequency;				// ticks per second
  LARGE_INTEGER  time_start, time_end;    // ticks in interval
  double elapsed_time = 0.0f;
  u32 repetitions = knTimes;

  ///////////////////////////////////////////////////////////////////////
  // Frequency: ticks per second
  QueryPerformanceFrequency(&frequency);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // Meassurement time
  // start timer
  QueryPerformanceCounter(&time_start);
  // execute function to meassure 'repetitions' times
  for (u32 rep = 0; rep < repetitions; ++rep) {
    c_list->ops_->insertAt(c_list, data[rep], 14, rand() % knTimes);
   
  }
  // stop timer
  QueryPerformanceCounter(&time_end);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("List ||| InsertAt\n");
  printf("Elapsed time: %f ms\n", elapsed_time);
  ///////////////////////////////////////////////////////////////////////
  // compute the average time
  double average_time = elapsed_time / repetitions;
  printf("Average time: %f ms\n\n", average_time);
  ///////////////////////////////////////////////////////////////////////
}
     
void ListExtractAt() {
  LARGE_INTEGER frequency;				// ticks per second
  LARGE_INTEGER  time_start, time_end;    // ticks in interval
  double elapsed_time = 0.0f;
  u32 repetitions = knTimes;

  ///////////////////////////////////////////////////////////////////////
  // Frequency: ticks per second
  QueryPerformanceFrequency(&frequency);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // Meassurement time
  // start timer
  QueryPerformanceCounter(&time_start);
  // execute function to meassure 'repetitions' times
  for (u32 rep = 0; rep < repetitions; ++rep) {
    //c_list->ops_->extractAt(c_list, rand()%knTimes);
    c_list->ops_->extractAt(c_list, rand()%c_list->ops_->length(c_list));
    //c_list->ops_->extractAt(c_list, (c_list->ops_->length(c_list)));
    //c_list->ops_->extractAt(c_list, (c_list->ops_->length(c_list)));
    //c_list->ops_->extractAt(c_list, 1);
  }
  // stop timer
  QueryPerformanceCounter(&time_end);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("List ||| ExtractAt\n");
  printf("Elapsed time: %f ms\n", elapsed_time);
  ///////////////////////////////////////////////////////////////////////
  // compute the average time
  double average_time = elapsed_time / repetitions;
  printf("Average time: %f ms\n\n", average_time);
  ///////////////////////////////////////////////////////////////////////
}


void ListContac() {
  LARGE_INTEGER frequency;				// ticks per second
  LARGE_INTEGER  time_start, time_end;    // ticks in interval
  double elapsed_time = 0.0f;
  u32 repetitions = knTimes;

  ///////////////////////////////////////////////////////////////////////
  // Frequency: ticks per second
  QueryPerformanceFrequency(&frequency);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // Meassurement time
  // start timer
  QueryPerformanceCounter(&time_start);
  // execute function to meassure 'repetitions' times
  for (u32 rep = 0; rep < repetitions; ++rep) {
    c_list->ops_->concat(c_list, concat_list);
  }
  // stop timer
  QueryPerformanceCounter(&time_end);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("List ||| Concat\n");
  printf("Elapsed time: %f ms\n", elapsed_time);
  ///////////////////////////////////////////////////////////////////////
  // compute the average time
  double average_time = elapsed_time / repetitions;
  printf("Average time: %f ms\n\n", average_time);
  ///////////////////////////////////////////////////////////////////////
}

void ListComp() {
  ListInsertFirst();
  ListExtractFirst();
  //c_list->ops_->reset(c_list);
  ListInsertLast();
  ListExtractLast();
  //c_list->ops_->reset(c_list);
  ListInsertAt();
  ListExtractAt();
  //c_list->ops_->reset(c_list);
  ListContac();
  c_list->ops_->reset(c_list);
}



//DL_List

void DL_ListInsertFirst() {
  LARGE_INTEGER frequency;				// ticks per second
  LARGE_INTEGER  time_start, time_end;    // ticks in interval
  double elapsed_time = 0.0f;
  u32 repetitions = knTimes;

  ///////////////////////////////////////////////////////////////////////
  // Frequency: ticks per second
  QueryPerformanceFrequency(&frequency);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // Meassurement time
  // start timer
  QueryPerformanceCounter(&time_start);
  // execute function to meassure 'repetitions' times
  for (u32 rep = 0; rep < repetitions; ++rep) {
    c_dl_list->ops_->insertFirst(c_dl_list, data[rep], 14);
  }
  // stop timer
  QueryPerformanceCounter(&time_end);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("DL_List ||| InsertFirst\n");
  printf("Elapsed time: %f ms\n", elapsed_time);
  ///////////////////////////////////////////////////////////////////////
  // compute the average time
  double average_time = elapsed_time / repetitions;
  printf("Average time: %f ms\n\n", average_time);
  ///////////////////////////////////////////////////////////////////////
}
     
void DL_ListExtractFirst() {
  LARGE_INTEGER frequency;				// ticks per second
  LARGE_INTEGER  time_start, time_end;    // ticks in interval
  double elapsed_time = 0.0f;
  u32 repetitions = knTimes;

  ///////////////////////////////////////////////////////////////////////
  // Frequency: ticks per second
  QueryPerformanceFrequency(&frequency);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // Meassurement time
  // start timer
  QueryPerformanceCounter(&time_start);
  // execute function to meassure 'repetitions' times
  for (u32 rep = 0; rep < repetitions; ++rep) {
    c_dl_list->ops_->extractFirst(c_dl_list);
  }
  // stop timer
  QueryPerformanceCounter(&time_end);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("DL_List ||| ExtractFirst\n");
  printf("Elapsed time: %f ms\n", elapsed_time);
  ///////////////////////////////////////////////////////////////////////
  // compute the average time
  double average_time = elapsed_time / repetitions;
  printf("Average time: %f ms\n\n", average_time);
  ///////////////////////////////////////////////////////////////////////
}
     
     
void DL_ListInsertLast() {
  LARGE_INTEGER frequency;				// ticks per second
  LARGE_INTEGER  time_start, time_end;    // ticks in interval
  double elapsed_time = 0.0f;
  u32 repetitions = knTimes;

  ///////////////////////////////////////////////////////////////////////
  // Frequency: ticks per second
  QueryPerformanceFrequency(&frequency);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // Meassurement time
  // start timer
  QueryPerformanceCounter(&time_start);
  // execute function to meassure 'repetitions' times
  for (u32 rep = 0; rep < repetitions; ++rep) {
    c_dl_list->ops_->insertLast(c_dl_list, data[rep], 14);
  }
  // stop timer
  QueryPerformanceCounter(&time_end);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("DL_List ||| InsertLast\n");
  printf("Elapsed time: %f ms\n", elapsed_time);
  ///////////////////////////////////////////////////////////////////////
  // compute the average time
  double average_time = elapsed_time / repetitions;
  printf("Average time: %f ms\n\n", average_time);
  ///////////////////////////////////////////////////////////////////////
}
     
void DL_ListExtractLast() {
  LARGE_INTEGER frequency;				// ticks per second
  LARGE_INTEGER  time_start, time_end;    // ticks in interval
  double elapsed_time = 0.0f;
  u32 repetitions = knTimes;

  ///////////////////////////////////////////////////////////////////////
  // Frequency: ticks per second
  QueryPerformanceFrequency(&frequency);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // Meassurement time
  // start timer
  QueryPerformanceCounter(&time_start);
  // execute function to meassure 'repetitions' times
  for (u32 rep = 0; rep < repetitions; ++rep) {
    c_dl_list->ops_->extractLast(c_dl_list);
  }
  // stop timer
  QueryPerformanceCounter(&time_end);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("DL_List ||| ExtractLast\n");
  printf("Elapsed time: %f ms\n", elapsed_time);
  ///////////////////////////////////////////////////////////////////////
  // compute the average time
  double average_time = elapsed_time / repetitions;
  printf("Average time: %f ms\n\n", average_time);
  ///////////////////////////////////////////////////////////////////////
}
     
     
void DL_ListInsertAt() {
  LARGE_INTEGER frequency;				// ticks per second
  LARGE_INTEGER  time_start, time_end;    // ticks in interval
  double elapsed_time = 0.0f;
  u32 repetitions = knTimes;

  ///////////////////////////////////////////////////////////////////////
  // Frequency: ticks per second
  QueryPerformanceFrequency(&frequency);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // Meassurement time
  // start timer
  QueryPerformanceCounter(&time_start);
  // execute function to meassure 'repetitions' times
  for (u32 rep = 0; rep < repetitions; ++rep) {
    c_dl_list->ops_->insertAt(c_dl_list, data[rep], 14, rand() % knTimes);
  }
  // stop timer
  QueryPerformanceCounter(&time_end);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("DL_List ||| InsertAt\n");
  printf("Elapsed time: %f ms\n", elapsed_time);
  ///////////////////////////////////////////////////////////////////////
  // compute the average time
  double average_time = elapsed_time / repetitions;
  printf("Average time: %f ms\n\n", average_time);
  ///////////////////////////////////////////////////////////////////////
}
     
void DL_ListExtractAt() {
  LARGE_INTEGER frequency;				// ticks per second
  LARGE_INTEGER  time_start, time_end;    // ticks in interval
  double elapsed_time = 0.0f;
  u32 repetitions = knTimes;

  ///////////////////////////////////////////////////////////////////////
  // Frequency: ticks per second
  QueryPerformanceFrequency(&frequency);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // Meassurement time
  // start timer
  QueryPerformanceCounter(&time_start);
  // execute function to meassure 'repetitions' times
  for (u32 rep = 0; rep < repetitions; ++rep) {
    c_dl_list->ops_->extractAt(c_dl_list, rand() % c_dl_list->ops_->length(c_dl_list));
  }
  // stop timer
  QueryPerformanceCounter(&time_end);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("DL_List ||| ExtractAt\n");
  printf("Elapsed time: %f ms\n", elapsed_time);
  ///////////////////////////////////////////////////////////////////////
  // compute the average time
  double average_time = elapsed_time / repetitions;
  printf("Average time: %f ms\n\n", average_time);
  ///////////////////////////////////////////////////////////////////////
}


void DL_ListContac() {
  LARGE_INTEGER frequency;				// ticks per second
  LARGE_INTEGER  time_start, time_end;    // ticks in interval
  double elapsed_time = 0.0f;
  u32 repetitions = knTimes;

  ///////////////////////////////////////////////////////////////////////
  // Frequency: ticks per second
  QueryPerformanceFrequency(&frequency);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // Meassurement time
  // start timer
  QueryPerformanceCounter(&time_start);
  // execute function to meassure 'repetitions' times
  for (u32 rep = 0; rep < repetitions; ++rep) {
    c_dl_list->ops_->concat(c_dl_list, concat_dl_list);
  }
  // stop timer
  QueryPerformanceCounter(&time_end);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("DL_List ||| Concat\n");
  printf("Elapsed time: %f ms\n", elapsed_time);
  ///////////////////////////////////////////////////////////////////////
  // compute the average time
  double average_time = elapsed_time / repetitions;
  printf("Average time: %f ms\n", average_time);
  ///////////////////////////////////////////////////////////////////////
}

void DL_ListComp() {
  DL_ListInsertFirst();
  DL_ListExtractFirst();
  DL_ListInsertLast();
  DL_ListExtractLast();
  DL_ListInsertAt();
  DL_ListExtractAt();
  c_dl_list->ops_->reset(c_dl_list);
  DL_ListContac();
  c_dl_list->ops_->reset(c_dl_list);
}


void TESTBASE_mm_free() {
  //Comparatives
  c_vector->ops_->softReset(c_vector);
  c_vector->ops_->destroy(c_vector);

  c_list->ops_->softReset(c_list);
  c_list->ops_->destroy(c_list);

  c_dl_list->ops_->softReset(c_dl_list);
  c_dl_list->ops_->destroy(c_dl_list);

  concat_vector1->ops_->reset(concat_vector1);
  concat_vector1->ops_->destroy(concat_vector1);

  concat_vector2->ops_->softReset(concat_vector2);
  concat_vector2->ops_->destroy(concat_vector2);

  concat_list->ops_->softReset(concat_list);
  concat_list->ops_->destroy(concat_list);

  concat_dl_list->ops_->softReset(concat_dl_list);
  concat_dl_list->ops_->destroy(concat_dl_list);


  for (int i = 0; i < knTimes; ++i) {
    MM->free(data[i]);
  }
}


int main(int argc, char** argv) {
	srand(time(NULL));

	TESTBASE_generateDataForComparative();

  VectorComp();
  ListComp();
  DL_ListComp();


  TESTBASE_mm_free();
	MM->status();
	printf("Press ENTER to continue\n");
	getchar();
	MM->destroy();
	return 0;
}
