#include <stdio.h>
#include <stdlib.h>
#include "hash.h"

//SEPERATE CHAINING IMPLEMENTATION


HTHash HTCreate(int max)		//This  dunction creates a hash table
{
	HTHash table;
	int i;
	table.hash=malloc(sizeof(HTElement)*max);		//Create the array
	table.max=max;
	for (i=0; i<max; i++) table.hash[i]=LstCreate();	//Create the empty lists
	return table;
}



int HTGet(HTHash hash,char *key, HTItem *pitem)		//This function checks if a key exists in a hash table and if so  it copies the item to pitem
{
	int index=HashCode(key,hash.max);			//Hash the key in order to find the index
	return LstSearch(hash.hash[index],key,pitem);	//Check if it exists in the list
}

HTElement HTGetNode(HTHash hash, char* key)
{
	int index=HashCode(key,hash.max);			//Hash the key in order to find the index
	return LstGetNode(hash.hash[index],key);

}

HTHash HTInsert(HTHash hash,char *key, HTItem item/*, invertedIndex *doubleList*/)	//This function inserts a key with its item in a hash table
{
	FunPtr visit=&VisitElement;
	int index=HashCode(key,hash.max);			//Hash the key in order to find th true index
	hash.hash[index]=LstInsert(hash.hash[index],key,item);	//Insert the key in the list (if the key does not exists)
	return hash;
}


HTHash HTRemove(HTHash hash, char *key)		//This function deletes an element of a hash table
{
	int index=HashCode(key,hash.max);		//Hash the key in order to find the true index
	hash.hash[index]=LstDelete(hash.hash[index],key);		//Delete it
	return hash;

}


void VisitElement(HTHash hash, char *key, HTItem item)	//This function visits so as to print  an element of a hash table
{
	printf("%s %s %s %d %d %f\n",key,item.fname,item.lname,item.zip,item.year,item.average);	//Print it
}

void HTDestroy(HTHash hash)		//This function destroys a hash table by freeing all the memory
{
	int i;
	for (i=0; i<hash.max; i++) {
		LstDestroy(&(hash.hash[i]));	//Free all the lists
	}
	free(hash.hash);		//Free the array
}



int HashCode (char *key,int max)		//This function hashes a key and returns an array's index
{
	int h=0, a=33;
  	for (; *key!='\0'; key++) h=(a*h + *key) % max;
  	return h;
}
