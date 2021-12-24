#define MAXSIZE 30	//This is the max size of  strings
#pragma once
typedef struct listnode *listptr;



typedef struct element{
  int prime;
  double time;
} ListItem;


struct listnode{
        ListItem item;
        listptr next;
};

listptr LstCreate();
listptr LstInsert(listptr list, ListItem item);
void LstDestroy(listptr *list);
void swap(listptr a, listptr b);
void bubbleSort(listptr start);
void removeDuplicates(listptr head);
void LstDestroy(listptr *list);