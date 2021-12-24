#pragma once


extern int copied;
extern int bytes;
extern int files_or_directories;
extern int verbose;
extern int deleted;
extern int links;


int isDirectory(const char *path);
char* concat(const char *s1, const char *s2);
void copy_directories(char *origindir, char *destdir);
int areSame(char *path1, char* path2);
char *getFileCreationTime(char *path);
int path2_is_earlier_than_path1(char *path1,char *path2);
char *Exists(char *name,char *dict, int *f);
int ChangeTime(const char *oldfile, const char *newfile);
void copy_files(char *name, char *origindir, char *destdir);
void copy_directories(char *origindir, char *destdir);
void listFilesRecursively(char *basePath);
void clean_up(char *dir1, char *dir2);
int remove_directory(const char *path);
int IsLink(char *path);
void copy_links(char *name, char *origindir, char *destdir);
void HardLink(char *dir, char *name, int n, char *dir2);