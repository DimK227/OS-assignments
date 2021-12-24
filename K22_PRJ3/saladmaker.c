#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <time.h>
#include <string.h>
#include "sem.h"


FILE *fp;
char vegetable[10];
FILE *fp2;
FILE *fp3;
sem_t wrt1;

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


float float_rand( float min, float max )                      //This function generates a random float number between min and max
{
    float scale = rand() / (float) RAND_MAX; /* [0, 1.0] */
    return min + scale * ( max - min );      /* [min, max] */
}



void Make_Salad(float time, restaurant **r)       //In this function the saladmaker makes a salad
{
    int id;
    float time1 = time/2.0;
    struct timeval  now;
    struct tm*      local;

    if (strcmp(vegetable,"onions") == 0) id = 1;
    else if (strcmp(vegetable,"peppers") == 0) id = 2;
    else if (strcmp(vegetable,"tomatoes") == 0) id = 3;

    printf("Start making salad\n");

    P(&((*r)->wrt));
    gettimeofday(&now, NULL);
    local = localtime(&now.tv_sec);
    fprintf(fp2,"%02d:%02d:%02d.%02ld [%d] [Saladmaker%d]: Start making salad\n",local->tm_hour, local->tm_min, local->tm_sec, now.tv_usec / 1000,getpid(),id);   //Write to logfile
    fflush(fp2);
    V(&((*r)->wrt));

    sleep(time1);                                                               //Time to cut the first vegetable
    gettimeofday(&now, NULL);
    local = localtime(&now.tv_sec);
    if (id == 1) fprintf(fp,"%02d:%02d:%02d.%02ld: I cut tomatoes\n", local->tm_hour, local->tm_min, local->tm_sec, now.tv_usec / 1000);
    else if (id == 2) fprintf(fp,"%02d:%02d:%02d.%02ld: I cut tomatoes\n", local->tm_hour, local->tm_min, local->tm_sec, now.tv_usec / 1000);     //Write to logfile
    else if (id == 3) fprintf(fp,"%02d:%02d:%02d.%02ld: I cut onions\n", local->tm_hour, local->tm_min, local->tm_sec, now.tv_usec / 1000);

    sleep(time1);                                                                //Time to cut the second vegetable
    gettimeofday(&now, NULL);
    local = localtime(&now.tv_sec);
    if (id == 1) fprintf(fp,"%02d:%02d:%02d.%02ld: I cut peppers\n", local->tm_hour, local->tm_min, local->tm_sec, now.tv_usec / 1000);
    else if (id == 2) fprintf(fp,"%02d:%02d:%02d.%02ld: I cut onions\n", local->tm_hour, local->tm_min, local->tm_sec, now.tv_usec / 1000);       //Write to logfile
    else if (id == 3) fprintf(fp,"%02d:%02d:%02d.%02ld: I cut peppers\n", local->tm_hour, local->tm_min, local->tm_sec, now.tv_usec / 1000);

    P(&(*r)->mutex);

    (*r)->Number_Of_Salads --;                                                  //A salad has been made
    if (id == 1) ((*r)->saladmaker1).salads++;
    else if (id == 2) ((*r)->saladmaker2).salads++;
    else if (id == 3)   ((*r)->saladmaker3).salads++;


    V(&(*r)->mutex);
    gettimeofday(&now, NULL);
    local = localtime(&now.tv_sec);
    printf("End making salad\n");
    P(&((*r)->wrt));
    fprintf(fp2,"%02d:%02d:%02d.%02ld [%d] [Saladmaker%d]: End making salad\n",local->tm_hour, local->tm_min, local->tm_sec, now.tv_usec / 1000,getpid(),id);  //Write to logfile
    fflush(fp2);
    V(&((*r)->wrt));

    P(&wrt1);
    fprintf(fp3,"%02d:%02d:%02d.%02ld\n",local->tm_hour, local->tm_min, local->tm_sec, now.tv_usec / 1000);         //Write to logfile
    V(&wrt1);


    fprintf(fp,"%02d:%02d:%02d.%02ld: I made a salad\n", local->tm_hour, local->tm_min, local->tm_sec, now.tv_usec / 1000);
}


