#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/types.h>
#include <utime.h>
#include <fcntl.h>
#include <unistd.h>
#include "files.h"


int bytes;
int copied;
int files_or_directories;
int verbose;

void copy_links(char *name, char *origindir, char *destdir)           //Create a soft link named name in destdir
{
  if (verbose == 1) printf("Time to create soft link named %s in %s\n", name, destdir);
  copied++;
  char  *dir, *dir1, *filename11, *filename1,  *filename22, *filename2;
  filename11 = destdir;
  filename22 = concat(filename11,"/");
  filename1 = concat(filename22, name);

  filename11 = origindir;
  filename22 = concat(filename11,"/");
  filename2 = concat(filename22, name);

  dir = realpath(origindir,NULL);
  dir = concat(dir,"/");


  if(strstr(realpath(filename2,NULL), dir) != NULL) {       //If the soft link points to file in source directory (if dir exists in the real path of filename2)

    dir1 = realpath(destdir,NULL);
    dir1 = concat(dir1, "/");


    char *word = realpath(filename2,NULL);
    int x,index,i,j=0;
    char new[30];
    //Take the name of the file that the soft link points
    for (x = strlen(word); x >= 0; --x) {
      if (word[x] == '/') {
        index = x;
        break;
      }
    }
    for (i=index+1; i <strlen(word); i++) {
      new[j] = word[i];
      j++;
    }

    dir1 = concat(dir1, new);
    symlink(dir1,filename1);          //The link points to the corresponding file of destination directory

  }

  else symlink(realpath(filename2,NULL),filename1);   //Else we don't have a problem
}



void HardLink(char *dir, char *name, int n, char *dir2)           //Create hard link
{
  copied++;
  if (verbose == 1) printf("Time to create hard link named %s in %s\n", name, dir2);
  struct dirent *pDirent;
  DIR *pDir;
  char *file, *file1, *file2;
  pDir = opendir(dir);
  file = concat(dir2,"/");
  file = concat(file,name);
  while ((pDirent = readdir(pDir)) != NULL) {
    struct stat buf;
    if  (strcmp(pDirent->d_name, ".") != 0 && strcmp(pDirent->d_name, "..") != 0 && strcmp(pDirent->d_name, name) != 0)  {
      file1 = concat(dir,"/");
      file1 = concat(file1,pDirent->d_name);
      stat(realpath(file1,NULL),&buf);
      if (buf.st_nlink > 1 && buf.st_ino == n) {        //Find the file that is connected with hardlink with the file named name
            file2 = concat(dir2,"/");
            file2 = concat(file2,pDirent->d_name);
            copy_files(pDirent->d_name,dir,dir2);
            link(realpath(file2,NULL), file);           //Link
      }
    }
  }
}




void copy_files(char *name, char *origindir, char *destdir)   //Create a file named name in destdir that is a copy of file named name in origindir
{
   copied++;
   FILE *fptr1, *fptr2;
   char  c, *filename11, *filename1,  *filename22, *filename2;
   filename11 = origindir;
   filename22 = concat(filename11,"/");
   filename1 = concat(filename22, name);
   printf("%s\n", filename1);
   fptr1 = fopen(filename1, "r");  // Open one file for reading
   if (fptr1 == NULL) return;

   filename11 = destdir;
   filename22 = concat(filename11,"/");
   filename2 = concat(filename22, name);

   fptr2 = fopen(filename2, "w");  // Open another file for writing
   if (fptr2 == NULL) return;
   c = fgetc(fptr1);   // Read contents from file
   while (c != EOF){
       fputc(c, fptr2);
       c = fgetc(fptr1);
       bytes += sizeof(c);
   }

   fclose(fptr1);
   fclose(fptr2);
   ChangeTime(filename1, filename2);
}


void clean_up(char *dir1, char *dir2)           //Delete the files that exists in dir2 and not in dir1
{
  if (verbose == 1) printf("Time to delete all files that exist in %s and not in %s\n", dir1, dir2);
  struct dirent *dp;
  int f;
  DIR *dir = opendir(dir1);
  char *filename11, *filename1,  *filename22, *filename2, *filename;
  if (!dir) return;
  while ((dp = readdir(dir)) != NULL) {         //Travere the dir1
    if  (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
        filename = Exists(dp->d_name, dir2,&f);     //Check if file exists in dir2
        filename11 = dir1;
        filename22 = concat(filename11,"/");
        filename1 = concat(filename22, dp->d_name);
        if (f==0) {                                 //If it does not exist
          if (isDirectory(filename1)) remove_directory(filename1);      //If it is a directory delete the directory
          else remove(filename1);           //else remove the file
        }
        //If it exists
        else if (f==1 && isDirectory(filename1) && isDirectory(filename)) clean_up(filename1, filename);    //If both are directories reccur
        else if (f==1 && isDirectory(filename1) && !isDirectory(filename)) remove_directory(filename1);     //If one is directory relete it
        else if (f==1 && !isDirectory(filename1) && isDirectory(filename)) remove(filename1);                //If it is file delete it
      }
  }
}


void listFilesRecursively(char *basePath)       //Counts how many files does a directory contains
{
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);
    if (!dir) return;
    while ((dp = readdir(dir)) != NULL) {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
            files_or_directories++;
            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);
            listFilesRecursively(path);
        }
    }

    closedir(dir);
}



int IsLink(char *path)        //Checks if a path is a soft link
{
    struct stat buf;
    int x;
    x = lstat (path, &buf);
    if (S_ISLNK(buf.st_mode)) return 1;
    else return 0;
}



int remove_directory(const char *path) {            //It deletes a directory
   DIR *d = opendir(path);
   size_t path_len = strlen(path);
   int r = -1;

   if (d) {
      struct dirent *p;
      r = 0;
      while (!r && (p=readdir(d))) {
          int r2 = -1;
          char *buf;
          size_t len;
          if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, "..")) continue;/* Skip the names "." and ".." as we don't want to recurse on them. */
          len = path_len + strlen(p->d_name) + 2;
          buf = malloc(len);
          if (buf) {
             struct stat statbuf;
             snprintf(buf, len, "%s/%s", path, p->d_name);
             if (!stat(buf, &statbuf)) {
                if (S_ISDIR(statbuf.st_mode)) r2 = remove_directory(buf);
                else r2 = unlink(buf);
             }
             free(buf);
          }
          r = r2;
      }
      closedir(d);
   }
   if (!r) r = rmdir(path);
   if (verbose == 1) printf("directory %s has been deleted\n", path);
}