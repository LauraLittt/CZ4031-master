//#pragma once
//
//#pragma once
//// DatabasePrinciples.cpp : This file contains the 'main' function. Program execution begins and ends there.
//// 32-bit pointers assumed
//// TODO: refactor and move some classes to their own files
//
//#include <iostream>
//#include <vector>
//#include "Storage.h"
//
//#define BLOCK_SIZE 100
//#define NUM_DATA_BLOCKS 2 // fun experiment to try: try decreasing this number to 1
//#define NUM_INDEX_BLOCKS 1
//
//const int MAX_RECORDS_IN_DATA_BLOCK = (BLOCK_SIZE - 13 /* number of bytes in data block header */) / (5 /* number of bytes per record header */ + 18 /* number of bytes per record */);
//
////void *calloc(size_t nitems, size_t size)
//
//using namespace std;
//
//struct Record
//{
//	char tconst[10];
//	float averageRating;
//	int numVotes;
//};
//
//struct RecordHeader
//{
//	Record* record_address;
//	bool record_is_tombstoned;
//};
//
//struct KeyHeader
//{
//	void* key_address;
//	bool key_is_tombstoned;
//};
//
//typedef union left_ptr
//{
//	RecordHeader* record_header_address;
//	KeyHeader* next_index_block_address;
//};
//
//struct Key
//{
//	float averageRating;
//	left_ptr left_ptr;
//};
//
//struct BlockIsFullException : public std::exception
//{
//	const char* what() const throw ()
//	{
//		return "Block is full";
//	}
//};
