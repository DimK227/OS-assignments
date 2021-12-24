#include	<stdio.h>
#include	<sys/types.h>
#include	<sys/wait.h>
#include	<sys/stat.h>
#include	<unistd.h>
#include	<string.h>
#include  <stdlib.h>
#include  <fcntl.h>
#include <time.h>
#include "math.h"
#include "list.h"
#define YES 1
#define NO  0

int k=2;

int SieveOfEratosthenes(int n)
{
    int prime[n];
    int i=0;
    int p;
    if (n==1) return NO;
    for (i=0; i<n; i++) prime[i]=1;
    prime[1] = 0;
    for (p=2; p*p*p*p<=n; p++) {                                                //Find all primes until square root
        if (prime[p] == 1){
            for (int i=p*p; i<=n; i += p)
                prime[i] = 0;
        }
    }
    for (i=1; i*i<=n; i++) {
      if (prime[i]==1){
          if (n%i==0) return NO;                                                //If the modulo == 0 it is not prime
      }
    }
    return YES;
    printf("\n");
}

int main(int argc, char *argv[]){
  int fd1,fd2,f=0;
  int primes = 0;
  int lb=0, ub=0, i=0;
  clock_t start,stop,start2,stop2;
  float total_time;
  start2 = clock();
  fd1 = open("myfifo1", O_WRONLY);
  fd2 = open("myfifo2", O_WRONLY);
  lb=atoi(argv[1]);
  ub=atoi(argv[2]);
  pid_t pid = atoi(argv[3]);
  if ( ( lb<1 )  || ( lb > ub ) ) {
          printf("usage: prime1 lb ub\n");
          exit(1); }

  for (i=lb ; i <= ub ; i++) {                                                  //For each number
    if (f==0) {
      start = clock();                                                          //Start the clock
      f=1;
    }
    if ( SieveOfEratosthenes(i)==YES ){                                         //If it is prime
        f=0;
        stop =clock();                                                          //Stop the clock
        double t = (double) (stop-start) / (CLOCKS_PER_SEC);
        ListItem data;
        data.time = t;
                                                                                //data.prime contains the prime number and data.time the time that was needed to find if its prime
        data.prime=i;
        write(fd1, &data, sizeof(ListItem));
    }
  }
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