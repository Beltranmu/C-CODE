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

//Sorting
const int ksorting1 = 250;
const int ksorting2 = 500;
const int ksorting3 = 1000;
const int ksorting4 = 2000;
const int knumsortingraange = 100000;
const bool kprintResults = false;

int* sorting1data;
int* sorting2data;
int* sorting3data;


List* sorting_list1 = NULL;
List* sorting_list2 = NULL;
DL_List* sorting_dl_list1 = NULL;
DL_List* sorting_dl_list2 = NULL;


void TESTBASE_generateDataForComparative() {

  sorting1data = MM->malloc(sizeof(int)*ksorting1);
  sorting2data = MM->malloc(sizeof(int)*ksorting2);
  sorting3data = MM->malloc(sizeof(int)*ksorting3);
  int n_signed = 1; // 1 ->positive  -1 -> negative

  for (int i = 0; i < ksorting1; ++i) {
    n_signed = 1;
    if (rand() % 2 == 0) {
      n_signed = -1;
    }
    *(sorting1data + i) = n_signed * rand()%100000;
  }

  for (int i = 0; i < ksorting2; ++i) {
    n_signed = 1;
    if (rand() % 2 == 0) {
      n_signed = -1;
    }
    *(sorting2data + i) = n_signed * rand() % 100000;
  }

  for (int i = 0; i < ksorting3; ++i) {
    n_signed = 1;
    if (rand() % 2 == 0) {
      n_signed = -1;
    }
    *(sorting3data + i) = n_signed * rand() % 100000;
  }

 
  if (NULL == sorting_list1) {
    sorting_list1 = LIST_create(ksorting4);
  }
  if (NULL == sorting_list2) {
    sorting_list2 = LIST_create(ksorting4);
  }

  if (NULL == sorting_dl_list1) {
    sorting_dl_list1 = DL_LIST_create(ksorting4);
  }
  if (NULL == sorting_dl_list2) {
    sorting_dl_list2 = DL_LIST_create(ksorting4);
  }

}


void SwapListElements(List* list, int position1, int position2) {

  void* data1 = list->ops_->at(list,position1);
  void* data2 = list->ops_->at(list,position2);
  
  MemoryNode* iterator = list->head_;
  for (int i = 0; i < position1; ++i) {
    iterator = list->head_->ops_->next(iterator);
  }
  iterator->ops_->setData(iterator, (void*)data2, 4);

  iterator = list->head_;
  for (int i = 0; i < position2; ++i) {
    iterator = list->head_->ops_->next(iterator);
  }
  iterator->ops_->setData(iterator, (void*)data1, 4);
    
}

void DL_SwapListElements(DL_List* list, int position1, int position2) {

  void* data1 = list->ops_->at(list, position1);
  void* data2 = list->ops_->at(list, position2);

  MemoryNode* iterator = list->head_;
  int list_lenght = list->ops_->length(list);
  if (position1 < list_lenght * 2) {
    for (int i = 0; i < position1; ++i) {
      iterator = list->head_->ops_->next(iterator);
    }
  }
  else {
    iterator = list->tail_;
    for (int i = 0; i < list_lenght - position1; ++i) {
      iterator = list->head_->ops_->previous(iterator);
    }
  }
  iterator->ops_->setData(iterator, (void*)data2, 4);

  iterator = list->head_;
  if (position2 < list_lenght * 2) {
    for (int i = 0; i < position2; ++i) {
      iterator = list->head_->ops_->next(iterator);
    }
  }
  else {
    iterator = list->tail_;
    for (int i = 0; i < list_lenght - position2; ++i) {
      iterator = list->head_->ops_->previous(iterator);
    }
  }
  iterator->ops_->setData(iterator, (void*)data1, 4);

}


int DL_Partition(DL_List *list, int first, int last) {
  int pivot = (int*)list->ops_->at(list, last); // pivot value
  int i = first;  // Index of smaller element

  for (int j = first; j < last; j++)
  {
    int j_element = (int*)list->ops_->at(list, j);
    if (j_element <= pivot) {
      DL_SwapListElements(list, i, j);
      ++i;   // increment index of smaller element
    }
  }
  // Swap the pivot to its place
  DL_SwapListElements(list, i, last);
  return i;
}

int Partition(List *list, int first, int last) {
  int pivot = (int*)list->ops_->at(list, last); // pivot value
  int i = first;  // Index of smaller element

  for (int j = first; j < last; j++)
  {
    int j_element = (int*)list->ops_->at(list, j);
    if (j_element <= pivot){
      SwapListElements(list,i, j);
      ++i;   // increment index of smaller element
    }
  }
  // Swap the pivot to its place
  SwapListElements(list, i, last);
  return i;
}

// Bubble Sort
void bubbleSort(List* list, u16 n)
{
  bool swapped;

  for (int i = 0; i < n - 1; ++i) {

    swapped = false;

    for (int j = 0; j < n - i - 1; ++j) {
      
      int botijo = (int*)list->ops_->at(list, j);
      int paella = (int*)list->ops_->at(list, j + 1);

      if (botijo > paella) {

        SwapListElements(list, j, j+1);
        swapped = true;

      }

    }

    if (false == swapped)
      break;

  }
}

// Doubly-Linked Bubble Sort
void DL_bubbleSort(DL_List* list, u16 n)
{
  bool swapped;

  for (int i = 0; i < n - 1; ++i) {

    swapped = false;

    for (int j = 0; j < n - i - 1; ++j) {

      int botijo = (int*)list->ops_->at(list, j);
      int paella = (int*)list->ops_->at(list, j + 1);

      if (botijo > paella) {
        
        DL_SwapListElements(list, j, j + 1);
        swapped = true;

      }

    }

    if (false == swapped)
      break;

  }
}

