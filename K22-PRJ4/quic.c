#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/types.h>
#include <utime.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include "files.h"



int first = 0;
int bytes = 0;
int copied = 0;
int files_or_directories = 0;
int verbose = 0;
int deleted = 0;
int links = 0;

int main(int argc, char *argv[])
{
  int i;
  char *origindir, *destdir;
  clock_t start,stop;

  //Deal with the program arguments

  origindir = argv[argc - 2];
  destdir = argv[argc - 1];
  for (i = 1; i < argc - 2; i++) {
    if (strcmp(argv[i], "-v") == 0) verbose = 1;
    else if (strcmp(argv[i], "-d") == 0) deleted = 1;
    else if (strcmp(argv[i], "-l") == 0) links = 1;
  }
  start = clock();
  if (deleted == 1) clean_up(destdir, origindir);                 //Delete the file that exists in destdir and not in origindir
  copy_directories(origindir, destdir);                           //Copy the directories
  stop = clock();
  double t = (double) (stop-start) / (CLOCKS_PER_SEC);
  listFilesRecursively(destdir);                                                          //Count how many files does the destination directory has
  printf("\n\nthere are %d files/directories in the hierarchy\n", files_or_directories);
  printf("number of entities copied is %d\n", copied);                                      //Print the statistics
  printf("copied %d bytes in %f sec at %f bytes/sec\n", bytes, t, (float)bytes/t);
  return 0;
}

void copy_directories(char *origindir, char *destdir)         //This function copies origindir to destdir
{
  first++;
  struct dirent *pDirent;
  DIR *pDir, *pDir2;
  FILE *fp1;
  FILE* fp2;
  int f;
  char  *filename1, *filename11, *filename22, *filename2, *filename;
  pDir = opendir(origindir);
  if (pDir == NULL) {                           //If there is no origindir exit the program
     printf("%s\n", origindir);
     printf("No such directory\n");
     exit(-1);
   }

   pDir2 = opendir(destdir);
   if (pDir2 == NULL) {               //If there is no destdir create it
      mkdir(destdir, 0777);
      if (first == 1) printf("created directory %s\n", destdir);
      pDir2 = opendir(destdir);
    }

   while ((pDirent = readdir(pDir)) != NULL) {                //Traverse the origindir
      struct stat buf;
      if  (strcmp(pDirent->d_name, ".") != 0 && strcmp(pDirent->d_name, "..") != 0)  {            //We don't want to recur on . and ..
        filename = Exists(pDirent->d_name,realpath(destdir,NULL), &f);                            //Check if a file named pDirent->d_name exists in destdir and if so get the realpath of it (Check Exists on files1.c)
        filename11 = origindir;
        filename22 = concat(filename11,"/");
        filename1 = concat(filename22, pDirent->d_name);
        stat(realpath(filename1,NULL),&buf);
        if (f==0){                                                              //If it does not exist
          if (isDirectory(filename1)){                                          //If we have directory reccur
                filename11 = destdir;
                filename22 = concat(filename11,"/");
                filename2 = concat(filename22, pDirent->d_name);
                printf("%s\n", filename1);
                copied++;
                copy_directories(filename1, filename2);
          }
          else if (buf.st_nlink > 1) {                                          //If we have hard link
            if (links == 1) HardLink(origindir,pDirent->d_name, buf.st_ino,destdir);
          }
          else if (IsLink(filename1)) {                                         //If we have soft link
             if (links == 1) copy_links(pDirent->d_name, origindir, destdir);
          }
          else copy_files(pDirent->d_name, origindir,destdir);                  //If we have simple files
        }
        else {                                                                  //If it exists
          if (areSame(filename1,filename)) continue;                            //If they are same skip the process and continue with the next file
          else {
            if (isDirectory(realpath(filename1,NULL))) {
              filename11 = destdir;
              filename22 = concat(filename11,"/");
              filename2 = concat(filename22, pDirent->d_name);
              copy_directories(filename1, filename2);
            }
            else {
              copy_files(pDirent->d_name, origindir,destdir);
            }
          }
        }
      }
    }
    closedir (pDir);
}