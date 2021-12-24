#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"




listptr LstCreate()	//This function creates an empty list
{
	return NULL;
}

listptr LstInsert(listptr list,ListItem item)		//This function inserts a node at the end of a list
{

	listptr temp=list,newnode;
	//Create the node
	newnode=malloc(sizeof(struct listnode));
	newnode->item=item;
	newnode->next=NULL;
	//Node is created
	if (temp==NULL) {	//If the list is empty just insert the node

		temp=newnode;
		return temp;
	}
	else {				//Else go to the end
		while(list->next!=NULL) list=list->next;
		list->next=newnode;	//Insert it
		return temp;
	}
}

void bubbleSort(listptr start)										//This function sorts a list
{
    int swapped, i;
    listptr ptr1;
    listptr lptr = NULL;

    if (start == NULL) return;
    do  {
        swapped = 0;
        ptr1 = start;

        while (ptr1->next != lptr) {
            if (ptr1->item.prime > ptr1->next->item.prime)  {
                swap(ptr1, ptr1->next);
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    }
    while (swapped);
}


void swap(listptr a, listptr b)
{
    ListItem temp = a->item;
    a->item = b->item;
    b->item = temp;
}

void removeDuplicates(listptr head)											//This function removes duplicates of a list
{
    listptr current = head;
    listptr next_next;
    if (current == NULL) return;
    while (current->next != NULL){
       if (current->item.prime == current->next->item.prime) {
           next_next = current->next->next;
           free(current->next);
           current->next = next_next;
       }
       else{
          current = current->next;
       }
    }
}

void LstDestroy(listptr *list)			//This function destroys a list by freeing all the memory
{

   listptr current = *list;
   listptr next;
   while (current != NULL){
       next = current->next;	//Save the next node
       free(current);		//Delete the current node
       current = next;
   }
   *list = NULL;
}