// Comb Sort
int getNextGap(int gap) {

  gap = (gap * 10) * 0.076923;
  if (gap < 1)
    return 1;
  return gap;

}

void combSort(List* list, int n) {

  int gap = n;
  bool swapped = true;

  while (gap != 1 || swapped) {

    gap = getNextGap(gap);
    swapped = false;

    for (int i = 0; i < n - gap; ++i) {

      int botijo = (int*)list->ops_->at(list, i);
      int paella = (int*)list->ops_->at(list, i + gap);

      if (botijo > paella) {
        SwapListElements(list, i, i + gap);
        swapped = true;
      }

    }

  }

}

// DL Comb Sort
void DL_combSort(DL_List* list, int n) {

  int gap = n;
  bool swapped = true;

  while (gap != 1 || swapped) {

    gap = getNextGap(gap);
    swapped = false;

    for (int i = 0; i < n - gap; ++i) {

      int botijo = (int*)list->ops_->at(list, i);
      int paella = (int*)list->ops_->at(list, i + gap);

      if (botijo > paella) {
        DL_SwapListElements(list, i, i + gap);
        swapped = true;
      }

    }

  }

}

void SortingTest250() {

  LARGE_INTEGER frequency;				// ticks per second
  LARGE_INTEGER  time_start, time_end;    // ticks in interval
  double elapsed_time = 0.0f;
  //-------------------------------------------
  // 250
  //-------------------------------------------
  printf("---------------------------------------------\n250\n---------------------------------------------\n\n");

  for (int i = 0; i < ksorting1; ++i) {
    sorting_list1->ops_->insertLast(sorting_list1, sorting1data[i], 4);
    sorting_list2->ops_->insertLast(sorting_list2, sorting1data[i], 4);

    sorting_dl_list1->ops_->insertLast(sorting_dl_list1, sorting1data[i], 4);
    sorting_dl_list2->ops_->insertLast(sorting_dl_list2, sorting1data[i], 4);
  }

  //LIST

  if (kprintResults) {
    for (int i = 0; i < ksorting1; ++i) {
      printf("%07d ||| %07d ||| %07d||| %07d\n",
        sorting_list1->ops_->at(sorting_list1, i),
        sorting_list2->ops_->at(sorting_list2, i),
        sorting_dl_list1->ops_->at(sorting_dl_list1, i),
        sorting_dl_list2->ops_->at(sorting_dl_list2, i));
    }
    printf("\n\n\n");
  }
  // Quick Sorting
  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter(&time_start);

  //QuickSorting(sorting_list1, 0, sorting_list1->ops_->length(sorting_list1) - 1);  
  bubbleSort(sorting_list1, sorting_list1->length_);
  // stop timer
  QueryPerformanceCounter(&time_end);
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("QuickSorting List 250\n");
  printf("Elapsed time: %f ms\n", elapsed_time);
  double average_time = elapsed_time / ksorting1;
  printf("Average time(250 elements): %f ms\n\n", average_time);
 

  // Cocktail shaker sort
  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter(&time_start);

  Cocktail_List(sorting_list2, sorting_list2->ops_->length(sorting_list2));
  
  // stop timer
  QueryPerformanceCounter(&time_end);
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("CocktailSort List 250\n");
  printf("Elapsed time: %f ms\n", elapsed_time);
  average_time = elapsed_time / ksorting1;
  printf("Average time(250 elements): %f ms\n\n", average_time);
    
 
  //DL_LIST 

  // Quick Sorting
  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter(&time_start);

  DL_QuickSorting(sorting_dl_list1, 0, sorting_dl_list1->ops_->length(sorting_dl_list1) - 1);
  // stop timer
  QueryPerformanceCounter(&time_end);
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("QuickSorting DL_List 250\n");
  printf("Elapsed time: %f ms\n", elapsed_time);
   average_time = elapsed_time / ksorting1;
  printf("Average time(250 elements): %f ms\n\n", average_time);


  // Cocktail shaker sort
  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter(&time_start);

  DL_Cocktail_List(sorting_dl_list2, sorting_dl_list2->ops_->length(sorting_dl_list2));

  // stop timer
  QueryPerformanceCounter(&time_end);
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("CocktailSort DL_List 250\n");
  printf("Elapsed time: %f ms\n", elapsed_time);
  average_time = elapsed_time / ksorting1;
  printf("Average time(250 elements): %f ms\n\n", average_time);


  if (kprintResults) {
    printf("\n\n\nDespues\n\n");
    for (int i = 0; i < ksorting1; ++i) {
      printf("%07d ||| %07d ||| %07d||| %07d\n",
        sorting_list1->ops_->at(sorting_list1, i),
        sorting_list2->ops_->at(sorting_list2, i),
        sorting_dl_list1->ops_->at(sorting_dl_list1, i),
        sorting_dl_list2->ops_->at(sorting_dl_list2, i));
    }
    printf("\n\n\n");
  }
  printf("____________________________________\n\n");
  //Resets 
  sorting_list1->ops_->softReset(sorting_list1);
  sorting_list2->ops_->softReset(sorting_list2);
  sorting_dl_list1->ops_->softReset(sorting_dl_list1);
  sorting_dl_list2->ops_->softReset(sorting_dl_list2);  
}

