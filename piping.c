#include "major2.h"

void my_pipeline(char cmd[])
{
	// passing whole command line so parsing is needed again
	char command[50];
	char commList[3][50];
	char * p;
	int commpos = 0;
	char* arg[50];
	int pos = 0;

	int status; // wait for the forks at the end
	int i;

	int fd1[2], fd2[2]; // two pipes
	if (pipe(fd1) == -1)//pipe failure
	{
		perror("Pipe");
		exit(1);
	}

	p = strtok(cmd, " \n");

	while (p != NULL)
	{
		if (strcmp(p, "|") == 0) //checks for | in the user arguments
		{
			commpos++;
			if (commpos > 2) //checks if user argument has more than one argument
			{
				printf("command can support UP TO 2 pipes and UP TO 3 commands...\n");
				printf("exiting...\n");
				exit(1);
			}
		}
		else
		{
			strcpy(command, p);
			strncat(commList[commpos], command, 50);
			strncat(commList[commpos], " ", 1);
		}

		p = strtok(NULL, " \n");
	}
	commpos++; // 2 pipes means 3 commands

	if (commpos == 2) // 2 commands
	{
		pos = 0;
		if (fork() == 0) //forks to create a new process to handle
		{
			p = strtok(commList[0], " \n");

			while (p != NULL) 
			{
				arg[pos] = p;
				pos++;
				p = strtok(NULL, " \n");
			}
			arg[pos] = NULL;
        
            // outputs info and closes the pipes
			dup2(fd1[1], fileno(stdout));
			close(fd1[0]);
			close(fd1[1]);
			execvp(arg[0], arg);
			printf("%s: command not found\n", arg[0]);
			exit(1);
		}
		else
		{
			p = strtok(commList[1], " \n");

			while (p != NULL)
			{
				arg[pos] = p;
				pos++;
				p = strtok(NULL, " \n");
			}
			arg[pos] = NULL;

			dup2(fd1[0], fileno(stdin));
			close(fd1[0]);
			close(fd1[1]);
			execvp(arg[0], arg);
			printf("%s: command not found\n", arg[0]);
			exit(1);
		}
		
	}
	else // 3 commands
	{
		if (pipe(fd2) == -1)
	        {
			perror("Pipe");
			exit(1);
		}

		pos = 0;

		if (fork() == 0)
		{
		    // Redirect output of process into pipe
			p = strtok(commList[0], " \n");

			while (p != NULL)
			{
				arg[pos] = p;
				pos++;
				p = strtok(NULL, " \n");
			}
			arg[pos] = NULL;

            
			dup2(fd1[1], fileno(stdout));

			close(fd1[0]);
			close(fd1[1]);
			close(fd2[0]);
			close(fd2[1]);

			execvp(arg[0], arg);
			printf("%s: comand not found\n", arg[0]);
			exit(1);
		}
		else
		{
			if (fork() == 0)
			{
			    // Redirect output of process into pipe
				p = strtok(commList[1], " \n");

				while (p != NULL)
				{
					arg[pos] = p;
					pos++;
					p = strtok(NULL, " \n");
				}
				arg[pos] = NULL;
                
				dup2(fd1[0], fileno(stdin));

				dup2(fd2[1], fileno(stdout));

				close(fd1[0]);
				close(fd1[1]);
				close(fd2[0]);
				close(fd2[1]);
				
				execvp(arg[0], arg);
				printf("%s: command not found\n", arg[0]);
				exit(1);
			}
			else
			{
				if (fork() == 0)
				{
					p = strtok(commList[2], " \n");

					while (p != NULL)
					{
						arg[pos] = p;
						pos++;
						p = strtok(NULL, " \n");
					}
					arg[pos] = NULL;

                    //duplicates pipe
					dup2(fd2[0], fileno(stdin));
                    
                    // Redirect output of process into pipe 
                    close(fd1[0]);
					close(fd1[1]);
					close(fd2[0]);
					close(fd2[1]);

					execvp(arg[0], arg);
					printf("%s: command not found\n", arg[0]);
					exit(1);
				}
			}
		}
        
        //Redirect output of process into pipe
        close(fd1[0]);
		close(fd1[1]);
		close(fd2[0]);
		close(fd2[1]);

		for (i = 0; i < 3; i++) // waiting for the forks to finish in the original parent
		{
			wait(&status);
		}
	}
}
