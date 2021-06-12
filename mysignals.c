#include "major2.h"

void sig_catch(int iSig) //restore and send the signal
{
	
	signal(iSig, SIG_DFL);
	raise(iSig);
}

void ignoreSig() // ignore all signals
{
	if (signal(1, SIG_IGN) == SIG_ERR)
	{
		perror("signal");
		exit(EXIT_FAILURE);
	}
	if (signal(2, SIG_IGN) == SIG_ERR)
	{
		perror("signal");
		exit(EXIT_FAILURE);
	}
	if (signal(3, SIG_IGN) == SIG_ERR)
	{
		perror("signal");
		exit(EXIT_FAILURE);
	}
	if (signal(4, SIG_IGN) == SIG_ERR)
	{
		perror("signal");
		exit(EXIT_FAILURE);
	}
	if (signal(6, SIG_IGN) == SIG_ERR)
	{
		perror("signal");
		exit(EXIT_FAILURE);
	}
	if (signal(8, SIG_IGN) == SIG_ERR)
	{
		perror("signal");
		exit(EXIT_FAILURE);
	}
	if (signal(11, SIG_IGN) == SIG_ERR)
	{
		perror("signal");
		exit(EXIT_FAILURE);
	}
	if (signal(13, SIG_IGN) == SIG_ERR)
	{
		perror("signal");
		exit(EXIT_FAILURE);
	}
	if (signal(14, SIG_IGN) == SIG_ERR)
	{
		perror("signal");
		exit(EXIT_FAILURE);
	}
	if (signal(15, SIG_IGN) == SIG_ERR)
	{
		perror("signal");
		exit(EXIT_FAILURE);
	}
	if (signal(30, SIG_IGN) == SIG_ERR)
	{
		perror("signal");
		exit(EXIT_FAILURE);
	}
	if (signal(31, SIG_IGN) == SIG_ERR)
	{
		perror("signal");
		exit(EXIT_FAILURE);
	}
	if (signal(20, SIG_IGN) == SIG_ERR)
	{
		perror("signal");
		exit(EXIT_FAILURE);
	}
	if (signal(17, SIG_IGN) == SIG_ERR)
	{
		perror("signal");
		exit(EXIT_FAILURE);
	}
	if (signal(18, SIG_IGN) == SIG_ERR)
	{
		perror("signal");
		exit(EXIT_FAILURE);
	}
	if (signal(21, SIG_IGN) == SIG_ERR)
	{
		perror("signal");
		exit(EXIT_FAILURE);
	}
	if (signal(22, SIG_IGN) == SIG_ERR)
	{
		perror("signal");
		exit(EXIT_FAILURE);
	}

	return;
}

void restoreSig() // restore all signles for the child process
{
	if (signal(1, sig_catch) == SIG_ERR)
	{
		perror("signal");
		exit(EXIT_FAILURE);
	}
	if (signal(2, sig_catch) == SIG_ERR)
	{
		perror("signal");
		exit(EXIT_FAILURE);
	}
	if (signal(3, sig_catch) == SIG_ERR)
	{
		perror("signal");
		exit(EXIT_FAILURE);
	}
	if (signal(4, sig_catch) == SIG_ERR)
	{
		perror("signal");
		exit(EXIT_FAILURE);
	}
	if (signal(6, sig_catch) == SIG_ERR)
	{
		perror("signal");
		exit(EXIT_FAILURE);
	}
	if (signal(8, sig_catch) == SIG_ERR)
	{
		perror("signal");
		exit(EXIT_FAILURE);
	}
	if (signal(11, sig_catch) == SIG_ERR)
	{
		perror("signal");
		exit(EXIT_FAILURE);
	}
	if (signal(13, sig_catch) == SIG_ERR)
	{
		perror("signal");
		exit(EXIT_FAILURE);
	}
	if (signal(14, sig_catch) == SIG_ERR)
	{
		perror("signal");
		exit(EXIT_FAILURE);
	}
	if (signal(15, sig_catch) == SIG_ERR)
	{
		perror("signal");
		exit(EXIT_FAILURE);
	}
	if (signal(30, sig_catch) == SIG_ERR)
	{
		perror("signal");
		exit(EXIT_FAILURE);
	}
	if (signal(31, sig_catch) == SIG_ERR)
	{
		perror("signal");
		exit(EXIT_FAILURE);
	}
	if (signal(20, sig_catch) == SIG_ERR)
	{
		perror("signal");
		exit(EXIT_FAILURE);
	}
	if (signal(17, sig_catch) == SIG_ERR)
	{
		perror("signal");
		exit(EXIT_FAILURE);
	}
	if (signal(18, sig_catch) == SIG_ERR)
	{
		perror("signal");
		exit(EXIT_FAILURE);
	}
	if (signal(21, sig_catch) == SIG_ERR)
	{
		perror("signal");
		exit(EXIT_FAILURE);
	}
	if (signal(22, sig_catch) == SIG_ERR)
	{
		perror("signal");
		exit(EXIT_FAILURE);
	}

	return;
}