void SortingTest500() {

  LARGE_INTEGER frequency;				// ticks per second
  LARGE_INTEGER  time_start, time_end;    // ticks in interval
  double elapsed_time = 0.0f;
  //-------------------------------------------
  // 500
  //-------------------------------------------
  printf("---------------------------------------------\n500\n---------------------------------------------\n\n");

  for (int i = 0; i < ksorting2; ++i) {
    sorting_list1->ops_->insertLast(sorting_list1, sorting2data[i], 4);
    sorting_list2->ops_->insertLast(sorting_list2, sorting2data[i], 4);

    sorting_dl_list1->ops_->insertLast(sorting_dl_list1, sorting2data[i], 4);
    sorting_dl_list2->ops_->insertLast(sorting_dl_list2, sorting2data[i], 4);
  }

  //LIST


  if (kprintResults) {
    printf("\n\n\nDespues\n\n");
    for (int i = 0; i < ksorting2; ++i) {
      printf("%07d ||| %07d ||| %07d||| %07d\n",
        sorting_list1->ops_->at(sorting_list1, i),
        sorting_list2->ops_->at(sorting_list2, i),
        sorting_dl_list1->ops_->at(sorting_dl_list1, i),
        sorting_dl_list2->ops_->at(sorting_dl_list2, i));
    }
    printf("\n\n\n");
  }
  // Quick Sorting
  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter(&time_start);

  QuickSorting(sorting_list1, 0, sorting_list1->ops_->length(sorting_list1) - 1);  
  // stop timer
  QueryPerformanceCounter(&time_end);
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("QuickSorting List 500\n");
  printf("Elapsed time: %f ms\n", elapsed_time);
  double average_time = elapsed_time / ksorting2;
  printf("Average time(500 elements): %f ms\n\n", average_time);


  // Cocktail shaker sort
  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter(&time_start);

  Cocktail_List(sorting_list2, sorting_list2->ops_->length(sorting_list2));

  // stop timer
  QueryPerformanceCounter(&time_end);
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("CocktailSort List 500\n");
  printf("Elapsed time: %f ms\n", elapsed_time);
  average_time = elapsed_time / ksorting2;
  printf("Average time(500 elements): %f ms\n\n", average_time);


  //DL_LIST 

  // Quick Sorting
  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter(&time_start);

  DL_QuickSorting(sorting_dl_list1, 0, sorting_dl_list1->ops_->length(sorting_dl_list1)  - 1);
  // stop timer
  QueryPerformanceCounter(&time_end);
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("QuickSorting DL_List 500\n");
  printf("Elapsed time: %f ms\n", elapsed_time);
  average_time = elapsed_time / ksorting2;
  printf("Average time(500 elements): %f ms\n\n", average_time);


  // Cocktail shaker sort
  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter(&time_start);

  DL_Cocktail_List(sorting_dl_list2, sorting_dl_list2->ops_->length(sorting_dl_list2));

  // stop timer
  QueryPerformanceCounter(&time_end);
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("CocktailSort DL_List 500\n");
  printf("Elapsed time: %f ms\n", elapsed_time);
  average_time = elapsed_time / ksorting2;
  printf("Average time(500 elements): %f ms\n\n", average_time);


  if (kprintResults) {
    printf("\n\n\nDespues\n\n");
    for (int i = 0; i < ksorting2; ++i) {
      printf("%07d ||| %07d ||| %07d||| %07d\n",
        sorting_list1->ops_->at(sorting_list1, i),
        sorting_list2->ops_->at(sorting_list2, i),
        sorting_dl_list1->ops_->at(sorting_dl_list1, i),
        sorting_dl_list2->ops_->at(sorting_dl_list2, i));
    }
    printf("\n\n\n");
  }
  printf("____________________________________\n\n");
  //Resets 
  sorting_list1->ops_->softReset(sorting_list1);
  sorting_list2->ops_->softReset(sorting_list2);
  sorting_dl_list1->ops_->softReset(sorting_dl_list1);
  sorting_dl_list2->ops_->softReset(sorting_dl_list2);
}

