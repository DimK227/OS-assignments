#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"


struct item{
	float a;		//average
	char *id;		//id
};

struct Item{
	int number;	//postal code
	int f;			//frequency of each postal code
};

listptr LstCreate()	//This function creates an empty list
{
	return NULL;
}

YearNodePtr YNCreate() //This function creates an empty YearNode list
{
	return NULL;
}

int compare(const void *a, const void *b)	//Compare function for qsort
{
	struct item *itemA = (struct item *)a;
	struct item *itemB = (struct item *)b;

	return (itemB->a > itemA->a) - (itemB->a < itemA->a);
}

int compare1(const void *a, const void *b) //Compare function for qsort
{
	struct Item *itemA = (struct Item *)a;
	struct Item *itemB = (struct Item *)b;

	return (itemB->number - itemA->number);
}

int compare2(const void *a, const void *b)  //Compare function for qsort
{
	struct Item *itemA = (struct Item *)a;
	struct Item *itemB = (struct Item *)b;

	return (itemB->f - itemA->f);
}


void PrintMinAv(StudentNodePtr list, int year)		//This function prints the student(s) with the minimum average
{
	float min = 11.0;							//Average cant be more than 10
	StudentNodePtr temp = list, temp1 = list;
	if (temp==NULL) {										//If thre are no students
		printf("No students enrolled in %d\n", year);
		return;
	}
	else {
		while (temp!=NULL) {
				if (temp->info->info.item.average < min) min = temp->info->info.item.average;			//Find the min
				temp = temp->nextStudent;
		}
		while (temp1!=NULL) {
				if (min == temp1->info->info.item.average) printf ("%s\n",temp1->info->info.item.studentID);		//Find which students have this minimum average
				temp1 = temp1->nextStudent;
		}
	}
}

void PrintCountOfEachYear(YearNodePtr list)			//This function prints how many students there are in each year of sth studies
{
	YearNodePtr temp = list;
	while (temp!=NULL) {														//Traverse the list
		printf("(%d years of studies-%d)\n", temp->year , LstCount(temp->students));   	//and print the count of each list
		temp=temp->next;
	}
		printf("\n");
}

int Get_TotalCount(YearNodePtr list) 		//This function prints how many students there are
{
	YearNodePtr temp = list;
	int cnt=0;
	while (temp!=NULL) {						//For each year call LstCount
				cnt += LstCount(temp->students);
				temp=temp->next;
	}
		return cnt;
	}


void PrintRank(YearNodePtr list, int n)			//This function prints the nth most popular postal code
{
	struct Item *array;
	int differents=0,cnt,i=0,j,frequency=1,counter=0, max;
	YearNodePtr temp1 = list;
	if (temp1==NULL) {						//Error message
		printf("No students enrolled\n");
		return;
	}
	cnt = Get_TotalCount(list);			//Find hoy many students there are in order to create an array with all the postal codes
	array = malloc(sizeof(struct Item)*cnt);
	if (array==NULL) return;
	while(temp1!=NULL) {
		StudentNodePtr temp2 = temp1->students;
		while (temp2!=NULL) {
			array[i].number = temp2->info->info.item.zip;  //Insert the postal code
			array[i].f = -1;							//initiate frequence with -1
	 		i++;
			temp2 = temp2->nextStudent;
		}
		temp1 = temp1->next;
	}
	qsort(array, cnt, sizeof(struct Item), compare1);		//Sort the array based on the postal codes
	for (i=1; i<cnt; i++) {
			if (array[i].number == array[i-1].number){
			 		frequency++;
					if (i==cnt-1) {
						for (j=i-counter-1; j<=i; j++) {
								array[j].f = frequency;
						}
					}
					counter++;
		 	}																															//This piece of code computes the frequency of each postal code
			else {
				for (j=i-counter-1; j<i; j++) {
						array[j].f = frequency;
				}

				if (i==cnt-1) {
					for (j=i-counter-1; j<=i; j++) {
							array[j].f = frequency;
					}
				}
				frequency=1;
				counter=0;

			}

	}
	qsort(array, cnt, sizeof(struct Item), compare2);		//Sort the array based on the frequency
	for (i=0; i<cnt-1; i++) {														//Then change the value of frequency of the repeated postal codes to -1 (except the first of the repeated sequence)
		  if (array[i].number==array[i+1].number) {
					array[i+1].f = -1;
			}
	}
	qsort(array, cnt, sizeof(struct Item), compare2);		//Resort the array based on the frequency
	for (i=0; i<cnt; i++){
			if (array[i].f != -1) {
					differents++;																//Find how many differents postal codes are there
			}
			else break;
	}
	if (n>differents) printf("Sorry there are only %d postal codes\n", differents);  //Error message
	else {
		max = array[n-1].f;
		printf("%d 	", array[n-1].number);
		for (i=n; i<differents; i++) {
			  if (max == array[i].f) {
						printf("%d 	", array[i].number);				//Print the nth postal code(s)
				}
		}
		printf("\n");
	}
	free(array);
}

