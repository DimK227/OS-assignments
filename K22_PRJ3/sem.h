#pragma once

/*extern FILE *fp1;
extern char *name1;*/

typedef struct{
  int pid;
  int salads;
} saladmaker_stats;

char* concat(const char *s1, const char *s2);
void P(sem_t *s);
void V(sem_t *s);