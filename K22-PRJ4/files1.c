#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/types.h>
#include <utime.h>
#include "files.h"


enum year{Jan, Feb, Mar, Apr, May, Jun, Jul,
          Aug, Sep, Oct, Nov, Dec};

int verbose;

char *Exists(char *name,char *dict, int *f)             //If a file named name exists in  dict f becomes 1 and the realpath is retunred
{
  struct dirent *pDirent;
  DIR *pDir;
  char entry[100];
  pDir = opendir(dict);
  while ((pDirent = readdir(pDir)) != NULL) {
    if (strcmp(pDirent->d_name,name)==0) {            //If we find it
      *f = 1;
      strcpy (entry, dict);
      strcat (entry, "/");
      strcat (entry, pDirent->d_name);
      if (verbose == 1) printf("file %s exists in %s\n",name, dict );
      return realpath(entry,NULL);
    }
  }
  *f=0;
  return NULL;
}

char* concat(const char *s1, const char *s2)                      //It unites 2 strings
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

int isDirectory(const char *path) {               //Check if path is directory
   struct stat statbuf;
   if (stat(path, &statbuf) != 0)
       return 0;
   return S_ISDIR(statbuf.st_mode);
}


int ChangeTime(const char *oldfile, const char *newfile)      //The last modification time of newfile becomes the same as the one in oldfile
{

    struct stat buf1;
    struct stat buf2;
    struct timeval times[2];
    memset(times,0,sizeof(times));
    if (stat(oldfile, &buf1) != 0)
        return(-1);
    if (stat(newfile, &buf2) != 0)
        return(-1);
    struct utimbuf puttime;
    puttime.modtime = buf1.st_mtime;
    puttime.actime = buf1.st_atime;
    return utime(newfile, &puttime);
}

int areSame(char *path1, char* path2)                 //This function checks if path1 and path2 are same based on the homework requiremnets
{
  if ((isDirectory(path1) && !isDirectory(path2)) || (!isDirectory(path1) && isDirectory(path2))) return 0;     //If one is directory and the other not
  else if (isDirectory(path1) && isDirectory(path2)){                                                           //If both are directories reccur
        struct dirent *pDirent1, *pDirent2;
        char  *filename1, *filename11, *filename22, *filename2, *filename, path11[1000], path22[1000];
        DIR *pDir1, *pDir2;
        int f, result = 1;
        pDir1 = opendir(path1);
        pDir2 = opendir(path2);
        while ((pDirent1 = readdir(pDir1)) != NULL) {
          int found=0;
          if (strcmp(pDirent1->d_name, "..") !=0 && strcmp(pDirent1->d_name, ".") !=0) {        //Traverse first directory and for each file search in the second directory
            strcpy(path11, path1);
            strcat(path11, "/");
            strcat(path11, pDirent1->d_name);
            while ((pDirent2 = readdir(pDir2)) != NULL) {                       //Traverse the secon directory
              if (strcmp(pDirent2->d_name, "..") !=0 && strcmp(pDirent2->d_name, ".") !=0 && strcmp(pDirent1->d_name,pDirent2->d_name)==0) {
                found = 1;
                strcpy(path22, path2);
                strcat(path22, "/");
                strcat(path22, pDirent2->d_name);
                  f = areSame(path11,path22);
                  if (f==1) {
                    break;
                  }
                  result *= f;
                  if (result==0) return 0;    //If at least one time f becomes 0 it means that there is a file which is not common in both directories
              }
            }
            if (found==0) return 0;         //If we could not a file named in pDirent1->d_name in secont directory
          }
        }
        return 1;

  }
  else if (!isDirectory(path1) && !isDirectory(path2)){         //If none is a directory
    FILE *fp1, *fp2;
    fp1 = fopen(path1, "r");
    fp2 = fopen(path2, "r");
    int sz1, sz2;
    fseek(fp1, 0L, SEEK_END);
    sz1 = ftell(fp1);
    rewind(fp1);
    fseek(fp2, 0L, SEEK_END);
    sz2 = ftell(fp2);
    rewind(fp2);
    if (sz1!=sz2){                              //Check the size
       return 0;
     }
    else {
      if (path2_is_earlier_than_path1(path1,path2)) return 0;     //An now the dates
    }
  }
  return 1;
}

