CC=gcc
CFLAGS= -Wall -g

Major2: mainshell.c myhistory.c mycd.c bashInterpreter.c myPath.c piping.c mysignals.c
	$(CC) -o Major2 $(CFLAGS) mainshell.c myhistory.c mycd.c bashInterpreter.c myPath.c piping.c mysignals.c
clean:
	$(RM) Major2
run:
	./Major2