void LstPrintN(StudentNodePtr list, int n, int year)			//This function prints the best n students
{
	StudentNodePtr temp=list;
  	int i,cnt=LstCount(list);
	if (list==NULL) {																		//Error message
		printf("No students enrolled in %d\n", year);
		return;
	}
	struct item *array;
	array = malloc(sizeof(struct item)*cnt);	//Create an array of id's along with their avearge
	if (array==NULL) return;
	for (i=0; i<cnt; i++) {
		array[i].id = malloc(MAXSIZE*sizeof(char));
		if (array[i].id==NULL) return;
		array[i].a = temp->info->info.item.average;					//Insert the average
		strcpy(array[i].id, temp->info->info.item.studentID);	//Insert the id
		temp = temp->nextStudent;
	}
	qsort(array, cnt, sizeof(struct item), compare);   //Sort the array based on thh average
	for (i=0; i<n; i++) {					//Print the first n
		printf("%s	with %f\n", array[i].id, array[i].a);
	}
	int j = n;
	while (array[n-1].a==array[j].a) {
		 printf("%s     with %f\n", array[j].id, array[j].a);
		 j++;
	}
	for (i=0; i<cnt; i++) {
		free(array[i].id);
	}
	free(array);
}

StudentNodePtr GetStudents(YearNodePtr list, int year)					//This function returns all the students of each specific year
{
	YearNodePtr temp = list;
	while (temp!=NULL) {
		if (temp->year == 2020-year+1) return temp->students;		//Find the year and return it
		else temp=temp->next;
	}
	return NULL;
}

YearNodePtr YNInsert(YearNodePtr list,listptr node, int year)			//This function inserts a node in the inverted index structure
{
	YearNodePtr temp = list;
	StudentNodePtr temp1;
	if (temp==NULL) {																					//If it is empty
			YearNodePtr newnode;
			newnode = malloc(sizeof(struct YearNode));
			newnode->year = 2020-year+1;
			newnode->next = NULL;
			newnode->students = malloc(sizeof(struct StudentNode));									//Create the node and just insert it
			newnode->students->info = node;
			newnode->students->nextStudent = NULL;
			return newnode;
	}
	else {																															//If it is not epty
			while (temp->next!=NULL || temp->year == 2020-year+1) {					//Traverse the list to find the node that has the same year with our new student who is going to be inserted
					if (temp->year == 2020-year+1) {														//When you find it
						temp1 = temp->students;
						while (temp1->nextStudent != NULL) temp1 = temp1->nextStudent;
						temp1->nextStudent = malloc(sizeof(struct StudentNode));		//Create the node and insert it
						temp1->nextStudent->info = node;
						temp1->nextStudent->nextStudent = NULL;

						return list;
					}
					else temp = temp->next;
			}
			YearNodePtr temp2 = temp;					//If we are here that means that there is no such year and so we create it
			temp->next=malloc(sizeof(struct YearNode));
			temp->next->year = 2020-year+1;
			temp->next->next = NULL;
			temp->next->students = malloc(sizeof(struct StudentNode));
			temp->next->students->info = node;
			temp->next->students->nextStudent = NULL;
			return list;
	}

}