char *getFileCreationTime(char *path) {         //Get the last modification date of path
    struct stat attr;
    stat(path, &attr);
    return ctime(&attr.st_mtime);
}

int path2_is_earlier_than_path1(char *path1,char *path2)        //This function checks if path2 has been modificated earlier than path1
{
  char date1[50];
  char date2[50];
  int day1,day2,m1,m2;
  char month1[10], month2[10];
  int year1,year2;
  char time1[10], time2[10];
  char words[15][15];
  int i,j,ctr;
  strcpy(date1,getFileCreationTime(path1));   //Take the date of path1

  //Take the day, the month, the year and the time

  j=0; ctr=0;
  for(i=0;i<=(strlen(date1));i++){
      if(date1[i]==' '||date1[i]=='\0'){
          words[ctr][j]='\0';
          ctr++;
          j=0;
      }
      else{
          words[ctr][j]=date1[i];
          j++;
      }
  }
  day1 = atoi(words[2]);
  strcpy(month1, words[1]);
  year1 = atoi(words[4]);
  strcpy(time1, words[3]);

  //Do the same with oath2
  strcpy(date2,getFileCreationTime(path2));
  j=0; ctr=0;
  for(i=0;i<=(strlen(date2));i++){
      if(date2[i]==' '||date2[i]=='\0'){
          words[ctr][j]='\0';
          ctr++;
          j=0;
      }
      else{
          words[ctr][j]=date2[i];
          j++;
      }
  }
  day2 = atoi(words[2]);
  strcpy(month2, words[1]);
  year2 = atoi(words[4]);
  strcpy(time2, words[3]);

  //Now compare the 2 dates and find the earlier

  if (year2 < year1) return 1;
  else if (year1 == year2) {
    if (strcmp(month1,"Jan") == 0) m1 = Jan;
    else if (strcmp(month1,"Feb") == 0) m1 = Feb;
    else if (strcmp(month1,"Mar") == 0) m1 = Mar;
    else if (strcmp(month1,"Apr") == 0) m1 = Apr;
    else if (strcmp(month1,"May") == 0) m1 = May;
    else if (strcmp(month1,"Jun") == 0) m1 = Jun;
    else if (strcmp(month1,"Jul") == 0) m1 = Jul;
    else if (strcmp(month1,"Aug") == 0) m1 = Aug;
    else if (strcmp(month1,"Sep") == 0) m1 = Sep;
    else if (strcmp(month1,"Oct") == 0) m1 = Oct;
    else if (strcmp(month1,"Nov") == 0) m1 = Nov;
    else m1 = Dec;

    if (strcmp(month2,"Jan") == 0) m2 = Jan;
    else if (strcmp(month2,"Feb") == 0) m2 = Feb;
    else if (strcmp(month2,"Mar") == 0) m2 = Mar;
    else if (strcmp(month2,"Apr") == 0) m2 = Apr;
    else if (strcmp(month2,"May") == 0) m2 = May;
    else if (strcmp(month2,"Jun") == 0) m2 = Jun;
    else if (strcmp(month2,"Jul") == 0) m2 = Jul;
    else if (strcmp(month2,"Aug") == 0) m2 = Aug;
    else if (strcmp(month2,"Sep") == 0) m2 = Sep;
    else if (strcmp(month2,"Oct") == 0) m2 = Oct;
    else if (strcmp(month2,"Nov") == 0) m2 = Nov;
    else m2 = Dec;
    if (m2 < m1) return 1;
    else if (m1 == m2) {
      if (day2 < day1) return 1;
      else if (day2 == day1) {
        if (strcmp(time2, time1) < 0) return 1;
      }
    }
  }
  return 0;
}