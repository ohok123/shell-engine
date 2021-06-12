#include "major2.h"

int CDcmd(char **args)
{
    	char s[100]; 

	if(args[1] == NULL) //If statement to send you back to /home if one argument is given
	{
		// printing current working directory 
		printf("%s\n", getcwd(s, 100)); 

		// using the command 
		chdir(".."); 
  
		// printing current working directory 
		printf("%s\n", getcwd(s, 100)); 
		
		// using the command 
		chdir(".."); 
  
		// printing current working directory 
		printf("%s\n", getcwd(s, 100)); 
    	}

	else if( chdir( args[1] ) == 0 ) //if statment for if multiple arguments are given
	{
		printf( "Directory changed to %s\n", args[1] );
		return EXIT_SUCCESS;
	}

	return 0;

}