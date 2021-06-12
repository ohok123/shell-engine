#include "major2.h"

int main(int argc, char *argv[])
{
	FILE *fp;
	int batchmode = 0; // determine if batch mode will be executed, stay 0 if not

	ignoreSig(); // start the shell with all signals ignored

	
	int cont = 0; // continues the while loop until user inputs 'quit'
	int pos = 0; // position for the char array
	char comm[50]; // command, holds the command the user inputs, any length
	char* arg[50]; // char array that holds up to 51 arguments, should be sufficent for this program 
	char line[150]; // holds the whole user input as a character string, 50 should be sufficent

	char * p; // character pointer for string token 
	char commList[50][50]; // array of 20 commands of size 50
	char command[50]; // holds a single command
	int commpos = 0; // position in the command list
	int leave = 0; // leave the loop

    char *redirarg[50]; // character array for redirection commands

	struct HistoryList HISTORYLIST; // list for the history
	char originPath[900];//stores the PATH for when it has to be restored
	setPath(originPath);//call fucntion to store PATH in originHold
	char currentPath[900];//current path
	strcpy(currentPath, originPath);//get a copy from originPath
	char histEntry[50]; // history entry
	char wholeCommand[50]; // holds the whole command as it was typed

	HISTORYLIST = createHistory(); // initiate history list

    int pipeline = 0; // initiate pipeline

	pid_t pid; // for forking

	if (argc == 2) // attempting to open the batch file
	{
		fp = fopen(argv[1], "r");
		if (fp == NULL) // file failed to open
		{
			printf("File opening failed. \n");
			exit(EXIT_FAILURE);
		}
		batchmode = 1;
	}

	//batch mode starts if 2 args were passed

	if(batchmode == 1)
	{
		while (fgets(line, 150, fp)) // read batch file line by line
		{
			for (int i = 0; i < 50; i++)
			{
				strcpy(commList[i], "\0"); // set/reset command list 
			}

			pos = 0; // set/reset position
			commpos = 0; // set/reset command position for list

			strcpy(histEntry, line); // copy the whole command as a history entry
			HISTORYLIST = addHistory(histEntry, HISTORYLIST); // add the entry into history list

			p = strtok(line, ";\n"); // begin using string token, delim at space and newline

			while (p != NULL) // parsing the line looking for ;
			{
				if (strcmp(p, " ") == 0 || strcmp(p, "  ") == 0)
				{
					// space means no command entered
				}
				else // command found
				{
					strcpy(command, p);
					if (commpos > 0)
					{
						for (int i = 0; i < strlen(command); i++)
						{
					 		p[i] = p[i+1];
						}
					}
					strcpy(commList[commpos], p);
					commpos++; // keep track of number of commands
				}
					
				p = strtok(NULL, ";\n");
			}
			for (int i = 0; i < commpos; i++)
			{
		    	// starting from here are redirection code
		        char redircommand[50]; // array to hold redirection command to prevent strtok changing original string
		        strcpy(redircommand, commList[i]); // make a copy of original command
		        char* argum = strtok(redircommand," "); 
		        int n = 0; // counter
			char *txt = NULL; // txt for text file name
			while (argum != NULL)
			{
				if(*argum == '<') // once find < , dup stdin file descriptor
				{
					txt = strtok(NULL," ");
					int in = open(txt, O_RDONLY);
					dup2(in,0);
					close(in);
				}
				else if(*argum == '>') // once find > , dup stdout file descriptor
				{
					txt = strtok(NULL, " ");
					int out = open(txt, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
					dup2(out,1);
					close(out);
				}
				else
				{
					redirarg[n] = argum; // if doesn't encounter < or > , keep parsing the actual execvp command and store inside array used for execvp
					n++; // update counter
				}
				argum = strtok(NULL," "); // keep parsing
			}
			redirarg[n] = NULL; // add null to the end because of execvp
			// end of redirection code
		
			strcpy(wholeCommand, commList[i]); // copy the whole command as a

			int redirection = 0;
			pos = 0;
			p = strtok(commList[i], " \n"); // parsing the command list item i

			strcpy(comm, p); // the first position is the command
			if (strcmp(comm, "exit") == 0) // check if user inputted quit
			{
				leave = 1; // will be used at the end, this way quit will happen after exec
			}
			else
			{
				pipeline = 0;
				while (p != NULL) // continue until the whole string traversed
				{
					 arg[pos] = p; // store p into the char array

					 if (strcmp(arg[pos], "|") == 0)
					 {
						 		pipeline = 1;
					 }
					 else if (strcmp(arg[pos], "<") == 0 || strcmp(arg[pos], ">") == 0)
					 {
						 		redirection = 1;   
					 }
					 pos++; // move to the next spot in array
					 p = strtok(NULL, " \n"); // next string token
				}



				arg[pos] = NULL; // assign null to the end of the char array for calling execvp

				pid = fork();

				if (pid == 0) // child proccess
				{
					restoreSig(); // restore signals for the child process
					if (pipeline == 1) // pipeline command
					{
						my_pipeline(wholeCommand);
						exit(1);
					}
					else if (redirection == 1) // redirection command
					{
						execvp(redirarg[0], redirarg);
						exit(1);
					}
					else if (strcmp(comm, "myhistory") == 0) // print myhistory function
					{
						History(HISTORYLIST);
						exit(1);
					}
					else if(strcmp(comm, "cd") == 0) // cd command
					{
						CDcmd(arg);
						exit(1);
					}
					else if(strcmp(comm, "bash2") == 0) // bash2 interpretor
					{
						bashInterpreter(arg[1]);
						exit(1);
					}
					else if(strcmp(comm, "path") == 0) // path function
					{
						exit(1);
					}
					else // if it a normal command here
					{
						execvp(comm, arg);
						printf("%s: command not found\n", comm); // user input is not valid
						exit(1);
					}
				}
				else if (pid > 0) // parent
				{
					wait( (int *)0 ); // waiting for the child to finish

					if((strcmp(comm, "path") == 0))//if path command entered do the following		
					{
						pathCommand2(currentPath, wholeCommand);	
					}
					//restore the file descriptor back to the parent process by accessing /dev/tty
					int in = open("/dev/tty", O_RDONLY);
					dup2(in,0);
					close(in);
					
					int out = open("/dev/tty", O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
					dup2(out,1);
					close(out);
					
				}
				else // fork error
				{
					perror("fork error");
					exit(2);
				}
			}
		}
		if (leave == 1)
		{
			restorPath(originPath); // return path to what it was at the start of the program
			return 0;
		}
		}
		
		fclose(fp);
		return 0;
	}

	//interpretor mode

	while (cont == 0)
	{
		for (int i = 0; i < 50; i++)
		{
			strcpy(commList[i], "\0"); // set/reset command list 
		}

		pos = 0; // set/reset position
		commpos = 0; // set/reset command position for list
		printf("Major 2 Shell > ");
		fgets(line, 150, stdin); // get the whole line of user input and store into 'line'

		strcpy(histEntry, line); // copy the whole command as a history entry

		HISTORYLIST = addHistory(histEntry, HISTORYLIST); // add the entry into history list
		

		p = strtok(line, ";\n"); // begin using string token, delim at space and newline

		while (p != NULL) // parsing the line looking for ;
		{
			if (strcmp(p, " ") == 0 || strcmp(p, "  ") == 0)
			{
				// space means no command entered
			}
			else // command found
			{
				strcpy(command, p);
				if (commpos > 0)
				{
					for (int i = 0; i < strlen(command); i++)
					{
						p[i] = p[i+1];
					}
				}
				strcpy(commList[commpos], p);
				commpos++; // keep track of number of commands
			}

			p = strtok(NULL, ";\n");
		}

		for (int i = 0; i < commpos; i++)
		{
		    // starting from here are redirection code
		    char redircommand[50]; // array to hold redirection command to prevent strtok changing original string
		    strcpy(redircommand, commList[i]); // make a copy of original command
		    char* argum = strtok(redircommand," "); 
            int n = 0; // counter
            char *txt = NULL; // txt for text file name
            while (argum != NULL)
            {
                if(*argum == '<') // once find < , dup stdin file descriptor
                {
                    txt = strtok(NULL," ");
                    int in = open(txt, O_RDONLY);
                    dup2(in,0);
                    close(in);
                }
                else if(*argum == '>') // once find > , dup stdout file descriptor
                {
                    txt = strtok(NULL, " ");
                    int out = open(txt, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
                    dup2(out,1);
                    close(out);
                    }
                else
                {
                    redirarg[n] = argum; // if doesn't encounter < or > , keep parsing the actual execvp command and store inside array used for execvp
                    n++; // update counter
                }
            argum = strtok(NULL," "); // keep parsing
            }
            redirarg[n] = NULL; // add null to the end because of execvp
            // end of redirection code
            
            
		   	strcpy(wholeCommand, commList[i]); // copy the whole command as a

            int redirection = 0;
			pos = 0;
			p = strtok(commList[i], " \n"); // parsing the command list item i

			strcpy(comm, p); // the first position is the command
			if (strcmp(comm, "exit") == 0) // check if user inputted quit
			{
				leave = 1; // will be used at the end, this way quit will happen after exec
			}
			else
			{
			    	pipeline = 0;
				while (p != NULL) // continue until the whole string traversed
				{
					arg[pos] = p; // store p into the char array
					
					if (strcmp(arg[pos], "|") == 0)
					{
					        pipeline = 1;
					}
					else if (strcmp(arg[pos], "<") == 0 || strcmp(arg[pos], ">") == 0)
					{
					        redirection = 1;   
					}
					pos++; // move to the next spot in array
					p = strtok(NULL, " \n"); // next string token
				}

				

				arg[pos] = NULL; // assign null to the end of the char array for calling execvp

				pid = fork();
			
				if (pid == 0) // child proccess
				{
					restoreSig();
					if (pipeline == 1)
					{
				         	my_pipeline(wholeCommand);
				        	exit(1);
					}
					else if (redirection == 1)
					{
					    execvp(redirarg[0], redirarg);
					    exit(1);
					}
					else if (strcmp(comm, "myhistory") == 0)
					{
						History(HISTORYLIST);
						exit(1);
					}
					else if(strcmp(comm, "cd") == 0)
		    			{
			                        CDcmd(arg);
                       				exit(1);
			    		}
					else if(strcmp(comm, "bash2") == 0)
                                        {
                                        bashInterpreter(arg[1]);
					exit(1);
                                        }
					else if(strcmp(comm, "path") == 0)
                                        {
                                      	exit(1);
                                        }
					else
					{
						execvp(comm, arg);
						printf("%s: command not found\n", comm); // user input is not valid
						exit(1);
					}
				}
				else if (pid > 0) // parent
				{
					wait( (int *)0 ); // waiting for the child to finish
					
					if((strcmp(comm, "path") == 0))//if path command entered do the following		
					{
						pathCommand2(currentPath, wholeCommand);	
					}
				    //restore the file descriptor back to the parent process by accessing /dev/tty
				    int in = open("/dev/tty", O_RDONLY);
				    dup2(in,0);
                    close(in);

                    int out = open("/dev/tty", O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
                    dup2(out,1);
                    close(out);

				}
				else // fork error
				{
					perror("fork error");
					exit(2);
				}
				
			}
		}
		if (leave == 1) // leave the loop
		{
			cont++; // this will leave the while loop
			restorPath(originPath); // restore path to the original state
		}
	}
	return 0;
}