void SortingTest1000() {

  LARGE_INTEGER frequency;				// ticks per second
  LARGE_INTEGER  time_start, time_end;    // ticks in interval
  double elapsed_time = 0.0f;
  //-------------------------------------------
  // 1000
  //-------------------------------------------
  printf("---------------------------------------------\n1000\n---------------------------------------------\n\n");

  for (int i = 0; i < ksorting3; ++i) {
    sorting_list1->ops_->insertLast(sorting_list1, sorting3data[i], 4);
    sorting_list2->ops_->insertLast(sorting_list2, sorting3data[i], 4);

    sorting_dl_list1->ops_->insertLast(sorting_dl_list1, sorting3data[i], 4);
    sorting_dl_list2->ops_->insertLast(sorting_dl_list2, sorting3data[i], 4);
  }

  //LIST


  if (kprintResults) {
    for (int i = 0; i < ksorting3; ++i) {
      printf("%07d ||| %07d ||| %07d||| %07d\n",
        sorting_list1->ops_->at(sorting_list1, i),
        sorting_list2->ops_->at(sorting_list2, i),
        sorting_dl_list1->ops_->at(sorting_dl_list1, i),
        sorting_dl_list2->ops_->at(sorting_dl_list2, i));
    }
    printf("\n\n\n");
  }
  // Quick Sorting
  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter(&time_start);

  QuickSorting(sorting_list1, 0, sorting_list1->ops_->length(sorting_list1) - 1);
  // stop timer
  QueryPerformanceCounter(&time_end);
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("QuickSorting List 1000\n");
  printf("Elapsed time: %f ms\n", elapsed_time);
  double average_time = elapsed_time / ksorting3;
  printf("Average time(1000 elements): %f ms\n\n", average_time);


  // Cocktail shaker sort
  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter(&time_start);

  Cocktail_List(sorting_list2, sorting_list2->ops_->length(sorting_list2));

  // stop timer
  QueryPerformanceCounter(&time_end);
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("CocktailSort List 1000\n");
  printf("Elapsed time: %f ms\n", elapsed_time);
  average_time = elapsed_time / ksorting3;
  printf("Average time(1000 elements): %f ms\n\n", average_time);


  //DL_LIST 

  // Quick Sorting
  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter(&time_start);

  DL_QuickSorting(sorting_dl_list1, 0, sorting_dl_list1->ops_->length(sorting_dl_list1) - 1);
  // stop timer
  QueryPerformanceCounter(&time_end);
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("QuickSorting DL_List 1000\n");
  printf("Elapsed time: %f ms\n", elapsed_time);
  average_time = elapsed_time / ksorting1;
  printf("Average time(1000 elements): %f ms\n\n", average_time);


  // Cocktail shaker sort
  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter(&time_start);

  DL_Cocktail_List(sorting_dl_list2, sorting_dl_list2->ops_->length(sorting_dl_list2));

  // stop timer
  QueryPerformanceCounter(&time_end);
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("CocktailSort DL_List 1000\n");
  printf("Elapsed time: %f ms\n", elapsed_time);
  average_time = elapsed_time / ksorting3;
  printf("Average time(1000 elements): %f ms\n\n", average_time);


  if (kprintResults) {
    printf("\n\n\nDespues\n\n");
    for (int i = 0; i < ksorting3; ++i) {
      printf("%07d ||| %07d ||| %07d||| %07d\n",
        sorting_list1->ops_->at(sorting_list1, i),
        sorting_list2->ops_->at(sorting_list2, i),
        sorting_dl_list1->ops_->at(sorting_dl_list1, i),
        sorting_dl_list2->ops_->at(sorting_dl_list2, i));
    }
    printf("\n\n\n");
  }
  printf("____________________________________\n\n");
  //Resets 
  sorting_list1->ops_->softReset(sorting_list1);
  sorting_list2->ops_->softReset(sorting_list2);
  sorting_dl_list1->ops_->softReset(sorting_dl_list1);
  sorting_dl_list2->ops_->softReset(sorting_dl_list2);
}



