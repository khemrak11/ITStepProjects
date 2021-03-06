#pragma once

#include "Word.h"
#include <tchar.h>
#include <fstream>

class Dictionary
{
private:
	int size = 0;
	const int capacity = 100;

	Word words[100];
	int sortID[100];

	int searchID[100];
	int searchAmountFound = 0;
public:
	Dictionary();

	void AddWord(Word word);
	void RemoveWord(int index);

	void ReadFromFile(const char* keyPath, const char* definitionPath);
	void SaveToFile(const char* keyPath, const char* definitionPath);

	void SetTitle(const TCHAR* c, int index, bool isSortedIndex = false);
	void SetDefinition(const TCHAR* c, int index, bool isSortedIndex = false);

	TCHAR* GetTitle(int index, bool isSorted = true);
	TCHAR* GetDefinition(int index, bool isSorted = true);

	int GetSize();
	int GetCapacity();

	bool IsFull();

	void Sort(bool onlySortLast = false);

	//Returns the amount of similar or greater matches. Call GetSearchID to get the values
	int Search(const TCHAR* c);
	int GetSearchID(int index);
};

