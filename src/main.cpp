#pragma once
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include<fstream>
#include "funcs.h"
#include "misc.h"
#include "Direct.h"
using namespace std;


#define CACHE_READ 0
#define CACHE_WRITE 1
#define CACHE_IREAD 2


int main(int argc, char *argv[])
{
	//the values that we get from the 
	//command line will be put into these
	//variables. we can use them later in the program
	//like for checking if they have valid values

	int cache_capacity = 0;
	int cache_blocksize = 0;
	int cache_associativity = 0;
	bool split=false;
	char hwrite='b';
	char mwrite = 'a';
	if (!parseParams(argc, argv, cache_capacity, cache_blocksize, cache_associativity,split,hwrite,mwrite)) {
		exit(2);
	}
	if (cache_capacity != 4 && cache_capacity != 8 && cache_capacity != 16 && cache_capacity != 32 && cache_capacity != 64) {
		cout << "Invalid Cache Capacity, terminating. " << endl;
		system("pause");
		exit(0);
	}
	if (cache_blocksize != 4 && cache_blocksize != 8 && cache_blocksize != 16 && cache_blocksize != 32 && cache_blocksize != 64
		&& cache_blocksize != 128 && cache_blocksize != 256 && cache_blocksize != 512) {
		cout << "Invalid Cache blocksize, terminating. " << endl;
		system("pause");
		exit(0);
	}
	if (cache_associativity != 1 && cache_associativity != 2 && cache_associativity != 4 && cache_associativity != 8 && cache_associativity != 16) {
		cout << "Invalid Cache Associativity, terminating. " << endl;
		system("pause");
		exit(0);
	}
	if ((split==false && cache_associativity>((cache_capacity*1024)/cache_blocksize)) || (split == true && cache_associativity>((cache_capacity * 512) / cache_blocksize)))
	{
		cout << "Invalid Cache Associativity, terminating. " << endl;
		system("pause");
		exit(0);
	}
	cout << "Cache Capacity: " << cache_capacity << endl;
	cout << "Cache BlockSize: " << cache_blocksize << endl;
	cout << "Cache Associativity: " << cache_associativity << endl;

	int i;
	int read_write;
	int address;
	unsigned int data;

	unsigned int* memory = new unsigned int[1024*1024*16];

	////initialise memory
	for (i = 0; i< (1024*1024*16); i++)
	{
		memory[i] = i;
		//cout << hex<<memory[i] << endl;
	}
	//Cache creation done here

	if (split)//split
	{
		cache_capacity = cache_capacity / 2;
		Direct instCache(cache_capacity, cache_blocksize, cache_associativity);
		Direct cache(cache_capacity, cache_blocksize, cache_associativity);
		fstream file("sample.trace", ios::in);
		//// repeat till we reach the end of the input	
		while (!(file.eof()))
		{
			//read in whether to read or write to the cache
			file >> dec >> read_write;

			// check again if we have reached the end
			// as this flag is set only after a 'cin'
			if ((file.eof())) return 1;

			file >> hex >> address;

			//if it is a cache write the we have to read the data
			if (read_write == CACHE_WRITE)
			{
				file >> hex >> data;
				//memory[address] = data;
				cache.write(address, data, memory, hwrite, mwrite);
				//output the new contents
				//cout << hex << "memory[" << address << "] = " << dec << data << endl;
				cout << hex << "Write memory[" << address << "] = " << memory[address] << endl;
			}
			else if(read_write==CACHE_READ)
			{
				//cache.read(read_write, address, memory);
				//output the contents, check for hits or misses on read from memory
				//cout << hex << "memory[" << address << "] = " << dec << memory[address] << endl;
				cout << hex << "Read data memory[" << address << "] = " << cache.read(read_write, address, memory) << endl;
			}
			else if (read_write == CACHE_IREAD)
				cout << hex << "Read Instruction memory[" << address << "] = " << instCache.read(read_write, address, memory) << endl;

		}
		cout <<dec<< endl<<"Data Hits = " << cache.hits << endl << "Data Misses = " << dec << cache.misses << endl;
		cout << "Instruction Hits = " << instCache.hits << endl << "Instruction Misses = " << instCache.misses << endl;

		cout <<dec<<endl<< "Data Hit Rate = " << (cache.hits / (float(cache.hits) + cache.misses)) * 100 << "%" << endl <<
			"Data Misses Rate = " << (cache.misses / (float(cache.hits) + cache.misses)) * 100 << "%" << endl;
		cout << "Instruction Hit Rate = " << (instCache.hits / (float(instCache.hits) + instCache.misses)) * 100 << "%" << endl <<
			"Instruction Misses Rate = " << (instCache.misses / (float(instCache.hits) + instCache.misses)) * 100 << "%" << endl;
		system("pause");
		exit(0);

	}
	else
	{
		Direct cache(cache_capacity, cache_blocksize, cache_associativity);
		fstream file("sample.trace", ios::in);
		//// repeat till we reach the end of the input	
		while (!(file.eof()))
		{
			//read in whether to read or write to the cache
			file >> dec >> read_write;

			// check again if we have reached the end
			// as this flag is set only after a 'cin'
			if ((file.eof())) return 1;

			file >> hex >> address;

			//if it is a cache write the we have to read the data
			if (read_write == CACHE_WRITE)
			{
				file >> hex >> data;
				//memory[address] = data;
				cache.write(address, data, memory,hwrite,mwrite);
				//output the new contents
				//cout << hex << "memory[" << address << "] = " << dec << data << endl;
				cout << hex << "Write memory[" << address << "] = " << memory[address] << endl;
			}
			else
			{
				//cache.read(read_write, address, memory);
				//output the contents, check for hits or misses on read from memory
				//cout << hex << "memory[" << address << "] = " << memory[address] << endl;
				cout << hex << "memory[" << address << "] = "  << cache.read(read_write, address, memory) << endl;
			}

		}
		cout <<dec<< "Hits = " << cache.hits << endl << "Misses = " << cache.misses << endl;
		cout << "Hit Rate = " << (cache.hits / (float(cache.hits) + cache.misses)) * 100 << "%" << endl <<
			"Misses Rate = " << (cache.misses / (float(cache.hits) + cache.misses)) * 100 << "%" << endl;
		system("pause");
		exit(0);

	}
	



}