void Sorting2Test250() {

  LARGE_INTEGER frequency;				// ticks per second
  LARGE_INTEGER  time_start, time_end;    // ticks in interval
  double elapsed_time = 0.0f;
  //-------------------------------------------
  // 250
  //-------------------------------------------
  printf("---------------------------------------------\n250\n---------------------------------------------\n\n");

  for (int i = 0; i < ksorting1; ++i) {
    sorting_list1->ops_->insertLast(sorting_list1, sorting1data[i], 4);
    sorting_list2->ops_->insertLast(sorting_list2, sorting1data[i], 4);

    sorting_dl_list1->ops_->insertLast(sorting_dl_list1, sorting1data[i], 4);
    sorting_dl_list2->ops_->insertLast(sorting_dl_list2, sorting1data[i], 4);
  }

  //LIST

  if (kprintResults) {
    for (int i = 0; i < ksorting1; ++i) {
      printf("%07d ||| %07d ||| %07d ||| %07d\n",
        sorting_list1->ops_->at(sorting_list1, i),
        sorting_list2->ops_->at(sorting_list2, i),
        sorting_dl_list1->ops_->at(sorting_dl_list1, i),
        sorting_dl_list2->ops_->at(sorting_dl_list2, i));
    }
    printf("\n\n\n");
  }
  // Quick Sort
  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter(&time_start);
     
  QuickSorting(sorting_list1, 0, sorting_list1->ops_->length(sorting_list1) - 1);
  // stop timer
  QueryPerformanceCounter(&time_end);
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("Quick Sort List 250\n");
  printf("Elapsed time: %f ms\n", elapsed_time);
  double average_time = elapsed_time / ksorting1;
  printf("Average time(250 elements): %f ms\n\n", average_time);


  // Cocktail Sort
  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter(&time_start);

  Cocktail_List(sorting_list2, sorting_list2->ops_->length(sorting_list2));

  // stop timer
  QueryPerformanceCounter(&time_end);
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("Cocktail Sort List 250\n");
  printf("Elapsed time: %f ms\n", elapsed_time);
  average_time = elapsed_time / ksorting1;
  printf("Average time(250 elements): %f ms\n\n", average_time);


  //DL_LIST 

  // Quick Sort
  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter(&time_start);

  DL_QuickSorting(sorting_dl_list1, 0, sorting_dl_list1->ops_->length(sorting_dl_list1) - 1);
  // stop timer
  QueryPerformanceCounter(&time_end);
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("Quick Sort DL_List 250\n");
  printf("Elapsed time: %f ms\n", elapsed_time);
  average_time = elapsed_time / ksorting1;
  printf("Average time(250 elements): %f ms\n\n", average_time);

  
  // Cocktail Sort
  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter(&time_start);

  DL_Cocktail_List(sorting_dl_list2, sorting_dl_list2->ops_->length(sorting_dl_list2));

  // stop timer
  QueryPerformanceCounter(&time_end);
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("Cocktail Sort DL_List 250\n");
  printf("Elapsed time: %f ms\n", elapsed_time);
  average_time = elapsed_time / ksorting1;
  printf("Average time(250 elements): %f ms\n\n", average_time);


  if (kprintResults) {
    printf("\n\n\nDespues\n\n");
    for (int i = 0; i < ksorting1; ++i) {
      printf("%07d ||| %07d ||| %07d ||| %07d\n",
        sorting_list1->ops_->at(sorting_list1, i),
        sorting_list2->ops_->at(sorting_list2, i),
        sorting_dl_list1->ops_->at(sorting_dl_list1, i),
        sorting_dl_list2->ops_->at(sorting_dl_list2, i));
    }
    printf("\n\n\n");
  }

  //Resets 
  sorting_list1->ops_->softReset(sorting_list1);
  sorting_list2->ops_->softReset(sorting_list2);
  sorting_dl_list1->ops_->softReset(sorting_dl_list1);
  sorting_dl_list2->ops_->softReset(sorting_dl_list2);

  // -------------------------------------------------
  for (int i = 0; i < ksorting1; ++i) {
    sorting_list1->ops_->insertLast(sorting_list1, sorting1data[i], 4);
    sorting_list2->ops_->insertLast(sorting_list2, sorting1data[i], 4);

    sorting_dl_list1->ops_->insertLast(sorting_dl_list1, sorting1data[i], 4);
    sorting_dl_list2->ops_->insertLast(sorting_dl_list2, sorting1data[i], 4);
  }

  //LIST

  if (kprintResults) {
    for (int i = 0; i < ksorting1; ++i) {
      printf("%07d ||| %07d ||| %07d ||| %07d\n",
        sorting_list1->ops_->at(sorting_list1, i),
        sorting_list2->ops_->at(sorting_list2, i),
        sorting_dl_list1->ops_->at(sorting_dl_list1, i),
        sorting_dl_list2->ops_->at(sorting_dl_list2, i));
    }
    printf("\n\n\n");
  }
  // Bubble Sorting
  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter(&time_start);

  bubbleSort(sorting_list1, sorting_list1->length_);
  // stop timer
  QueryPerformanceCounter(&time_end);
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("Bubble Sort List 250\n");
  printf("Elapsed time: %f ms\n", elapsed_time);
  average_time = elapsed_time / ksorting1;
  printf("Average time(250 elements): %f ms\n\n", average_time);


  // Comb Sort
  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter(&time_start);

  combSort(sorting_list2, sorting_list2->length_);

  // stop timer
  QueryPerformanceCounter(&time_end);
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("Comb Sort List 250\n");
  printf("Elapsed time: %f ms\n", elapsed_time);
  average_time = elapsed_time / ksorting1;
  printf("Average time(250 elements): %f ms\n\n", average_time);


  //DL_LIST 

  // Bubble Sorting
  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter(&time_start);

  DL_bubbleSort(sorting_dl_list1, sorting_dl_list1->length_);
  // stop timer
  QueryPerformanceCounter(&time_end);
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("Bubble Sort DL_List 250\n");
  printf("Elapsed time: %f ms\n", elapsed_time);
  average_time = elapsed_time / ksorting1;
  printf("Average time(250 elements): %f ms\n\n", average_time);


  // Comb Sort
  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter(&time_start);

  DL_combSort(sorting_dl_list2, sorting_dl_list2->length_);

  // stop timer
  QueryPerformanceCounter(&time_end);
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("Comb Sort DL_List 250\n");
  printf("Elapsed time: %f ms\n", elapsed_time);
  average_time = elapsed_time / ksorting1;
  printf("Average time(250 elements): %f ms\n\n", average_time);


  if (kprintResults) {
    printf("\n\n\nDespues\n\n");
    for (int i = 0; i < ksorting1; ++i) {
      printf("%07d ||| %07d ||| %07d||| %07d\n",
        sorting_list1->ops_->at(sorting_list1, i),
        sorting_list2->ops_->at(sorting_list2, i),
        sorting_dl_list1->ops_->at(sorting_dl_list1, i),
        sorting_dl_list2->ops_->at(sorting_dl_list2, i));
    }
    printf("\n\n\n");
  }
  printf("_____________________________________\n\n");
  //Resets 
  sorting_list1->ops_->softReset(sorting_list1);
  sorting_list2->ops_->softReset(sorting_list2);
  sorting_dl_list1->ops_->softReset(sorting_dl_list1);
  sorting_dl_list2->ops_->softReset(sorting_dl_list2);
}

