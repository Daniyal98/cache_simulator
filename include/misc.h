#pragma once
#include <iostream>
using namespace std;

struct Block
{
	bool val;
	unsigned int tag;
	bool dirty;
	unsigned int* data;
	unsigned int lru;
	Block() {
		val = false;
		tag = 0;
		dirty = false;
		data = NULL;
		lru = 0;
	}
	void set(int x)
	{
		data = new unsigned int[x];
	}
};
struct Set
{
	Block* blocks;
	int maxLRU;
	Set()
	{
		blocks = NULL;
		maxLRU = 0;

	}
	void init(int x,int y)
	{
		blocks = new Block[x];
		for (int i = 0; i < x; i++) {
			blocks[i].set(y);
		}
	}
	~Set() {
		delete[]blocks;
	}
};
