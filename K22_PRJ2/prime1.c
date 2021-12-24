#include	<stdio.h>
#include	<sys/types.h>
#include	<sys/wait.h>
#include	<sys/stat.h>
#include	<unistd.h>
#include	<string.h>
#include  <stdlib.h>
#include  <fcntl.h>
#include <signal.h>
#include "list.h"
#include <time.h>
#define YES 1
#define NO  0


int k=0;

int prime(int n){
        int i;
        if (n==1) return(NO);
        for (i=2 ; i<n ; i++)
                if ( n % i == 0) return(NO);
        return(YES);
}



int main(int argc, char *argv[]){
  int fd1,fd2;
  int f=0;
  int lb=0, ub=0, i=0;
  listptr list_of_times;
  clock_t start,stop,start2,stop2;
  start2 = clock();
  fd1 = open("myfifo1", O_WRONLY);
  fd2 = open("myfifo2", O_WRONLY);
  lb=atoi(argv[1]);
  ub=atoi(argv[2]);
  if ( ( lb<1 )  || ( lb > ub ) ) {
          printf("usage: prime1 lb ub\n");
          exit(1); }

  for (i=lb ; i <= ub ; i++) {                          //For each number
    if (f==0) {
      start = clock();                                  //Start the clock
      f=1;
    }
    if ( prime(i)==YES ){                               //If it is prime
        f=0;
        stop =clock();                                  //Stop the clock
        double t = (double) (stop-start) / (CLOCKS_PER_SEC);
        ListItem data;
        data.prime = i;
                                                        //data.prime contains the prime number and data.time the time that was needed to find if its prime
        data.time = t;
        write(fd1, &data, sizeof(ListItem));
    }
  }
  pid_t pid = atoi(argv[3]);                          //Take the root pid
  kill(pid, SIGUSR1);                                 //Send the signal
  stop2 = clock();
  ListItem item;
  item.prime=getpid();
                                                  //Create the item (item.prime now represents the pid of this process and item.time the duration of this worker)
  item.time=(double) (stop2-start2) / (CLOCKS_PER_SEC);
  write(fd2, &item, sizeof(ListItem));            //Write it to the pipe
  close(fd1);
  close(fd2);
  return 0;
}