void Sorting2Test500() {



  LARGE_INTEGER frequency;				// ticks per second
  LARGE_INTEGER  time_start, time_end;    // ticks in interval
  double elapsed_time = 0.0f;
  //-------------------------------------------
  // 500
  //-------------------------------------------
  printf("---------------------------------------------\n500\n---------------------------------------------\n\n");

  for (int i = 0; i < ksorting2; ++i) {
    sorting_list1->ops_->insertLast(sorting_list1, sorting2data[i], 4);
    sorting_list2->ops_->insertLast(sorting_list2, sorting2data[i], 4);

    sorting_dl_list1->ops_->insertLast(sorting_dl_list1, sorting2data[i], 4);
    sorting_dl_list2->ops_->insertLast(sorting_dl_list2, sorting2data[i], 4);
  }

  //LIST


  if (kprintResults) {
    printf("\n\n\nDespues\n\n");
    for (int i = 0; i < ksorting2; ++i) {
      printf("%07d ||| %07d ||| %07d||| %07d\n",
        sorting_list1->ops_->at(sorting_list1, i),
        sorting_list2->ops_->at(sorting_list2, i),
        sorting_dl_list1->ops_->at(sorting_dl_list1, i),
        sorting_dl_list2->ops_->at(sorting_dl_list2, i));
    }
    printf("\n\n\n");
  }
  // Quick Sorting
  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter(&time_start);

  QuickSorting(sorting_list1, 0, sorting_list1->ops_->length(sorting_list1) - 1);
  // stop timer
  QueryPerformanceCounter(&time_end);
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("QuickSorting List 500\n");
  printf("Elapsed time: %f ms\n", elapsed_time);
  double average_time = elapsed_time / ksorting2;
  printf("Average time(500 elements): %f ms\n\n", average_time);


  // Cocktail shaker sort
  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter(&time_start);

  Cocktail_List(sorting_list2, sorting_list2->ops_->length(sorting_list2));

  // stop timer
  QueryPerformanceCounter(&time_end);
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("CocktailSort List 500\n");
  printf("Elapsed time: %f ms\n", elapsed_time);
  average_time = elapsed_time / ksorting2;
  printf("Average time(500 elements): %f ms\n\n", average_time);


  //DL_LIST 

  // Quick Sorting
  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter(&time_start);

  DL_QuickSorting(sorting_dl_list1, 0, sorting_dl_list1->ops_->length(sorting_dl_list1) - 1);
  // stop timer
  QueryPerformanceCounter(&time_end);
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("QuickSorting DL_List 500\n");
  printf("Elapsed time: %f ms\n", elapsed_time);
  average_time = elapsed_time / ksorting2;
  printf("Average time(500 elements): %f ms\n\n", average_time);


  // Cocktail shaker sort
  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter(&time_start);

  DL_Cocktail_List(sorting_dl_list2, sorting_dl_list2->ops_->length(sorting_dl_list2));

  // stop timer
  QueryPerformanceCounter(&time_end);
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("CocktailSort DL_List 500\n");
  printf("Elapsed time: %f ms\n", elapsed_time);
  average_time = elapsed_time / ksorting2;
  printf("Average time(500 elements): %f ms\n\n", average_time);


  if (kprintResults) {
    printf("\n\n\nDespues\n\n");
    for (int i = 0; i < ksorting2; ++i) {
      printf("%07d ||| %07d ||| %07d||| %07d\n",
        sorting_list1->ops_->at(sorting_list1, i),
        sorting_list2->ops_->at(sorting_list2, i),
        sorting_dl_list1->ops_->at(sorting_dl_list1, i),
        sorting_dl_list2->ops_->at(sorting_dl_list2, i));
    }
    printf("\n\n\n");
  }

  //Resets 
  sorting_list1->ops_->softReset(sorting_list1);
  sorting_list2->ops_->softReset(sorting_list2);
  sorting_dl_list1->ops_->softReset(sorting_dl_list1);
  sorting_dl_list2->ops_->softReset(sorting_dl_list2);

 

  for (int i = 0; i < ksorting2; ++i) {
    sorting_list1->ops_->insertLast(sorting_list1, sorting2data[i], 4);
    sorting_list2->ops_->insertLast(sorting_list2, sorting2data[i], 4);

    sorting_dl_list1->ops_->insertLast(sorting_dl_list1, sorting2data[i], 4);
    sorting_dl_list2->ops_->insertLast(sorting_dl_list2, sorting2data[i], 4);
  }

  //LIST


  if (kprintResults) {
    printf("\n\n\nDespues\n\n");
    for (int i = 0; i < ksorting2; ++i) {
      printf("%07d ||| %07d ||| %07d ||| %07d\n",
        sorting_list1->ops_->at(sorting_list1, i),
        sorting_list2->ops_->at(sorting_list2, i),
        sorting_dl_list1->ops_->at(sorting_dl_list1, i),
        sorting_dl_list2->ops_->at(sorting_dl_list2, i));
    }
    printf("\n\n\n");
  }
  // Bubble Sorting
  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter(&time_start);
    
  bubbleSort(sorting_list1, sorting_list1->length_);
  // stop timer
  QueryPerformanceCounter(&time_end);
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("Bubble Sort List 500\n");
  printf("Elapsed time: %f ms\n", elapsed_time);
  average_time = elapsed_time / ksorting2;
  printf("Average time(500 elements): %f ms\n\n", average_time);

  
  // Comb Sort
  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter(&time_start);

  combSort(sorting_list2, sorting_list2->length_);

  // stop timer
  QueryPerformanceCounter(&time_end);
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("Comb Sort List 500\n");
  printf("Elapsed time: %f ms\n", elapsed_time);
  average_time = elapsed_time / ksorting2;
  printf("Average time(500 elements): %f ms\n\n", average_time);


  //DL_LIST 

  // Bubble Sorting
  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter(&time_start);

  DL_bubbleSort(sorting_dl_list1, sorting_dl_list1->length_);
  // stop timer
  QueryPerformanceCounter(&time_end);
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("Bubble Sort DL_List 500\n");
  printf("Elapsed time: %f ms\n", elapsed_time);
  average_time = elapsed_time / ksorting2;
  printf("Average time(500 elements): %f ms\n\n", average_time);

  
  // Comb Sort
  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter(&time_start);

  DL_combSort(sorting_dl_list2, sorting_dl_list2->length_);

  // stop timer
  QueryPerformanceCounter(&time_end);
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("Comb Sort DL_List 500\n");
  printf("Elapsed time: %f ms\n", elapsed_time);
  average_time = elapsed_time / ksorting2;
  printf("Average time(500 elements): %f ms\n\n", average_time);


  if (kprintResults) {
    printf("\n\n\nDespues\n\n");
    for (int i = 0; i < ksorting2; ++i) {
      printf("%07d ||| %07d ||| %07d ||| %07d\n",
        sorting_list1->ops_->at(sorting_list1, i),
        sorting_list2->ops_->at(sorting_list2, i),
        sorting_dl_list1->ops_->at(sorting_dl_list1, i),
        sorting_dl_list2->ops_->at(sorting_dl_list2, i));
    }
    printf("\n\n\n");
  }
  
  //Resets 
  sorting_list1->ops_->softReset(sorting_list1);
  sorting_list2->ops_->softReset(sorting_list2);
  sorting_dl_list1->ops_->softReset(sorting_dl_list1);
  sorting_dl_list2->ops_->softReset(sorting_dl_list2);
}

