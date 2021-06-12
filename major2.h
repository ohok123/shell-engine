#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>
#include <ctype.h>


struct HistoryList // history structure
{
	char histList[20][50]; // 20 entries 50 chars long
	int capacity; // growing capacity
};

struct HistoryList createHistory(); // initalize the history list with null
struct HistoryList addHistory(char entry[], struct HistoryList h1); // adding history entry
void History(struct HistoryList h1); // print the history
int CDcmd(char **args);
void bashInterpreter(char* fileName);//reads in fileName that should be a bash script
void my_pipeline(char cmd[]);
void setPath(char* str);//str is equal to PATH after function is called
void restorPath(char *str);//PATH is what ever string str is
void pathCommand2(char *pth, char *input);//pth is the path and input is print path, append to path  , remove from path
void sig_catch(int iSig);
void ignoreSig();
void restoreSig();
