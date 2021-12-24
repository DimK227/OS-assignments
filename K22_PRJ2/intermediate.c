#include	<stdio.h>
#include	<sys/types.h>
#include	<sys/wait.h>
#include	<sys/stat.h>
#include	<unistd.h>
#include	<string.h>
#include  <stdlib.h>
#include  <fcntl.h>
#include <signal.h>
#include <poll.h>
#include "list.h"



int main(int argc, char *argv[])
{
  pid_t childpid;
  listptr list = LstCreate();
  int new_lb,new_ub=0,total_size=0,size,fd1,fd2,fd3,fd4,lb = atoi(argv[1]),ub = atoi(argv[2]), Number_Of_Children = atoi(argv[3]), child ,i;
  ListItem *foo;
  double logos;
  char ppid[12];
  struct pollfd fdarray[1];
  fd3 = open("myfifo3", O_WRONLY);
  pid_t first = getppid();
  sprintf(ppid, "%d", getppid());
  for (child=1; child<=Number_Of_Children; child++) {
      childpid=fork();
      if (childpid== -1){
          perror("Failed to fork\n");
          exit(1);
        }
        if (childpid==0) {
          logos=(ub-lb)/Number_Of_Children;
          if (logos==0) logos=1;
          new_lb = lb + (child-1)*logos;
          new_ub = new_lb +logos;                                                             //Find the lowest number and the highest number that we will check for primes
          if ((new_ub > ub) || (new_ub < ub && child == Number_Of_Children)) new_ub = ub;
          if (((ub-lb)/Number_Of_Children) == 0 && new_lb!=new_ub){new_lb = new_ub;}
          char number1[12];
          char number2[12];
          char id[12];
          sprintf(number1, "%d", new_lb);
          //printf("INTERMEDIATE Gia n = %d  lb = %d and ub = %d\n", child, new_lb, new_ub);                                                              //Make these numbers strings
          sprintf(number2, "%d", new_ub);
          //printf("intermediate gia %d einai lb=\n", );
          if (child%3==1) execlp("./prime1","./prime1",number1, number2,ppid,(char *) NULL);
          else if (child%3==2) execlp("./prime2","./prime2",number1, number2,ppid,(char *) NULL);                    //Execute thr right prime finding program
          else execlp("./prime3","./prime3",number1, number2,ppid ,(char *) NULL);
          exit(1);
        }
        else {
          fd1 = open("myfifo1", O_RDONLY);
          fdarray[0].fd=fd1;
          fdarray[0].events=POLLIN;
          int rc;                                                                               //Initialize the poll
          rc = poll(fdarray, 1, 10000);
          if (rc==0) {
            printf("Polled time out from intermediate\n");
            exit(-1);
          }
          while (1) {
          if (rc==1 && fdarray[0].events == POLLIN) {                                 //If there is something to read
              ListItem data;
              if (read(fd1, &data, sizeof(ListItem)) == 0) break;                      //Break from the loop when there is nothing to read
              else list = LstInsert(list,data);                                        //Insert the data that has just been read to a list
            }
          }
          bubbleSort(list);                                                           //Sort the list
          removeDuplicates(list);                                                     //Remove duplicates
          while (list!=NULL) {
            write(fd3, &(list->item), sizeof(ListItem));                              //Write to the pipe all the datas
            list = list->next;
          }
        }
  }
  close(fd3);
  close(fd1);
}