#include "major2.h"

void setPath(char* str)				//takes in a pointer to astring and gives it PATH
{
	char* s = getenv("PATH");		//get path
        strcpy(str, s);				//copy it
}
void restorPath(char *str)			//used to restore path to what ever str is, keep a copy of origin in main to restore it
{
	char temp[900];				//to use putenv format is "PATH={PATHNAME}"
	strcpy(temp, "PATH=");
	strcat(temp, str);
	if(putenv(temp) == -1)
		{
		printf("Restoring path failed\n");
		exit(1);
		}
}
void pathCommand2(char *pth, char *input)
{
	//pth will keep changed path, also holds current path
	//input is the user input
	char temp[900];						//hold a copy of a string
	char hold[3][900] = {"\0"};				//array that holds the 3 inputs path +/- " "
	char *pch;						//used to search for character
	int inputs = 0;						//number of user inputs
	strcpy(temp, input);					//make a copy
	pch = strtok(temp, " ");				//get parts that are seperated by a space
	strcpy(hold[0], pch);					//first is path
	while (pch != NULL)					//loop to get all parts, should be 2 more 
		{
		pch = strtok(NULL, " \n");
		if(pch != NULL)					//was getting seg faults so use this to avoid it
			{
			inputs++;				//incrsase by 1 
			strcpy(hold[inputs], pch);		//add to array
			}
		}
	if(inputs == 0)						//if zero then its just path so print path
		{
		printf("%s\n", pth);
		return;
		}
	else if(inputs == 2)					//if count is 2 then perform append or remove
		{
		if(strcmp(hold[1], "-") == 0)			//if - then remove
			{
			char hold2[900]= {"\0"};		//store path 
			char hold3[900]= {"\0"};		//store new path after section removed
			int counter = 0;			//keep track of number of ':' in path
			strcpy(hold2, pth);			//make a copy
			pch = strchr(hold2, ':');		//get first occurance of ':'
			while(pch != NULL)			//loop and get number of ':'
				{
				counter++;	
				pch = strchr(pch + 1, ':');	//search for next one
				}
			int countLoop = 0;			//for following while looop
			pch = strtok(hold2, ":");		//get first part 
			if(strcmp(pch, hold[2]) != 0)		//check if part that is to be removed, if not then cat it to hold3
				{
				strcat(hold3, pch);
				}
			while (countLoop < counter)		//do for rest of path
				{
				pch = strtok(NULL, ":");	//get next part
				if(strcmp(pch, hold[2]) != 0)	//check if parth that is getting removed
				{
				strcat(hold3, ":");		//if not then add ':'
				strcat(hold3, pch);		//add path to parth3
				}
				countLoop++;			//increase counter
				}
			strcpy(pth, hold3);			//copy hold3 to pth, pth is now the new path
			}

		else if(strcmp(hold[1], "+") == 0)		//append to path
			{
			strcat(pth, ":");			//add ':' at end of path
			strcat(pth, hold[2]);			//cat hold[2] which has what user wants to append
			}
		else						//no plus or minus found 
			{
			printf("path, path + \"./bin\", or path - \"./bin\" not entered. Unable to perform command\n");
			return;
			}
		}
	else							//user entered to many or not enough arguments
	{
	printf("Invalid number of arguments\n");
	return;
	}
}