int main(int argc, char* argv[])
{
  struct timeval  now;
  struct tm*      local;
  sem_init(&wrt1,1,1);
  int i,shmid,id = getpid(),cnt=0;
  float lb,ub,Time_To_Make_Salad;
  restaurant *r;
  srand(time(0));
  for (i=0; i<argc; i++) {
    if (strcmp(argv[i],"-t1")==0) lb = atof(argv[i+1]);
    if (strcmp(argv[i],"-t2")==0) ub = atof(argv[i+1]);
    if (strcmp(argv[i],"-s")==0) shmid = atoi(argv[i+1]);
    if (strcmp(argv[i],"-v")==0) strcpy(vegetable,argv[i+1]);
  }

  if (strcmp(vegetable,"onions") == 0) i = 1;
  else if (strcmp(vegetable,"peppers") == 0) i = 2;
  else if (strcmp(vegetable,"tomatoes") == 0) i = 3;


  fp2 = fopen("logfile.txt","a");
  fp3 = fopen("file.txt","a");

  char str[20];
  sprintf(str, "%d", id);
  char *str2;
  str2 = concat(str,".txt");          //Create the name and open the files
  char *name = concat("logfile",str2);
  fp = fopen(name,"w");
  r = (restaurant *) shmat(shmid, (void*)0, 0);
  Time_To_Make_Salad = float_rand(lb,ub);
  printf("Waiting for ingredients\n");


  P(&(r->wrt));
  gettimeofday(&now, NULL);
  local = localtime(&now.tv_sec);
  fprintf(fp2,"%02d:%02d:%02d.%02ld [%d] [Saladmaker%d]: Waiting for ingredients\n",local->tm_hour, local->tm_min, local->tm_sec, now.tv_usec / 1000,getpid(),i);  //Write to logfile
  fflush(fp2);
  V(&(r->wrt));



  //Each saladmaker that does not work at first is suspended

  if (strcmp(vegetable,"onions") == 0)   {
    (r->saladmaker1).pid = getpid();
    (r->saladmaker1).salads = 0;
    P(&(r->saladmaker_who_has_onions));
  }
  else if (strcmp(vegetable,"tomatoes") == 0) {
    (r->saladmaker3).pid = getpid();
    (r->saladmaker3).salads = 0;
    P(&(r->saladmaker_who_has_tomatoes));
  }
  else if (strcmp(vegetable,"peppers") == 0) {
    (r->saladmaker2).pid = getpid();
    (r->saladmaker2).salads = 0;
    P(&(r->saladmaker_who_has_peppers));
  }

  while (r->Number_Of_Salads > 0){
      printf("Get ingredients\n");

      P(&(r->wrt));
      gettimeofday(&now, NULL);
      local = localtime(&now.tv_sec);
      fprintf(fp2,"%02d:%02d:%02d.%02ld [%d] [Saladmaker%d]: Get ingredients\n",local->tm_hour, local->tm_min, local->tm_sec, now.tv_usec / 1000,getpid(),i);   //Write to logfile
      fflush(fp2);
      V(&(r->wrt));
      V(&(r->chef));

      P(&wrt1);
      fprintf(fp3,"[Saladmaker%d]: %02d:%02d:%02d.%02ld - ",i,local->tm_hour, local->tm_min, local->tm_sec, now.tv_usec / 1000);      //Write to file
      V(&wrt1);

      Make_Salad(Time_To_Make_Salad,&r);                                                              //Make a salad
      printf("Waiting for ingredients\n");

      P(&(r->wrt));
      gettimeofday(&now, NULL);
      local = localtime(&now.tv_sec);
      fprintf(fp2,"%02d:%02d:%02d.%02ld [%d] [Saladmaker%d]: Waiting for ingredients\n",local->tm_hour, local->tm_min, local->tm_sec, now.tv_usec / 1000,getpid(),i);  //Write to logfile
      fflush(fp2);
      V(&(r->wrt));


      //When he finishes his job he gets suspended until a notification 
      if (strcmp(vegetable,"onions") == 0) P(&(r->saladmaker_who_has_onions));
      else if (strcmp(vegetable,"tomatoes") == 0) P(&(r->saladmaker_who_has_tomatoes));
      else if (strcmp(vegetable,"peppers") == 0) P(&(r->saladmaker_who_has_peppers));
  }
  V(&(r->chef));
  sem_destroy(&(r->saladmaker_who_has_onions));
  sem_destroy(&(r->saladmaker_who_has_peppers));
  sem_destroy(&(r->saladmaker_who_has_tomatoes));
  sem_destroy(&(r->mutex));
  free(str2);
  free(name);
  int err = shmdt((void *) r);
  if (err == -1) perror ("Detachment.");
  else printf(">> Detachment of Shared Segment %d\n", err);
  fclose(fp2);
}
