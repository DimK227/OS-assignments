#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <string.h>
#include <semaphore.h>
#include "sem.h"


FILE *fp1;


typedef struct {
  saladmaker_stats saladmaker1;
  saladmaker_stats saladmaker2;
  saladmaker_stats saladmaker3;
  sem_t chef;
  sem_t saladmaker_who_has_tomatoes;
  sem_t saladmaker_who_has_onions;
  sem_t saladmaker_who_has_peppers;
  sem_t mutex;
  sem_t wrt;
  int Number_Of_Salads;
}restaurant;


void fun()                //This function prints when 2 proccesses work corcurrently
{
  FILE *fp,*fp2;
  fp = fopen("file.txt","r");
  char *p;
  int i,j,ctr,k=0,j1,ctr1;
  char input[256],input1[256];
  char words[15][15];
  char time[15];
  char start[15];
  char end[15];
  char person[15];
  while(fgets(input,256,fp) != NULL){   //Get each line
    j=0; ctr=0;
    for(i=0;i<=(strlen(input));i++){
        if(input[i]==' '||input[i]=='\0'){
            words[ctr][j]='\0';
            ctr++;
            j=0;
        }
        else{
            words[ctr][j]=input[i];     //words has all the words of the line
            j++;
        }
    }
    strcpy(person,words[0]);
    strcpy(start,words[1]);
    strcpy(end,words[3]);
    fp2 = fopen("file.txt","r");
    while(fgets(input1,256,fp2) != NULL){   //For the time start-end read the file againn to find concurrent activities
        j=0; ctr=0;
        for(i=0;i<=(strlen(input1));i++){
            if(input1[i]==' '||input1[i]=='\0'){
                words[ctr][j]='\0';
                ctr++;
                j=0;
            }
            else{
                words[ctr][j]=input1[i];    //words has the words of the line
                j++;
            }
        }
        if (strcmp(words[0],person)!=0 && strcmp(words[1],start) > 0 && strcmp(words[3],end) > 0 && strcmp(end,words[1])>0) printf("%s-%s\n",words[1],end);   //This is the condition
      }
  }
  fclose(fp);
}