void Sorting2Test1000() {

  LARGE_INTEGER frequency;				// ticks per second
  LARGE_INTEGER  time_start, time_end;    // ticks in interval
  double elapsed_time = 0.0f;
  //-------------------------------------------
  // 1000
  //-------------------------------------------
  printf("---------------------------------------------\n1000\n---------------------------------------------\n\n");

  for (int i = 0; i < ksorting3; ++i) {
    sorting_list1->ops_->insertLast(sorting_list1, sorting3data[i], 4);
    sorting_list2->ops_->insertLast(sorting_list2, sorting3data[i], 4);

    sorting_dl_list1->ops_->insertLast(sorting_dl_list1, sorting3data[i], 4);
    sorting_dl_list2->ops_->insertLast(sorting_dl_list2, sorting3data[i], 4);
  }

  //LIST


  if (kprintResults) {
    for (int i = 0; i < ksorting3; ++i) {
      printf("%07d ||| %07d ||| %07d ||| %07d\n",
        sorting_list1->ops_->at(sorting_list1, i),
        sorting_list2->ops_->at(sorting_list2, i),
        sorting_dl_list1->ops_->at(sorting_dl_list1, i),
        sorting_dl_list2->ops_->at(sorting_dl_list2, i));
    }
    printf("\n\n\n");
  }
  // Quicksorting
  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter(&time_start);

  QuickSorting(sorting_list1, 0, sorting_list1->ops_->length(sorting_list1) - 1);
  // stop timer
  QueryPerformanceCounter(&time_end);
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("Quick Sort List 1000\n");
  printf("Elapsed time: %f ms\n", elapsed_time);
  double average_time = elapsed_time / ksorting3;
  printf("Average time(1000 elements): %f ms\n\n", average_time);

  
  // Cocktail
  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter(&time_start);

  Cocktail_List(sorting_list2, sorting_list2->ops_->length(sorting_list2));

  // stop timer
  QueryPerformanceCounter(&time_end);
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("Cocktail Sort List 1000\n");
  printf("Elapsed time: %f ms\n", elapsed_time);
  average_time = elapsed_time / ksorting3;
  printf("Average time(1000 elements): %f ms\n\n", average_time);


  //DL_LIST 

  // Quick Sorting
  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter(&time_start);

  DL_QuickSorting(sorting_dl_list1, 0, sorting_dl_list1->ops_->length(sorting_dl_list1) - 1);
  // stop timer
  QueryPerformanceCounter(&time_end);
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("Quick Sort DL_List 1000\n");
  printf("Elapsed time: %f ms\n", elapsed_time);
  average_time = elapsed_time / ksorting1;
  printf("Average time(1000 elements): %f ms\n\n", average_time);

  
  // Cocktail
  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter(&time_start);

  DL_Cocktail_List(sorting_dl_list2, sorting_dl_list2->ops_->length(sorting_dl_list2));

  // stop timer
  QueryPerformanceCounter(&time_end);
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("Cocktail Sort DL_List 1000\n");
  printf("Elapsed time: %f ms\n", elapsed_time);
  average_time = elapsed_time / ksorting3;
  printf("Average time(1000 elements): %f ms\n\n", average_time);


  if (kprintResults) {
    printf("\n\n\nDespues\n\n");
    for (int i = 0; i < ksorting3; ++i) {
      printf("%07d ||| %07d ||| %07d ||| %07d\n",
        sorting_list1->ops_->at(sorting_list1, i),
        sorting_list2->ops_->at(sorting_list2, i),
        sorting_dl_list1->ops_->at(sorting_dl_list1, i),
        sorting_dl_list2->ops_->at(sorting_dl_list2, i));
    }
    printf("\n\n\n");
  }

  //Resets 
  sorting_list1->ops_->softReset(sorting_list1);
  sorting_list2->ops_->softReset(sorting_list2);
  sorting_dl_list1->ops_->softReset(sorting_dl_list1);
  sorting_dl_list2->ops_->softReset(sorting_dl_list2);

  // -----------------------------------------
  for (int i = 0; i < ksorting3; ++i) {
    sorting_list1->ops_->insertLast(sorting_list1, sorting3data[i], 4);
    sorting_list2->ops_->insertLast(sorting_list2, sorting3data[i], 4);

    sorting_dl_list1->ops_->insertLast(sorting_dl_list1, sorting3data[i], 4);
    sorting_dl_list2->ops_->insertLast(sorting_dl_list2, sorting3data[i], 4);
  }

  //LIST


  if (kprintResults) {
    for (int i = 0; i < ksorting3; ++i) {
      printf("%07d ||| %07d ||| %07d ||| %07d\n",
        sorting_list1->ops_->at(sorting_list1, i),
        sorting_list2->ops_->at(sorting_list2, i),
        sorting_dl_list1->ops_->at(sorting_dl_list1, i),
        sorting_dl_list2->ops_->at(sorting_dl_list2, i));
    }
    printf("\n\n\n");
  }
  // Bubble Sorting
  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter(&time_start);

  bubbleSort(sorting_list1, sorting_list1->length_);
  // stop timer
  QueryPerformanceCounter(&time_end);
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("Bubble Sort List 1000\n");
  printf("Elapsed time: %f ms\n", elapsed_time);
  average_time = elapsed_time / ksorting3;
  printf("Average time(1000 elements): %f ms\n\n", average_time);


  // Comb Sort
  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter(&time_start);

  combSort(sorting_list2, sorting_list2->length_);

  // stop timer
  QueryPerformanceCounter(&time_end);
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("Comb Sort List 1000\n");
  printf("Elapsed time: %f ms\n", elapsed_time);
  average_time = elapsed_time / ksorting3;
  printf("Average time(1000 elements): %f ms\n\n", average_time);


  //DL_LIST 



  // Bubble Sort
  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter(&time_start);

  DL_bubbleSort(sorting_dl_list1, sorting_dl_list1->length_);
  // stop timer
  QueryPerformanceCounter(&time_end);
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("Bubble Sort DL_List 1000\n");
  printf("Elapsed time: %f ms\n", elapsed_time);
  average_time = elapsed_time / ksorting1;
  printf("Average time(1000 elements): %f ms\n\n", average_time);


  // Comb Sort
  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter(&time_start);

  DL_combSort(sorting_dl_list2, sorting_dl_list2->length_);

  // stop timer
  QueryPerformanceCounter(&time_end);
  // compute the elapsed time in microseconds
  elapsed_time = (time_end.QuadPart - time_start.QuadPart) * 1000000.0f / frequency.QuadPart;
  printf("Comb Sort DL_List 1000\n");
  printf("Elapsed time: %f ms\n", elapsed_time);
  average_time = elapsed_time / ksorting3;
  printf("Average time(1000 elements): %f ms\n\n", average_time);



  if (kprintResults) {
    printf("\n\n\nDespues\n\n");
    for (int i = 0; i < ksorting3; ++i) {
      printf("%07d ||| %07d ||| %07d ||| %07d\n",
        sorting_list1->ops_->at(sorting_list1, i),
        sorting_list2->ops_->at(sorting_list2, i),
        sorting_dl_list1->ops_->at(sorting_dl_list1, i),
        sorting_dl_list2->ops_->at(sorting_dl_list2, i));
    }
    printf("\n\n\n");
  }
  //Resets 
  sorting_list1->ops_->softReset(sorting_list1);
  sorting_list2->ops_->softReset(sorting_list2);
  sorting_dl_list1->ops_->softReset(sorting_dl_list1);
  sorting_dl_list2->ops_->softReset(sorting_dl_list2);
}



