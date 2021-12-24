#pragma once
#include "list.h"


typedef struct {
	listptr *hash;		//The hash table will contain lists
	int max;

}HTHash;

typedef listptr HTElement;		//Each element will be a lists
typedef ListItem HTItem;


typedef void (*FunPtr)(HTHash hash,char *key,HTItem item);	//Pointer to function

HTHash HTCreate(int max);
int  HTGet(HTHash hash,char *key, HTItem *pitem);
HTElement HTGetNode(HTHash hash, char* key);
HTHash HTInsert(HTHash hash,char *key, HTItem item);
HTHash HTRemove(HTHash hash, char *key);
void HTDestroy(HTHash hash);
void VisitElement(HTHash hash, char *key, HTItem item);
int HashCode (char *key,int max);