listptr LstInsert(listptr list,char *key,ListItem item)		//This function inserts a node at the end of a list and in the inverted index structure
{
	listptr temp=list,newnode,newnode2;
	//Create the node
	newnode=malloc(sizeof(struct listnode));
	(newnode->info).key=malloc(MAXSIZE*sizeof(char));
	strcpy((newnode->info).key,key);
	(newnode->info).item=item;
	newnode->next=NULL;
	//Node is created
	if (temp==NULL) temp=newnode;	//If the list is empty just insert the node
	else {				//Else go to the end
		while(list->next!=NULL) list=list->next;
		list->next=newnode;	//Insert it
	 }
	return temp;
}


int LstCount(StudentNodePtr list)		//This function counts the node of a list
{
	int cnt=0;
	StudentNodePtr temp=list;
	while (temp!=NULL) {
		temp=temp->nextStudent;
		cnt++;
	}
	if (cnt==0) return -1.0;
	return cnt;
}

float LstAverage(StudentNodePtr list)			//This function finds the average grade of a list
{
	StudentNodePtr temp=list;
	float s=0.0;
	int cnt=0;

	while (temp!=NULL) {
			cnt++;
			s+=temp->info->info.item.average;
			temp=temp->nextStudent;
	}
	return s/cnt;
}


int LstSearch(listptr list,char *key,ListItem *pitem)		//This function searches for a key in a list and if it exists copies the item to the pitem
{
	listptr temp=list;
	while (temp!=NULL) {
		if (strcmp((temp->info).key,key)==0) {		//If we find it
			(*pitem)=(temp->info).item; 						//Copy
			return 1;				//Return TRUE
		}
		temp=temp->next;
	}
	return 0;
}

listptr LstGetNode(listptr list, char *key)
{
	listptr temp = list;
	while (temp!=NULL) {
		if (strcmp((temp->info).key,key)==0) {		//If we find it
			return temp;			//Return the node
		}
		temp=temp->next;
	}
}


listptr LstDelete(listptr list, char *node)  	//This function deletes a node
{
    listptr dummy,templist=list;
    dummy=list;					//Dummy points to the start of the list
    if (strcmp((list->info).key,node)==0) {  	//If the node that we are looking for is at the start
                templist=list;
                list=list->next;
                free(templist);
                return list;
   }
   else {
                while (strcmp((list->info).key,node)!=0) {    	//I go at the end of the list saving the node before the last one
                        templist=list;
                        list=list->next;
                        if (list==NULL) {
                                printf("There is no node with code %s\n",node); 	//That means that the  node  that we want to delete does not exist
                                return dummy;
                           }
                }
                list=list->next;
                free(templist->next);
        	templist->next=list;
        	return dummy;
  }

}



void LstDestroy(listptr *list)			//This function destroys a list by freeing all the memory
{

   listptr current = *list;
   listptr next;
   while (current != NULL){
       next = current->next;	//Save the next node
       free(current->info.item.studentID);
       free(current->info.item.fname);
       free(current->info.item.lname);
       free(current);		//Delete the current node
       current = next;
   }
   *list = NULL;
}

void YNDestroy(YearNodePtr *list)
{
	YearNodePtr current = *list;
	YearNodePtr next;
	while (current != NULL){
		next = current->next;	//Save the next node
		SNDestroy(&current->students);
		free(current);
		current=next;

	}
}

void SNDestroy(StudentNodePtr *list)
{
	StudentNodePtr current = *list;
	StudentNodePtr next;
	while (current != NULL){
			next = current->nextStudent;	//Save the next node
			free(current);		//Delete the current node
			current = next;
	}
	*list = NULL;
}
