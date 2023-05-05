#pragma once
#include "misc.h"


class Direct
{

public:
	int capacity;
	int blocksize;
	int associativity;
	//Block* blocks;
	Set * sets;
	int hits, misses;
	int lrubits;

	unsigned int extract(unsigned int value, int begin, int end)
	{
		unsigned int mask = (1 << (end - begin)) - 1;
		return (value >> begin) & mask;
	}


	Direct();
	Direct(int, int, int);
	unsigned int read(bool, unsigned int,unsigned int*);
	void write(unsigned int, unsigned int, unsigned int*,char hpolicy='b',char mpolicy='a');

	~Direct();
};