int main(int argc, char* argv[])
{
  int id,i,num,prev=5,cnt=0,pid = getpid();
  restaurant *r;

  int val,f=0;
  double mantime;
  struct timeval  now;
  struct tm*      local;
  id = shmget(IPC_PRIVATE,sizeof(restaurant),0666);
  if (id == -1) perror ("Creation");
  else printf("Allocated Shared Memory with ID: %d\n",(int)id);
  r = (restaurant *) shmat(id, (void*)0, 0);
  if ( r == (void *)  -1) { perror("Attachment."); exit (2);}
  for (i=0; i<argc; i++) {
    if (strcmp(argv[i],"-n")==0) r->Number_Of_Salads = atoi(argv[i+1]);
    if (strcmp(argv[i],"-m")==0) mantime = atof(argv[i+1]);
  }

  fp1 = fopen("logfile.txt","a");                                               //Open the log file for all proccesses
  sem_init(&(r->chef),1,0);
  sem_init(&(r->mutex),1,1);
  sem_init(&(r->wrt),1,1);
  sem_init(&(r->saladmaker_who_has_onions),1,0);
  sem_init(&(r->saladmaker_who_has_tomatoes),1,0);
  sem_init(&(r->saladmaker_who_has_peppers),1,0);
  srand(time(0));
  while(r->Number_Of_Salads>0){
    do {
      num = rand()%3;                                                           //Select different vegetables than the previous time
    }while (num==prev);
    prev = num;
                                                                                //In each case I inform the suitable saladmaker and I write to th file
    if (num == 0) {
        printf("Selecting ingredients [tomatoes][peppers]\n");
        gettimeofday(&now, NULL);
        local = localtime(&now.tv_sec);

        P(&(r->wrt));
        fprintf(fp1,"%02d:%02d:%02d.%02ld [%d] [Chef]: Selecting ingredients [tomatoes][peppers]\n",local->tm_hour, local->tm_min, local->tm_sec, now.tv_usec / 1000,pid);
        fflush(fp1);
        V(&(r->wrt));

        gettimeofday(&now, NULL);
        local = localtime(&now.tv_sec);

        P(&(r->wrt));
        fprintf(fp1,"%02d:%02d:%02d.%02ld [%d] [Chef]: Notify saladmaker1\n",local->tm_hour, local->tm_min, local->tm_sec, now.tv_usec / 1000,pid);
        V(&(r->wrt));
        fflush(fp1);
        printf("Notify saladmaker1\n");

        V(&(r->saladmaker_who_has_onions));

    }
    else if (num == 1){
      printf("Selecting ingredients [tomatoes][onions]\n");
      gettimeofday(&now, NULL);
      local = localtime(&now.tv_sec);

      P(&(r->wrt));
      fprintf(fp1,"%02d:%02d:%02d.%02ld [%d] [Chef]: Selecting ingredients [tomatoes][onions]\n",local->tm_hour, local->tm_min, local->tm_sec, now.tv_usec / 1000,pid);
      fflush(fp1);
      V(&(r->wrt));

      printf("Notify saladmaker2\n");
      gettimeofday(&now, NULL);
      local = localtime(&now.tv_sec);
      P(&(r->wrt));
      fprintf(fp1,"%02d:%02d:%02d.%02ld [%d] [Chef]: Notify saladmaker2\n",local->tm_hour, local->tm_min, local->tm_sec, now.tv_usec / 1000,pid);
      fflush(fp1);
      V(&(r->wrt));
      V(&(r->saladmaker_who_has_peppers));
    }
    else {
      printf("Selecting ingredients [onions][peppers]\n");
      gettimeofday(&now, NULL);
      local = localtime(&now.tv_sec);

      P(&(r->wrt));
      fprintf(fp1,"%02d:%02d:%02d.%02ld [%d] [Chef]: Selecting ingredients [onions][peppers]\n",local->tm_hour, local->tm_min, local->tm_sec, now.tv_usec / 1000,pid);
      fflush(fp1);
      V(&(r->wrt));

      printf("Notify saladmaker3\n");
      gettimeofday(&now, NULL);
      local = localtime(&now.tv_sec);
      P(&(r->wrt));
      fprintf(fp1,"%02d:%02d:%02d.%02ld [%d] [Chef]: Notify saladmaker3\n",local->tm_hour, local->tm_min, local->tm_sec, now.tv_usec / 1000,pid);
      fflush(fp1);
      V(&(r->wrt));
      V(&(r->saladmaker_who_has_tomatoes));
    }
    P(&(r->chef));
    gettimeofday(&now, NULL);
    local = localtime(&now.tv_sec);
    printf("Man time for resting\n");

    P(&(r->wrt));
    fprintf(fp1,"%02d:%02d:%02d.%02ld [%d] [Chef]: Man time for resting\n",local->tm_hour, local->tm_min, local->tm_sec, now.tv_usec / 1000,pid);
    fflush(fp1);
    V(&(r->wrt));

    sleep(mantime);                                                              //Rest
  }


  V(&(r->saladmaker_who_has_onions));
  V(&(r->saladmaker_who_has_peppers));                              //Notify the saladmakers that chef finished his job and he will not give any more ingredients
  V(&(r->saladmaker_who_has_tomatoes));
  P(&(r->chef));
                                                                                //Wait until all saladmakers are done
  P(&(r->chef));
  sem_destroy(&(r->chef));
  sem_destroy(&(r->wrt));
  printf("Total salads: %d\n", (r->saladmaker1).salads+(r->saladmaker2).salads+(r->saladmaker3).salads);
  printf("salads of saladmaker1 [%d]: %d\n",(r->saladmaker1).pid, (r->saladmaker1).salads);
  printf("salads of saladmaker2 [%d]: %d\n",(r->saladmaker2).pid, (r->saladmaker2).salads);
  printf("salads of saladmaker3 [%d]: %d\n",(r->saladmaker3).pid, (r->saladmaker3).salads);
  fun();
  int err = shmctl(id, IPC_RMID, 0);
  if (err == -1) perror ("Removal.");
  else printf("Just Removed Shared Segment. %d\n", (int)(err));
  fclose(fp1);
}