int QuickSorting(List* list, int index_start, int index_end) {

  if (index_start < index_end){
    int pivot_index = Partition(list, index_start, index_end);

    QuickSorting(list, index_start, pivot_index - 1);
    QuickSorting(list, pivot_index + 1, index_end);
  }
}

int DL_QuickSorting(DL_List* list, int index_start, int index_end) {

  if (index_start < index_end) {
    int pivot_index = DL_Partition(list, index_start, index_end);

    DL_QuickSorting(list, index_start, pivot_index - 1);
    DL_QuickSorting(list, pivot_index + 1, index_end);
  }
}


int Cocktail_List(List* list, int number_elements) {
  bool swapped = true;
  int first_index = 0;
  int last_index = number_elements - 1;
  int n1, n2;

  while (swapped) {
    // reset the swapped flag on entering
    // the loop, because it might be true from
    // a previous iteration.
    swapped = false;

    // loop from left to right same as
    // the bubble sort
    for (int i = first_index; i < last_index; ++i)
    {
      n1 = list->ops_->at(list, i);
      n2 = list->ops_->at(list, i+1);
      if (n1 > n2) {
        SwapListElements(list, i, i+1);
        swapped = true;
      }
    }

 
    if (!swapped)
      break;
    swapped = false;
    --last_index;
    for (int i = last_index - 1; i >= first_index; --i){

      n1 = list->ops_->at(list, i);
      n2 = list->ops_->at(list, i + 1);

      if (n1 > n2) {
        SwapListElements(list, i, i + 1);
        swapped = true;
      }
    }
    ++first_index;
  }
}

int DL_Cocktail_List(List* list, int number_elements) {
  bool swapped = true;
  int first_index = 0;
  int last_index = number_elements - 1;
  int n1, n2;

  while (swapped) {
    swapped = false;
    for (int i = first_index; i < last_index; ++i){
      n1 = list->ops_->at(list, i);
      n2 = list->ops_->at(list, i + 1);
      if (n1 > n2) {
        DL_SwapListElements(list, i, i + 1);
        swapped = true;
      }
    }
    if (!swapped)
      break;
    swapped = false;
    --last_index;
    for (int i = last_index - 1; i >= first_index; --i) {
      n1 = list->ops_->at(list, i);
      n2 = list->ops_->at(list, i + 1);
      if (n1 > n2) {
        DL_SwapListElements(list, i, i + 1);
        swapped = true;
      }
    }
    ++first_index;
  }
}

void SortingTest() {
  Sorting2Test250();
  Sorting2Test500();
  Sorting2Test1000();

}

void TESTBASE_mm_free() {

 

  //Sorting Times
  
  sorting_list1->ops_->softReset(sorting_list1);
  sorting_list2->ops_->softReset(sorting_list2);
  sorting_dl_list1->ops_->softReset(sorting_dl_list1);
  sorting_dl_list2->ops_->softReset(sorting_dl_list2);

  sorting_list1->ops_->destroy(sorting_list1);
  sorting_list2->ops_->destroy(sorting_list2);
  sorting_dl_list1->ops_->destroy(sorting_dl_list1);
  sorting_dl_list2->ops_->destroy(sorting_dl_list2);
  
  MM->free(sorting1data);
  MM->free(sorting2data);
  MM->free(sorting3data);

  
}


int main(int argc, char** argv) {
	srand(time(NULL));

	TESTBASE_generateDataForComparative();

  /*VectorComp();
  ListComp();
  DL_ListComp();*/


  SortingTest();

  TESTBASE_mm_free();
	MM->status();
	printf("Press ENTER to continue\n");
	getchar();
	MM->destroy();
	return 0;
}
