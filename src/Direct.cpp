#include "Direct.h"
#include <iostream>
using namespace std;

Direct::Direct()
{
	capacity = lrubits = blocksize = hits = misses = associativity = 0;
	sets = NULL;
}

Direct::Direct(int c, int b, int a)
{
	capacity = c;
	blocksize = b;
	associativity = a;
	lrubits = log(a) / log(2);
	//blocks = new Block[(c * 1024) / b];
	sets = new Set[((c * 1024) / b) / a];
	for (int i = 0; i < ((c * 1024) / b) / a; i++){
		sets[i].init(a,b);
	}
	//for (int i = 0; i < (c * 1024) / b; i++)
	//{
	//	blocks[i].set(b);
	//}

}

unsigned int Direct::read(bool dori, unsigned int address, unsigned int* memory)
{
	int offsetBits = log(blocksize) / log(2);
	int indexBits = log(((capacity * 1024) / blocksize)/associativity) / log(2);
	int tagBits = 24 - indexBits - offsetBits;
	int offset = extract(address, 0, offsetBits);
	int index = extract(address, offsetBits, indexBits+offsetBits);
	int tag = extract(address, indexBits + offsetBits, 24);
	for(int i=0;i<associativity;i++)
		if (!sets[index].blocks[i].val)
		{
			misses++;
			sets[index].blocks[i].val = true;
			sets[index].blocks[i].tag = tag;
			int blockaddress = (address % blocksize) ;
			int count = 0;
			while (blockaddress > 0) {
				count++;
				blockaddress--;
			}
			unsigned int baseaddress = address - count;
			for (int j = 0; j < blocksize; j++) {
				sets[index].blocks[i].data[j] = memory[baseaddress];
				baseaddress++;
			}
			return sets[index].blocks[i].data[offset];
		}
		else if (sets[index].blocks[i].tag != tag)
		{
			misses++;
			int blockaddress = address%blocksize;
			int count = 0;
			while (blockaddress > 0) {
				count++;
				blockaddress--;
			}
			unsigned int baseaddress = address - count;

			if (sets[index].blocks[i].dirty)
				for (int j = 0; j < blocksize; j++) {
					memory[baseaddress] = sets[index].blocks[i].data[j];
					baseaddress++;
				}
			sets[index].blocks[i].tag = tag;
			baseaddress = address - count;
			for (int j = 0; j < blocksize; j++) {
				sets[index].blocks[i].data[j] = memory[baseaddress];
				baseaddress++;
			}
			return sets[index].blocks[i].data[offset];
		}
		else
		{
			hits++;
			return sets[index].blocks[i].data[offset];

		}
}

void Direct::write(unsigned int address, unsigned int data, unsigned int *memory,char hpolicy,char mpolicy)
{
	int offsetBits = log(blocksize) / log(2);
	int indexBits = log(((capacity * 1024) / blocksize) / associativity) / log(2);
	int tagBits = 24 - indexBits - offsetBits;
	int offset = extract(address, 0, offsetBits);
	int index = extract(address, offsetBits, indexBits + offsetBits);
	int tag = extract(address, indexBits + offsetBits, 24);
	for(int j=0;j<associativity;j++)
		if (!sets[index].blocks[j].val)
		{
			misses++;
			if (mpolicy == 'a')
			{
				sets[index].blocks[j].val = true;
				sets[index].blocks[j].tag = tag;
				sets[index].blocks[j].data[offset] = data;
				if (hpolicy == 't')
					memory[address] = data;
				else if (hpolicy == 'b')
				{
					sets[index].blocks[j].dirty = true;
					//memory[address] = data;
				}
		
			}
			else if(mpolicy=='n')
			{
				memory[address] = data;
			}
		}
		else if (sets[index].blocks[j].tag != tag)
		{
			misses++;

			if (mpolicy == 'a')
			{
				sets[index].blocks[j].tag = tag;
				sets[index].blocks[j].data[offset] = data;
				if (hpolicy == 't')
					memory[address] = data;
				else if (hpolicy == 'b')
				{
					sets[index].blocks[j].dirty = true;
					//memory[address] = data;
				}
			}
			else if (mpolicy == 'n')
			{
				memory[address] = data;
			}
		}
		else
		{
			hits++;
			if (hpolicy == 'b')
			{
				if (!sets[index].blocks[j].dirty)
				{
					sets[index].blocks[j].dirty = true;
					sets[index].blocks[j].data[offset] = data;
				}
				else
				{
					int blockaddress = address%blocksize;
					int count = 0;
					while (blockaddress > 0) {
						count++;
						blockaddress--;
					}
					unsigned int baseaddress = address - count;

					if (sets[index].blocks[j].dirty)
						for (int i = 0; i < blocksize; i++) {
							memory[baseaddress] = sets[index].blocks[j].data[i];
							baseaddress++;
						}
					sets[index].blocks[j].data[offset] = data;
				}
			}
			else if(hpolicy=='t')
			{
				sets[index].blocks[j].data[offset] = data;
				memory[address] = data;
			}
		}
}


Direct::~Direct()
{
	delete[]sets;
}
