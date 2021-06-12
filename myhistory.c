#include "major2.h"

struct HistoryList createHistory()
{
	struct HistoryList h1;

	for (int i = 0; i < 20; i++) // filling 20 entries with null
	{
		strcpy(h1.histList[i], "\0");
	}
	h1.capacity = 0; // capacity starts at 0 can grow beyond 20

	return h1;
}

struct HistoryList addHistory(char entry[], struct HistoryList h1)
{
	if (h1.capacity < 20) // if capacity hasn't reached 20 then simply add entry to list
	{
		strcpy(h1.histList[h1.capacity], entry); 
	}
	else // capacity is beyond 20 so need to delete the earliest entry
	{
		char temp[20][50] = {"\0"}; // temp char array of 20 null entries

		for (int i = 1; i < 20; i++)
		{
			strcpy(temp[i-1], h1.histList[i]); // temp will copy over histlist shifted by 1
		}
		strcpy(temp[19], entry); // temp last entry is the new entry

		for (int i = 0; i < 20; i++)
		{
			strcpy(h1.histList[i], temp[i]); // copy over the temp to histlist
		}
	}
	h1.capacity++; // increase capacity/ entry list number

	return h1;
}

void History(struct HistoryList h1) // print the history
{
	int start = 0;
	int end = 0;
	int iter = 0; // iterator

	if (h1.capacity < 20) // when capacity is under 20 then only print out that amount, not the nulls
	{
		end = h1.capacity; // ex: only 5 entries will be printed if cap = 5
	}
	else // capacity beyond 20 so...
	{
		end = h1.capacity; // ex: 29 will be the end
		start = h1.capacity - 20; // start is now at entry 10
	}

	printf("Command History\n");

	for (int i = start; i < end; i++) // will either print 20 entries or up to capacity
	{
		printf("[%d] %s", i+1, h1.histList[iter]);
		iter++;
	}
}
