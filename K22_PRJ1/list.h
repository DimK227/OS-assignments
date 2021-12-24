typedef struct listnode *listptr;
typedef struct YearNode *YearNodePtr;
typedef struct StudentNode *StudentNodePtr;
#define MAXSIZE 20

typedef struct {
  char *studentID;
  char *fname;
  char *lname;
  int zip;
  int year;
  float average;
}ListItem;


struct element {
	char *key;
	ListItem item;
};

struct listnode{
        struct element info;
        listptr next;
};

struct YearNode{
	int year;
	StudentNodePtr students;
	YearNodePtr next;
};

struct StudentNode{
  listptr info;
	StudentNodePtr nextStudent;
};



void PrintMinAv(StudentNodePtr list, int year);
void PrintCountOfEachYear(YearNodePtr list);
void PrintRank(YearNodePtr list, int n);
int Get_TotalCount(YearNodePtr list);
void LstPrintN(StudentNodePtr list, int n, int year);
float LstAverage(StudentNodePtr list);
StudentNodePtr GetStudents(YearNodePtr list, int year);
listptr LstCreate();
YearNodePtr YNCreate();
YearNodePtr YNInsert(YearNodePtr list, listptr node, int year);
listptr LstInsert(listptr list,char *key,ListItem item);
listptr LstGetNode(listptr list, char *key);
int LstCount(StudentNodePtr list);
int LstSearch(listptr list,char *key, ListItem *pitem);
listptr LstDelete(listptr list, char *node);
void LstDestroy(listptr *list);
void YNDestroy(YearNodePtr *list);
void SNDestroy(StudentNodePtr *list);
