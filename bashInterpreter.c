#include "major2.h"
/*Reads in a bash file
 * <variable name>=<value>  to read in a variable
 * <variable name>=<operand1><operator><operand2>   do an opperation and store in a variable
 * display <string> 	display a string to terminal, use $A for a variable replacing A with variable 
 * # Some comment  if line starts with # then line is ignored
 * */
void bashInterpreter(char* fileName)
	{
	char holdInput[50];						//store line from file
	char holdInput2[50];						//used to store a copy of a c string
	char holdInput3[50];
	char temp[50] = {"\0"};                                                  //used for storing a c string
	char *tken;							//for strtok
	char *pch;							//used for searching for a char in c string
	char * line = NULL;						//for reading from file
	char holdVarName[30][30] = {"\0"};				//used for holding 30 different variables
	char holdString[30][30] = {"\0"};				//if variable holds a string then it is stored here
	int holdType[30];						//0 is not used , 1 is a integer, 2 is a string
	for(int i = 0; i < 30; i++)					//loop through holdType array and set them to 0
	{
	holdType[i] = 0;
	}
	int holdValue[30];						//if variable holds an integer then it is stored here
	int arrPos = 0;							//hold position in array
	int arrSize = 0;						//holds size of array
	int tempNum;							//hold an integer
	int isInList = 0, beforePlus = 0, count = 0;			//isInList is 0 for not and 1 if already in list, beforePlus is 0 if a variable is before 1 is after
									//count keeps track of variables in line from file, varInList is 0 if not in list and 1 if it is
    	int varInList = 0, alreadyPrint = 0;				//varInList is 0 if not in list and 1 if it is, alreadyPrint 1 for printed and 0 for not
									
	int opd1, opd2;							//opd1 is opperand 1 for arithmetic and opd2 is second opperand
	size_t len = 0;							//for reading in from file
	ssize_t read;							//for reading in from file, got it from stackover flow
	FILE *fp;							//pointer for file

	fp = fopen(fileName, "r");					//open up file and read
	if(fp == NULL)
	{
		perror("Error opening file");
	}
	while((read = getline(&line, &len, fp)) != -1)			//get first line of numbers
	{
		strncpy(holdInput3, line, 7);				//get first seven characters from line to check if they are the word "display:
		holdInput3[7] = '\0';					//add null terminator
                if(line[0] == '#')					//check if comment, if so then ignore
		{
			;//printf("found a comment\n");
                }
		else if((strcmp(holdInput3, "display") == 0))		//if display then do the following
		{
			strcpy(holdInput, line);			//make a copy of line
			isInList = 0;					//set to 0, goes to 1 if variable in list of variables
			if(((strchr(line, '"')) != NULL) && ((strchr(line, '$')) != NULL))//if displaying a string with a variable then do the following
				{
				tken = strtok(line, "$");		//get first variable
                                tken = strtok(NULL, "\"");		//get rid of newline
                                for(int i = 0; i < arrSize; i++)	//loop through array of variables
                                        {
                                        if(strcmp(holdVarName[i], tken) == 0)	//if variable in array then set arrPos to the place in array
                                                        {
                                                        isInList = 1;	//set to 1 to mean that its a valid variable
                                                        arrPos = i;	//set arrPos to place in array
                                                        }
                                        }
				strcpy(line, holdInput);		//revert line back
				tken = strtok(line, "\"");		//get data in ""
                                tken = strtok(NULL, "\"");		//get second "

				for(int i = 0; i < strlen(tken);i++)	//iterate though " "
					{
					alreadyPrint = 0;		//reset to 0 so that it means it hasn't been printed
					if((tken[i] == '$'))//	&& (holdType[arrPos] == 2))
						{
						for(int j = 0; j < arrSize; j++)	//iterate though list of variables 
						{
							if(( strcmp(holdVarName[j], &tken[i+1]) == 0) && (alreadyPrint == 0))	//if variable in list then and not printed yet then do the following
							{
								if((holdType[arrPos] == 2) && (alreadyPrint == 0))		//if valueType is 2 then its a string so do following
								{
									printf("%s" , holdString[arrPos]);			//print out string
									alreadyPrint++;						//set alreadyPrint to true(1)
									i++;							//skip next character since it is variable
								}
								if((holdType[arrPos] == 1) && (alreadyPrint == 0))		//if valueType is 1 then its an integer to do following
                                                                {
                                                                        printf("%d" , holdValue[arrPos]);			//print out integer in array
                                                                        alreadyPrint++;						//set alreadyPrint to true(1)
									i++;							//skip next character since it is variable
                                                                }
							}
						}
						if(alreadyPrint == 0)						//variable not found so alert user
						{
							printf("Variable not found\n");
						}
						alreadyPrint = 0;						//reset alreadyPrint for next loop
                                                }
					else							//not a variable so print out character
					{
						printf("%c", tken[i]);
					}
						
					}
				printf("\n");							//newline for next line
				}
			else if(((strchr(line, '$')) != NULL))					//if only a variable then print out following
				{
				isInList = 0;							//set to 0 so that it means variable is not in list
				tken = strtok(line, "$");					//get variable
				tken = strtok(NULL, "\n");					//get rid of newline

				for(int k = 0; k < arrSize; k++)				//iterate through array of variables looking for tken
                                        {
					if(strchr(tken, holdVarName[k][0]) != NULL)			//if found then do following 
                                                        {
                                                        isInList = 1;				//set to 1, means found
                                                        arrPos = k;				//set arrPos to position of variable in array
                                                        }
                                        }
				if((holdType[arrPos] == 1) && (isInList == 1))			//if holdType is 1 and in list ,then print out integer using holdValue array
				{
					printf("%d", holdValue[arrPos]);
				}
				else if((holdType[arrPos] == 2)&& (isInList == 1))		//if holdType is 2 and in list ,then print out string using holdString array
				{
					printf("%s", holdString[arrPos]);
				}
				else								//not found so alert user
				{
					printf("VARIABLE NOT ASSIGNED");
				}
				printf("\n");							//display done so endline
				}
			else if(((strchr(line, '"')) != NULL))					//only a string
                                {
                                tken = strtok(line, "\"");					//get first "
				tken = strtok(NULL, "\"");					//get second "
				printf("%s\n", tken);						//print it out with out ""
                                }
			else									//display wasn't a string or a variable so alert user
				{
				printf("Wrong format for display, line will be ignored\n");
				}
		}
		else if(strchr(line, '=') != NULL)						//equal found so it an assignment
		{
			if((strchr(line, '+')) != NULL) 					//plus found so do addition
				{
				isInList = 0;							//set to 0, if in list then change to 1
                                arrPos = arrSize;						//have arrPos be last position in variable array
				strcpy(holdInput, line);					//copy line to holdInput
				tken = strtok(line, "=");					//get variable that will hold sum
                                for(int i = 0; i < arrSize; i++)				//loop through array of variables
                                        {
                                        if(strcmp(holdVarName[i], tken) == 0)			//if found then set isInList to 1 and have arrPos equal this position in array
                                                        {
                                                        isInList = 1;
                                                        arrPos = i;
                                                        }
                                        }
                                if(isInList == 0)						//if not in variable not in array then add it
                                        {
                                        strcpy(&holdVarName[arrPos][0], tken);
                                        arrSize++;
                                        }

				if(strchr(holdInput, '$') == NULL)				//if no variable found then turn strings to integers and add them
					{
					strcpy(line, holdInput);				//make a copy
					tken = strtok(line, "=");				//get first number
					tken = strtok(NULL, "+");
					strcpy(temp, tken);					//make a copy
                                        tempNum = atoi(temp);					//use atoi to get integer
                                        opd1 = tempNum;						//have opd1 equal tempNum
					tken = strtok(NULL, "\n");				//get second number
					strcpy(temp, tken);					//make a copy
                                        tempNum = atoi(temp);					//use atoi to get integer
                                        opd2 = tempNum;						//have opd2 equal tempNum
					holdType[arrPos] = 1;					//make that place in array be an integer
					holdValue[arrPos] = opd2 + opd1;			//store the sum into array
					}
				else								//1 or 2 variables
					{
					beforePlus = 0;						//use to check were variable is, 1 means variable after +
					pch = strchr(holdInput, '$');				//loop for variable
					if(pch > strchr(holdInput, '+'))			//if after set to 1
						{
						beforePlus++;
						}
					count = 0;						//keep track of variables
					while(pch != NULL)					//loop through holdInput
						{
						count++;
						pch = strchr(pch + 1, '$');			//look for next variable
						}
					if(count == 2)						//two variables found so do following
						{
						varInList = 0;	//2 is that both var are in list
						//following section gets first varibles, looks for in array, if found then set opd1 to it, and increase varInList by 1
						strcpy(line, holdInput);			
						tken = strtok(line, "$");
                                                tken = strtok(NULL, "+");
						strcpy(holdInput2, tken);
						for(int i = 0; i < arrSize; i++)
							{
							if(strcmp(holdVarName[i], tken) == 0)
								{	
								if(holdType[i] == 1)
									{
									opd1 = holdValue[i];
									varInList++;
									}
								}
							}
						////following section gets first varibles, looks for in array, if found then set opd1 to it, and increase varInList by 1
						strcpy(line, holdInput);
						tken = strtok(line, "$");
						tken = strtok(NULL, "$");
						tken = strtok(NULL, "\n");
						for(int i = 0; i < arrSize; i++)
                                                        {
                                                        if(strcmp(holdVarName[i], tken) == 0)
                                                                {
								if(holdType[i] == 1)		//check if type of variable is an integer
									{
                                                                	varInList++;
                                                                	opd2 = holdValue[i];
									}
                                                                }
                                                        }
						if(varInList == 2)				//both variables in list so add them and store in array
							{
							holdValue[arrPos] = (opd2 + opd1);
							holdType[arrPos] = 1;
							}
						else						//variables not found so alert user
							{
							printf("One of these two variables is not initiated or is a string, %s or %s\n", holdInput2, tken);
							}
						}
					else if(beforePlus == 1)				//1 variable and its after +
						{
						//gets first number, use atoi to change to integer and have opd1 equal it
						strcpy(line, holdInput);
                                                tken = strtok(line, "=");
                                                tken = strtok(NULL, "+");
						strcpy(temp, tken);
                                        	tempNum = atoi(temp);
						opd1 = tempNum;
						//gets second variable and searchs through list of variables looking for it, if found then set opd2 to its value
                                                strcpy(line, holdInput);
						tken = strtok(line, "$");
                                                tken = strtok(NULL, "\n");

						for(int i = 0; i < arrSize; i++)
                                                        {
                                                        if(strcmp(holdVarName[i], tken) == 0)
                                                                {
                                                                if(holdType[i] == 1)		//check if type of variable is an integer
                                                                        {
                                                                        varInList++;
                                                                        opd2 = holdValue[i];
                                                                        }
                                                                }
                                                        }
						if(varInList == 1)				//if variable is valid then have sum be added to array 
                                                        {
                                                        holdValue[arrPos] = (opd2 + opd1);
                                                        holdType[arrPos] = 1;
                                                        }
                                                else						//variable not found so alert user
                                                        {
                                                        printf("This variable is not initiated or is a string, %s\n", tken);
                                                        }
						}
					else							//variable before plus so do following 
						{
						varInList = 0;  //1 is that its in list
						//gets first variable and searchs through list of variables looking for it, if found then set opd2 to its value
						strcpy(line, holdInput);
                                                tken = strtok(line, "$");
                                                tken = strtok(NULL, "+");
						for(int i = 0; i < arrSize; i++)
                                                        {
                                                        if(strcmp(holdVarName[i], tken) == 0)
                                                                {
                                                                if(holdType[i] == 1)
                                                                        {
                                                                        varInList++;
                                                                        opd2 = holdValue[i];
                                                                        }
                                                                }
                                                        }	
						//gets second number, use atoi to change to integer and have opd1 equal it			
						strcpy(line, holdInput);
                                                tken = strtok(line, "+");
                                                tken = strtok(NULL, "\n");
						strcpy(temp, tken);
                                                tempNum = atoi(temp);
                                                opd1 = tempNum;
						if(varInList == 1)				//if variable is valid then have sum be added to array
                                                        {
                                                        holdValue[arrPos] = (opd2 + opd1);
                                                        holdType[arrPos] = 1;
                                                        }
                                                else						//variable not found so alert user
                                                        {
                                                        printf("This variable is not initiated or is a string, %s\n", tken);
                                                        }
						}
					}
				}
//********************************************************************************************************************************************
			else if((strchr(line, '-')) != NULL)
				{				
				isInList = 0;
				//get variable before equal, check if its in array of variables, if not then add it to array
                                arrPos = arrSize;
				strcpy(holdInput, line);
				tken = strtok(line, "=");
                                for(int i = 0; i < arrSize; i++)
                                        {
                                        if(strcmp(holdVarName[i], tken) == 0)
                                                        {
                                                        isInList = 1;
                                                        arrPos = i;
                                                        }
                                        }
                                if(isInList == 0)
                                        {
                                        strcpy(&holdVarName[arrPos][0], tken);
                                        arrSize++;
                                        }
				if(strchr(holdInput, '$') == NULL)		//if no variables then do the following
					{
					strcpy(line, holdInput);		//restore line
					//get first number, use atoi to make it an integer
					tken = strtok(line, "=");
					tken = strtok(NULL, "-");
					strcpy(temp, tken);
                                        tempNum = atoi(temp);
                                        opd1 = tempNum;
					//get second number, use atoi to make it an integer
					tken = strtok(NULL, "\n");
					strcpy(temp, tken);
                                        tempNum = atoi(temp);
                                        opd2 = tempNum;
					holdType[arrPos] = 1;			//set value type to 1 since integer
					holdValue[arrPos] = opd1 - opd2;	//add both and put in array of integers 
					}
				else						//if variable then check if befoer plus or after
					{
					beforePlus = 0;				//0 before , 1 is variable after plus
					pch = strchr(holdInput, '$');		//get location of 1 variable
					if(pch > strchr(holdInput, '-'))	//check if afer minus, if so then set beforePlus to 1
						{
						beforePlus++;
						}
					count = 0;				//keep track of variables in line
					while(pch != NULL)
						{
						count++;
						pch = strchr(pch + 1, '$');	//get next variable
						}
					if(count == 2)				//if two variables found then do following
						{
						varInList = 0;	//2 is that both var are in list
						//get first variable, check if it in list and try to get its value
						strcpy(line, holdInput);
						tken = strtok(line, "$");
                                                tken = strtok(NULL, "-");
						strcpy(holdInput2, tken);
						for(int i = 0; i < arrSize; i++)
							{
							if(strcmp(holdVarName[i], tken) == 0)
								{
								if(holdType[i] == 1)
									{
									opd1 = holdValue[i];
									varInList++;
									}
								}
							}
						//get second variable, check if it in list and try to get its value
						strcpy(line, holdInput);
						tken = strtok(line, "$");
						tken = strtok(NULL, "$");
						tken = strtok(NULL, "\n");
						for(int i = 0; i < arrSize; i++)
                                                        {
                                                        if(strcmp(holdVarName[i], tken) == 0)
                                                                {
								if(holdType[i] == 1)
									{
                                                                	varInList++;
                                                                	opd2 = holdValue[i];
									}
                                                                }
                                                        }
						if(varInList == 2)		//if 2 variables then perform opperation and put value in holdValue
							{
							holdValue[arrPos] = (opd1 - opd2);
							holdType[arrPos] = 1;
							}
						else				//if 1 of the two not in array then print out message
							{
							printf("One of these two variables is not initiated or is a string, %s or %s\n", holdInput2, tken);
							}				
						}
					else if(beforePlus == 1)	//1 variable and it after plus
						{
						varInList = 0;  //1 is that its in list
						//get first number, use atoi to change to integer and store it in opd1
						strcpy(line, holdInput);
                                                tken = strtok(line, "=");
                                                tken = strtok(NULL, "-");
						strcpy(temp, tken);
                                        	tempNum = atoi(temp);
						opd1 = tempNum;
						//get variable, check if a valid variable, it valid store in opd2
                                                strcpy(line, holdInput);
						tken = strtok(line, "$");
                                                tken = strtok(NULL, "\n");

						for(int i = 0; i < arrSize; i++)
                                                        {
                                                        if(strcmp(holdVarName[i], tken) == 0)
                                                                {
                                                                if(holdType[i] == 1)
                                                                        {
                                                                        varInList++;
                                                                        opd2 = holdValue[i];
                                                                        }
                                                                }
                                                        }
						if(varInList == 1)			//if valid variable then perform opperation and store in holdValue
                                                        {
                                                        holdValue[arrPos] = (opd1 - opd2);
                                                        holdType[arrPos] = 1;
                                                        }
                                                else					//not a valid variable, so alert user
                                                        {
                                                        printf("This variable is not initiated or is a string, %s\n", tken);
                                                        }
						}
					else				//1 variable and it before plus
						{
						varInList = 0;  //1 is that its in list
						//get variable, check if a valid variable, it valid store in opd1
						strcpy(line, holdInput);
                                                tken = strtok(line, "$");
                                                tken = strtok(NULL, "-");
						for(int i = 0; i < arrSize; i++)
                                                        {
                                                        if(strcmp(holdVarName[i], tken) == 0)
                                                                {
                                                                if(holdType[i] == 1)
                                                                        {
                                                                        varInList++;
                                                                        opd1 = holdValue[i];
                                                                        }
                                                                }
                                                        }
						//get number, use atoi to change to integer and store it in opd2
						strcpy(line, holdInput);
                                                tken = strtok(line, "-");
                                                tken = strtok(NULL, "\n");
						strcpy(temp, tken);
                                                tempNum = atoi(temp);
                                                opd2 = tempNum;
						if(varInList == 1)			//if valid variable then perform opperation and store in holdValue
                                                        {
                                                        holdValue[arrPos] = (opd1 - opd2);
                                                        holdType[arrPos] = 1;
                                                        }
                                                else					//not a valid variable, so alert user
                                                        {
                                                        printf("This variable is not initiated or is a string, %s\n", tken);
                                                        }
						}
					}
				}
//********************************************************************************************************************************************
			else if((strchr(line, '*')) != NULL)
				{			
				isInList = 0;
                                //get variable before equal, check if its in array of variables, if not then add it to array
				arrPos = arrSize;
				strcpy(holdInput, line);
				tken = strtok(line, "=");
                                for(int i = 0; i < arrSize; i++)
                                        {
                                        if(strcmp(holdVarName[i], tken) == 0)
                                                        {
                                                        isInList = 1;
                                                        arrPos = i;
                                                        }
                                        }
                                if(isInList == 0)
                                        {
                                        strcpy(&holdVarName[arrPos][0], tken);
                                        arrSize++;
                                        }
				if(strchr(holdInput, '$') == NULL)			//if no variables then do the following
					{
					strcpy(line, holdInput);			//restore line
					//get first number, use atoi to make it an integer
					tken = strtok(line, "=");
					tken = strtok(NULL, "*");
					strcpy(temp, tken);
                                        tempNum = atoi(temp);
                                        opd1 = tempNum;
					//get second number, use atoi to make it an integer
					tken = strtok(NULL, "\n");
					strcpy(temp, tken);
                                        tempNum = atoi(temp);
                                        opd2 = tempNum;
					//get two numbers
					holdType[arrPos] = 1;				//set value type to 1 since integer
					holdValue[arrPos] = opd1 * opd2;		//add both and put in array of integers 
					}
				else							//if variable then check if befoer plus or after
					{
					beforePlus = 0;					//0 before , 1 is variable after plus
					pch = strchr(holdInput, '$');			//get location of 1 variable
					if(pch > strchr(holdInput, '*'))		//check if afer minus, if so then set beforePlus to 1
						{
						beforePlus++;
						}
					count = 0;					//keep track of variables in line
					while(pch != NULL)
						{
						count++;
						pch = strchr(pch + 1, '$');		//get next variable
						}
					if(count == 2)					//if two variables found then do following
						{
						varInList = 0;	//2 is that both var are in list
						//get first variable, check if it in list and try to get its value
						strcpy(line, holdInput);
						tken = strtok(line, "$");
                                                tken = strtok(NULL, "*");
						strcpy(holdInput2, tken);
						for(int i = 0; i < arrSize; i++)
							{
							if(strcmp(holdVarName[i], tken) == 0)
								{
								if(holdType[i] == 1)
									{
									opd1 = holdValue[i];
									varInList++;
									}
								}
							}
						//get second variable, check if it in list and try to get its value
						strcpy(line, holdInput);
						tken = strtok(line, "$");
						tken = strtok(NULL, "$");
						tken = strtok(NULL, "\n");
						for(int i = 0; i < arrSize; i++)
                                                        {
                                                        if(strcmp(holdVarName[i], tken) == 0)
                                                                {
								if(holdType[i] == 1)
									{
                                                                	varInList++;
                                                                	opd2 = holdValue[i];
									}
                                                                }
                                                        }
						if(varInList == 2)				//if 2 variables then perform opperation and put value in holdValue
							{
							holdValue[arrPos] = (opd1 * opd2);
							holdType[arrPos] = 1;
							}
						else						//if 1 of the two not in array then print out message
							{
							printf("One of these two variables is not initiated or is a string, %s or %s\n", holdInput2, tken);
							}			
						}
					else if(beforePlus == 1)				//1 variable and it after plus
						{
						varInList = 0;  //1 is that its in list
						//get first number, use atoi to change to integer and store it in opd1
						strcpy(line, holdInput);
                                                tken = strtok(line, "=");
                                                tken = strtok(NULL, "*");
						strcpy(temp, tken);
                                        	tempNum = atoi(temp);
						opd1 = tempNum;
						//get variable, check if a valid variable, it valid store in opd2
                                                strcpy(line, holdInput);
						tken = strtok(line, "$");
                                                tken = strtok(NULL, "\n");
						for(int i = 0; i < arrSize; i++)
                                                        {
                                                        if(strcmp(holdVarName[i], tken) == 0)
                                                                {
                                                                if(holdType[i] == 1)
                                                                        {
                                                                        varInList++;
                                                                        opd2 = holdValue[i];
                                                                        }
                                                                }
                                                        }
						if(varInList == 1)				//if valid variable then perform opperation and store in holdValue
                                                        {
                                                        holdValue[arrPos] = (opd1 * opd2);
                                                        holdType[arrPos] = 1;
                                                        }
                                                else						//not a valid variable, so alert user
                                                        {
                                                        printf("This variable is not initiated or is a string, %s\n", tken);
                                                        }
						}
					else							//1 variable and it before plus
						{
						varInList = 0;  //1 is that its in list
						//get variable, check if a valid variable, it valid store in opd1
						strcpy(line, holdInput);
                                                tken = strtok(line, "$");
                                                tken = strtok(NULL, "*");
						for(int i = 0; i < arrSize; i++)
                                                        {
                                                        if(strcmp(holdVarName[i], tken) == 0)
                                                                {
                                                                if(holdType[i] == 1)
                                                                        {
                                                                        varInList++;
                                                                        opd1 = holdValue[i];
                                                                        }
                                                                }
                                                        }
						//get number, use atoi to change to integer and store it in opd2
						strcpy(line, holdInput);
                                                tken = strtok(line, "*");
                                                tken = strtok(NULL, "\n");
						strcpy(temp, tken);
                                                tempNum = atoi(temp);
                                                opd2 = tempNum;
						if(varInList == 1)				//if valid variable then perform opperation and store in holdValue
                                                        {
                                                        holdValue[arrPos] = (opd1 * opd2);
                                                        holdType[arrPos] = 1;
                                                        }
                                                else						//not a valid variable, so alert user
                                                        {
                                                        printf("This variable is not initiated or is a string, %s\n", tken);
                                                        }
						}
					}
//********************************************************************************************************************************************
				}
			else if((strchr(line, '"')) != NULL)			//variable will store a string so do following 
                                {
				//check if variable is in list, if so get its position, if not then add it to holdVarName
				isInList = 0;
                                arrPos = arrSize;
				tken = strtok(line, "=");
				for(int i = 0; i < arrSize; i++)
                                        {
                                        if(strcmp(holdVarName[i], tken) == 0)
                                                        {
                                                        isInList = 1;
                                                        arrPos = i;
                                                        }
                                        }
				if(isInList == 0)
                                        {
                                        strcpy(&holdVarName[arrPos][0], tken);
                                        arrSize++;
                                        }
				tken = strtok(NULL, "\"");			//get rid of second "
				holdType[arrPos] = 2;				//make holdType 2 for a string
				strcpy(&holdString[arrPos][0], tken);		//add string to array
                                }
			else							//if no "" $ + - * detected then it is a variable being assigned a number
				{
				isInList = 0;					//set to 0 if variable in holdVarName then set to 1
				//get variable, check if in holdVarName, if not then add it to holdVarName
				arrPos = arrSize;
				tken = strtok(line, "=");
				for(int i = 0; i < arrSize; i++)
					{
					if(strcmp(holdVarName[i], tken) == 0)
							{
							isInList = 1;
							arrPos = i;
							}
					}
				if(isInList == 0)					//if not in holdVarName , add it
					{
					strcpy(&holdVarName[arrPos][0], tken);
					arrSize++;
					}
				tken = strtok(NULL, "\n");				//get number by itself
				strcpy(temp, tken);
				tempNum = atoi(temp);					//make to integer
				holdType[arrPos] = 1;					//have type be integer
				holdValue[arrPos] = tempNum;				//add to holdValue
				}
		}
	}
	}
