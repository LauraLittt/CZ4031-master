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
//	RecordHeader *record_header_address;
//	KeyHeader *next_index_block_address;
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
//	const char * what() const throw ()
//	{
//		return "Block is full";
//	}
//};
//
//class Block {
//public:
//	Block(void* start_address, void* end_address, bool _is_data_block) {
//		block_start_address = start_address;
//		block_end_address = end_address;
//		is_data_block = _is_data_block;
//		record_headers_end_address = (char*)start_address + sizeof(block_start_address) + sizeof(block_end_address) + sizeof(is_data_block);
//	};
//protected:
//	bool is_data_block;
//	void* block_start_address;
//	void* record_headers_end_address;
//	void* block_end_address;
//};
//
//class IndexBlock : Block {
//	IndexBlock(void* start_address, void* end_address) : Block(start_address, end_address, false) {
//		is_leaf = true; // by default
//	};
//
//	std::vector<KeyHeader> get_block_headers() { // in-memory operation
//		void *key_headers_start_address = &block_start_address + sizeof(block_start_address) + sizeof(block_end_address) + sizeof(is_data_block);
//		ptrdiff_t size = ((KeyHeader*)record_headers_end_address - (KeyHeader*)key_headers_start_address) / sizeof(KeyHeader);
//		std::vector<KeyHeader> headers = std::vector<KeyHeader>();
//		for (int i = 0; i < size; i++) {
//			headers.push_back(*((KeyHeader*)key_headers_start_address + i * sizeof(KeyHeader)));
//		}
//		return headers;
//	};
//
//	// TODO: think about how to query the tree
//	//Record get_key() {
//	//};
//
//	// TODO: this is where the actual tree insertion logic comes in
//	//void put_data(Record new_record) {
//	//};
//
//private:
//	bool is_leaf;
//	void* last_rightmost_pointer;
//};
//
//class DataBlock : Block {
//public:
//	DataBlock(void* start_address, void* end_address) : Block(start_address, end_address, true) {
//	};
//
//	void print_block_records() {
//		std::vector<RecordHeader> current_block_headers = get_block_headers();
//		for (int i = 0; i < current_block_headers.size(); i++) {
//			Record current_record = *current_block_headers[i].record_address;
//			std::cout << "tconst: " << current_record.tconst << " average rating: " << current_record.averageRating << " number of votes: " << current_record.numVotes << std::endl;
//		}
//	}
//
//	Record get_data(RecordHeader* requested_data_header_address) {
//		if ((void*) requested_data_header_address < block_start_address) {
//			throw exception("Requested data is before block's start address");
//		}
//		if ((void*) requested_data_header_address > record_headers_end_address) {
//			throw exception("Requested data exceeds record headers' end address");
//		}
//		RecordHeader data_header = *requested_data_header_address;
//		return *(data_header.record_address);
//	};
//
//	void put_data(Record new_record) {
//		std::vector<RecordHeader> current_block_headers = get_block_headers();
//
//		for (int i = 0; i < current_block_headers.size(); i++) {
//			if (current_block_headers[i].record_is_tombstoned) {
//				*(current_block_headers[i].record_address) = new_record;
//				return;
//			}
//		}
//		
//		if (current_block_headers.size() < MAX_RECORDS_IN_DATA_BLOCK) {
//			Record *last_address;
//			if (current_block_headers.size() == 0) { // calling back() on empty vector causes exception
//				last_address = (Record*) block_end_address;
//			}
//			else {
//				last_address = current_block_headers.back().record_address;
//			}
//			Record *new_record_address = last_address - 1;
//			*new_record_address = new_record;
//			RecordHeader new_record_header = RecordHeader{
//				new_record_address,
//				false
//			};
//
//			*((RecordHeader*) record_headers_end_address) = new_record_header;
//			record_headers_end_address = (RecordHeader*) record_headers_end_address + 1;
//		}
//		else {
//			throw BlockIsFullException();
//		}
//	};
//private:
//	std::vector<RecordHeader> get_block_headers() { // in-memory operation
//		void *record_headers_start_address = (char*)block_start_address + sizeof(block_start_address) + sizeof(block_end_address) + sizeof(is_data_block);
//		int size = (reinterpret_cast<uintptr_t>(record_headers_end_address) - reinterpret_cast<uintptr_t>(record_headers_start_address)) / sizeof(RecordHeader);
//		std::vector<RecordHeader> headers = std::vector<RecordHeader>();
//		for (int i = 0; i < size; i++) {
//			headers.push_back(*((RecordHeader*)record_headers_start_address + i));
//		}
//		return headers;
//	};
//};
//
//class DB {
//public:
//	void createBlocks() {
//		if (data_blocks_start_address != NULL) { // prevent re-creation of blocks
//			return;
//		}
//		data_blocks_start_address = calloc(NUM_DATA_BLOCKS, BLOCK_SIZE);
//		data_blocks_current_ptr = 0;
//		for (int i = 0; i < NUM_DATA_BLOCKS; i++) { // need to do this to get memory addresses
//			DataBlock b = DataBlock(((char*)data_blocks_start_address + i * BLOCK_SIZE), (char*)data_blocks_start_address + (i + 1) * BLOCK_SIZE);
//			disk_blocks.push_back(b);
//		}
//		index_blocks_start_address = calloc(NUM_INDEX_BLOCKS, BLOCK_SIZE);
//	}
//	static DB* getDBInstance();
//
//	void get_data(float rating) {
//		// TODO:
//		// 1. query B+Tree for the address to the record header in the data block
//		// 2. get the block's logical address
//		// 3. access the record header in the block
//		// 4. get the data using the pointer in the record header
//	}
//
//	void print_db_records() {
//		for (int i = 0; i < disk_blocks.size(); i++) {
//			DataBlock current_data_block = disk_blocks[i];
//			current_data_block.print_block_records();
//		}
//	}
//
//	int get_block_logical_address(void *record_header_address) {
//		return ((unsigned long) record_header_address - (unsigned long) data_blocks_start_address) / BLOCK_SIZE;
//	}
//
//	void put_data(Record r) {
//		// get the current data block first
//		// https://stackoverflow.com/questions/42081389/objects-in-stdvector-not-saving-attributes-correctly
//		// that's why need to put ampersand after DataBlock at start (get as reference so you can mutate it)
//		DataBlock& current_data_block = disk_blocks[data_blocks_current_ptr];
//
//		try {
//			current_data_block.put_data(r);
//		}
//		catch (BlockIsFullException& ex) {
//			data_blocks_current_ptr++;
//			put_data(r);
//			return;
//		}
//
//		// TODO: update the B+Tree after this
//	}
//protected:
//	static DB* dbInstance;
//
//private:
//	DB() {}
//	void* data_blocks_start_address = NULL;
//	int data_blocks_current_ptr = NULL;
//	void* index_blocks_start_address = NULL;
//	std::vector<DataBlock> disk_blocks;
//};
//
//DB* DB::dbInstance = nullptr;
//DB* DB::getDBInstance() {
//	if (dbInstance == NULL) {
//		dbInstance = new DB();
//	}
//	return dbInstance;
//}
//
//void test_storage()
//{
//	DB* db = DB::getDBInstance();
//	db->createBlocks();
//	try {
//		Record fake_record = {
//			"tt0000001",
//			5.6,
//			1645
//		};
//		std::cout << "Added record one" << std::endl;
//		db->put_data(fake_record);
//		Record fake_record2 = {
//			"tt0000002",
//			5.6,
//			1645
//		};
//		db->put_data(fake_record2);
//		std::cout << "Added record two" << std::endl;
//		Record fake_record3 = {
//			"tt0000003",
//			5.6,
//			1645
//		};
//		db->put_data(fake_record3);
//		std::cout << "Added record three" << std::endl;
//		Record fake_record4 = {
//			"tt0000004",
//			5.6,
//			1645
//		};
//		db->put_data(fake_record4);
//		std::cout << "Added record four" << std::endl;
//		db->print_db_records();
//	}
//	catch (exception& ex) {
//		std::cout << ex.what() << std::endl;
//	}
//
//	//void *start_addr = malloc(100);
//	//void *end_addr = &start_addr + 100;
//	//std::cout << "see start_addr: " << &start_addr << std::endl;
//	//std::cout << "see end_addr: " << &end_addr << std::endl;
//	//Block b = Block(start_addr, end_addr, true);
//
//	//char* fake_address = (char*) malloc(1);
//	//*fake_address = 'c';
//	//std::cout << "See fake address: " << *fake_address << endl;
//	//std::cout << "See fake address+1: " << *(fake_address+1) << endl;
//
//	//int *p1 = (int*) malloc(10 * sizeof *p1);
//	//printf("default-aligned addr:   %p\n", (void*)p1);
//	//free(p1);
//
//	//int *p2 = (int*) _aligned_malloc(1024, 1024 * sizeof *p2);
//	//printf("1024-byte aligned addr: %p\n", (void*)p2);
//	//_aligned_free(p2);
//}
//
//// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
//// Debug program: F5 or Debug > Start Debugging menu
//
//// Tips for Getting Started: 
////   1. Use the Solution Explorer window to add/manage files
////   2. Use the Team Explorer window to connect to source control
////   3. Use the Output window to see build output and other messages
////   4. Use the Error List window to view errors
////   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
////   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
