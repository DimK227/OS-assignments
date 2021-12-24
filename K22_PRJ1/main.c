#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#include <string.h>



int main(int argc, char *argv[])
{
  HTHash hash;
  YearNodePtr doubleList;
  FILE *fp, *fp2;
  FunPtr visit;
  char *filename,*cfile;
  int i,j,number1,cnt=0,count=0,ctr;
  char ch, *word;
  float number2;
  for (i=0; i<argc; i++) {
    if (strcmp(argv[i],"-i")==0) filename = argv[i+1];
    if (strcmp(argv[i],"-c")==0) cfile = argv[i+1];
  }

  fp2 = fopen(cfile, "r");      //Open the file
  word = malloc(MAXSIZE*sizeof(char));
	if (fp2 == NULL) return -1;

  fscanf(fp2,"%s",word);
  fscanf(fp2,"%s",word);      //Based on my configfile the thord word is the size of the hash table
  fscanf(fp2,"%s",word);
  fclose(fp2);
  hash=HTCreate(atoi(word));             //Create the hash table
  doubleList = YNCreate();        //Create the inverted index structure
  fp = fopen(filename, "r");      //Open the file
	if (fp == NULL) return -1;



  while((ch = fgetc(fp)) != EOF){     //Find how many words there are (count+cnt)
    if (ch==' ') count++;
    if (ch=='\n') cnt++;
  }
  int counter=0;
  fp = fopen(filename, "r");
  for (i=1; i<=count+cnt; i+=6) {           //Each line has six words
    HTItem item;
    HTItem pitem;
    item.studentID = malloc(MAXSIZE*sizeof(char));
    item.fname = malloc(MAXSIZE*sizeof(char));
    item.lname = malloc(MAXSIZE*sizeof(char));
    fscanf(fp,"%s",word);
    strcpy(item.studentID,word);

    fscanf(fp,"%s",word);
    strcpy(item.fname,word);
    fscanf(fp,"%s",word);
    strcpy(item.lname,word);
                                            //Insert the data to item which is going to be inserted to the hash table by reading 6 times (one for each word) each time
    fscanf(fp,"%d",&number1);
    item.zip=number1;

    fscanf(fp,"%d",&number1);
    item.year=number1;

    fscanf(fp,"%f",&number2);
    item.average=number2;
    if (HTGet(hash,item.studentID,&item)==0) {
      hash = HTInsert(hash,item.studentID,item);                            //Insert it to the hash table
      doubleList = YNInsert(doubleList,HTGetNode(hash,item.studentID), item.year);    //Get the node that just was inserted and put it in the invertedIndex
    }
    else counter++;
  }
  fclose(fp);

  char input[256];
  char words[15][15];
  printf("Type what do you want to do\n");
  fgets(input, sizeof(input), stdin);
  strtok(input, "\n");
  //This piece of code puts each word that the user gives as input in the words array
  j=0; ctr=0;
  for(i=0;i<=(strlen(input));i++){
      if(input[i]==' '||input[i]=='\0'){
          words[ctr][j]='\0';
          ctr++;
          j=0;
      }
      else{
          words[ctr][j]=input[i];
          j++;
      }
  }
  //Prompt starts
  while (strcmp(input,"exit")!=0) {         //While the user does not say exit
      if (strcmp(words[0],"i")==0) {        //Insert a student
          HTItem item;
          strcpy(item.studentID,words[1]);
          strcpy(item.fname,words[2]);
          strcpy(item.lname,words[3]);
          item.zip=atoi(words[4]);
          item.year=atoi(words[5]);
          item.average=atoi(words[6]);
          if (HTGet(hash,words[1],&item)==0) {
              hash = HTInsert(hash,item.studentID,item);
              doubleList = YNInsert(doubleList,HTGetNode(hash,item.studentID), item.year);
              printf("Student %s inserted\n", words[1]);
          }
          else printf("Student %s exists\n", words[1]);   //Message error
    }
    else if (strcmp(words[0],"l")==0) {             //Print all the data of a student
          HTItem item;
          if (HTGet(hash,words[1],&item)==1) VisitElement(hash, words[1], item);
          else printf("Student %s does not exist\n", words[1]);  //Message error
    }
    else if (strcmp(words[0],"d")==0) {        //Delete a student
          HTItem item;
          if (HTGet(hash,words[1],&item)==1) {
              hash = HTRemove(hash, words[1]);
              printf("Record %s deleted\n", words[1]);
          }
          else printf("Student %s does not exist\n", words[1]);
    }
    else if (strcmp(words[0],"n")==0) {                   //Print how many students there are in a speacific year
          printf("%d students in %d\n",LstCount(GetStudents(doubleList,atoi(words[1]))), atoi(words[1]) );
    }
    else if (strcmp(words[0],"t")==0) {                        //Print the top  n students of a year
          LstPrintN(GetStudents(doubleList,atoi(words[2])), atoi(words[1]), atoi(words[2]));
    }
    else if (strcmp(words[0],"a")==0) {                           //Find the average of all students of a specific year
          float a;
          if ((a=LstAverage(GetStudents(doubleList,atoi(words[1])))) >=0) printf("%f\n",a);
          else printf("No students enrolled in %d\n", atoi(words[1]));  //Message error
    }
    else if (strcmp(words[0],"m")==0) {                   //Find who has the minimum average
          PrintMinAv(GetStudents(doubleList,atoi(words[1])), atoi(words[1]));
    }
    else if (strcmp(words[0],"c")==0) {             //Count how many students each year has
          PrintCountOfEachYear(doubleList);
    }
    else if (strcmp(words[0],"p")==0) {           //Finf the rankth most popular postal code
          PrintRank(doubleList, atoi(words[1]));
    }
    printf("Type what do you want to do\n");
    fgets(input, sizeof(input), stdin);
    strtok(input, "\n");
    j=0; ctr=0;
    for(i=0;i<=(strlen(input));i++){
        if(input[i]==' '||input[i]=='\0'){
            words[ctr][j]='\0';
            ctr++;
            j=0;
        }
        else{
            words[ctr][j]=input[i];
            j++;
        }
    }
  }
  printf("exit program\n");
  free(word);
  HTDestroy(hash);          //Free the space
  YNDestroy(&doubleList);
}
