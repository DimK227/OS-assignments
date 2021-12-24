#include	<stdio.h>
#include	<sys/types.h>
#include	<sys/wait.h>
#include	<sys/stat.h>
#include	<unistd.h>
#include	<string.h>
#include  <stdlib.h>
#include  <fcntl.h>
#include <poll.h>
#include <signal.h>
#include <string.h>
#include <math.h>
#include "list.h"

int counter=0;

void func(int signum){
  counter++;
}

int main(int argc, char *argv[])
{
  listptr list =LstCreate();
  listptr list_of_times =LstCreate();
  pid_t childpid;
  char ppid[12];
  int new_lb,k,fd3,fd2,lb,ub,Number_Of_Children, i,children;
  int new_ubound;
  double logos,max,min;
  for (i=0; i<argc; i++) {
    if (strcmp(argv[i],"-l")==0) lb = atoi(argv[i+1]);
    if (strcmp(argv[i],"-u")==0) ub = atoi(argv[i+1]);
    if (strcmp(argv[i],"-w")==0) Number_Of_Children = atoi(argv[i+1]);
  }
  struct  pollfd  fdarray [1];
  ListItem *foo;
  mkfifo("myfifo1", 0777);
  mkfifo("myfifo2", 0777);                                                          //Create the 3 named pipes
  mkfifo("myfifo3", 0777);
  fd2 = open("myfifo2", O_RDWR);
  if (ub < Number_Of_Children*Number_Of_Children) Number_Of_Children = sqrt(ub);
  for (children=1; children<=Number_Of_Children; children++) {
      childpid=fork();
      if (childpid==0) {
        logos=(double)(ub-lb)/(double)Number_Of_Children;
        if (logos==0) logos=1;
        new_lb = lb + (children-1)*logos;
        new_ubound = new_lb + logos;
        if (new_ubound > ub|| (new_ubound < ub && children == Number_Of_Children)) new_ubound = ub;
          char number1[12];
          char number2[12];
          char number3[12];                                                           //Get the pid of the root
          sprintf(number1, "%d", new_lb);
                                                                                                    //Make these numbers strings
          sprintf(number2, "%d", new_ubound);
          sprintf(number3, "%d", Number_Of_Children);
          execlp("./intermediate","./intermediate", number1, number2, number3, ppid, (char *) NULL);      //Exec the I node with the propper arguments
          perror("exec failure ");
        }
        else {
          struct sigaction sa = {0};
          sa.sa_handler = &func;                                          //Initialize sigaction
          sigaction(SIGUSR1, &sa, NULL);
          fd3 = open("myfifo3", O_RDONLY);                       //Open fifo to read
          fdarray[0].fd=fd3;
                                                                        //Initialize poll
          fdarray[0].events=POLLIN;
          int rc;
          rc = poll(fdarray, 1, 100000);
          if (rc==0) {
            printf("Polled time out from Myprime\n");
            exit(-1);
          }
          while (1) {
            if ((rc==1||rc==-1) && fdarray[0].events == POLLIN) {               //If there is something to read
                ListItem data;
                if (read(fd3, &data, sizeof(ListItem)) == 0) break;      //Break from the loop when there is nothing to read
                else list = LstInsert(list,data);                       //Insert the data that has just been read to a list
            }
          }
        }
      }
      bubbleSort(list);                                               //Sort the list
      removeDuplicates(list);                                         //Remove duplicates
      printf("Primes in[%d,%d] are:\n", lb,ub);
      while (list!=NULL) {
        if (list->item.prime>0) printf("%d in %f msecs\n", list->item.prime, 1000*list->item.time); //Print the results
        list = list->next;
      }
      printf("\n");
      for (k=0; k<Number_Of_Children*Number_Of_Children; k++) {
        ListItem data;
        read(fd2, &data, sizeof(ListItem));
        list_of_times = LstInsert(list_of_times,data);
      }
      bubbleSort(list_of_times);
      i=0;
      min = 10000.0;
      max=-1.0;
      while (list_of_times!=NULL) {
        printf("Time for W%d: W%d-time %f msecs\n", i, i,1000*list_of_times->item.time);     //Print the times of the workers
        if (list_of_times->item.time < min) min = list_of_times->item.time;
        if (list_of_times->item.time > max) max = list_of_times->item.time;
        i++;
        list_of_times = list_of_times->next;
      }

      printf("\n\n\n");
      printf("Min Time for Workers : %f msecs\n",1000*min );
      printf("Max Time for Workers : %f msecs\n",1000*max );
      printf("NUM of USR1 Received : %d/%d\n",counter, Number_Of_Children*Number_Of_Children );                                                              //Print the number of signals
      close(fd3);
                                                                                                                        //Close the pipes
      close(fd2);
      LstDestroy(&list);
                                                                                                                        //Free the pipes
      LstDestroy(&list_of_times);
}