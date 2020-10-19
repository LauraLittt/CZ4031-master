#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <Windows.h>
#include <strsafe.h>
#include <iostream>
#include <map> 

#include <sstream>
#include <iterator>
#include <cstdio>
#include <stdio.h>
#include <io.h>
#include <direct.h>

#define SELF_REMOVE_STRING  TEXT("cmd.exe /C ping 1.1.1.1 -n 1 -w 3000 > Nul & Del /f /q \"%s\"")
#define _CRT_SECURE_NO_DEPRECATE  //for VS 2019

#include <filesystem>
#include "B+ Tree.h"


using namespace std;



void initialInsertion(BPTree** bPTree) {
    //if (rmdir("C:\Users\achua021\Downloads\CZ4031-Laura_edits\DBFiles") != 0)
    //{
    //    cout << "Old DBFiles deleted" << endl;
    //}




    //return;

    string tconst;
    string str_avgR;
    string string_numVotes;
    float avgRating;
    int numVotes;
    int i = 0;
    int rollNo;
    string str;
    ifstream inFile;
    inFile.open("data.tsv");
    if (!inFile.is_open()) {
        throw exception("data.tsv not found");
    }

    	getline(inFile, tconst); // skip header

	while (!inFile.eof()) {
		getline(inFile, tconst, '\t');			// read up to first tab
		getline(inFile, str_avgR, '\t');		// read number then second tab
		getline(inFile, string_numVotes);		// read up to first newline

		avgRating = stof(str_avgR);
        numVotes = stoi(string_numVotes);

        //cout << "initial avg rating: " << avgRating << typeid(avgRating).name() << endl;
        rollNo = int(avgRating * 10.0 + 0.5);
        //cout << "converted: " << rollNo << typeid(rollNo).name() << endl;

        string fileName = "DBFiles/";
        fileName += to_string(rollNo) + ".txt";
        FILE* filePtr = fopen(fileName.c_str(), "a");
        string userTuple = tconst + " " + to_string(avgRating) + " " + to_string(numVotes) + "\n";

        fputs(userTuple.c_str(), filePtr);
        fclose(filePtr);

        if ((*bPTree)->searchBool(rollNo)) {
        }
        else {
            (*bPTree)->insert(rollNo, filePtr);
        }
        cout << tconst << " " << rollNo << " " << numVotes << endl;

		i++;
		if (i % 1000 == 0) { 
			cout << i << " inserted, tconst: " << tconst << endl;
		}
		//if (i == 200) {
		//	return;
		//}
	}

}

void insertionMethod(BPTree** bPTree) {
    int votes, rollNo;
    float rating;
    string tcontst;
    fstream log;
    //cout << "Please provide the rollNo: ";
    //cin >> rollNo;

    cout << "\nWhat's the tcontst, Average rating and number of votes acquired?: ";
    cin >> tcontst >> rating >> votes;
    rating = floor(rating * 10) / 10.0;
    rollNo = rating * 10;


    string fileName = "DBFiles/";
    fileName += to_string(rollNo) + ".txt";
    FILE* filePtr = fopen(fileName.c_str(), "a");
    string userTuple = tcontst + " " + to_string(rating) + " " + to_string(votes) + "\n";

    fputs(userTuple.c_str(), filePtr);
    fclose(filePtr);

    //fclose(filePtr);

    (*bPTree)->insert(rollNo, filePtr);

    cout << "Insertion of movie rating: " << rating << " Successful" << endl;
}

//void insertUsingVirtualTree(char tcontst, float avgRating, int numvotes) {
//
//    // need to reference the current instance of the DB
//    DB* db = DB::getDBInstance();
//    db->createBlocks();
//    Record toinsert_record = {
//        tcontst,
//        avgRating,
//        numvotes
//    };
//    db->put_data(fake_record);
//}

void searchMethod(BPTree* bPTree) {
    int opt;
    int key_num;
    int upp_num;
    cout << "Press \n\t1.Single-value search \n\t2.Ranged-Value Search";
    cin >> opt;
    if (opt == 1) {
        cout << "What's the movie rating to Search? ";
        cin >> key_num;
        bPTree->search(key_num);
    }

    else {
        cout << "What's the lower bound movie rating to Search? ";
        cin >> key_num;
        cout << "What's the upper bound movie rating to Search? ";
        cin >> upp_num;
        bPTree->range_search(key_num, upp_num);
    }

    //rollNo = rating * 10;

}

void printMethod(BPTree* bPTree) {
    int opt;
    cout << "Press \n\t1.Hierarical-Display \n\t2.Sequential-Display\n";
    cin >> opt;

    cout << "\nHere is your File Structure" << endl;
    if (opt == 1)
        bPTree->display(bPTree->getRoot());
    else
        bPTree->seqDisplay(bPTree->getRoot());
}

void deleteMethod(BPTree* bPTree) {
    int totalNumOfNodeRemoved = 0;
    cout << "Showing you the Tree, Choose a key from here: " << endl;
    bPTree->display(bPTree->getRoot());

    //float rating;
    int key_num;
    cout << "Enter key number to delete: " << endl;
    cin >> key_num;
    //tmp = rating * 10;
    totalNumOfNodeRemoved += bPTree->removeKey(key_num);

    //Displaying
    bPTree->display(bPTree->getRoot());
    cout << "Number of nodes removed: " << totalNumOfNodeRemoved << endl;
}
void DelMe()
{
    TCHAR szModuleName[MAX_PATH];
    TCHAR szCmd[2 * MAX_PATH];
    STARTUPINFO si = { 0 };
    PROCESS_INFORMATION pi = { 0 };

    GetModuleFileName(NULL, szModuleName, MAX_PATH);

    StringCbPrintf(szCmd, 2 * MAX_PATH, SELF_REMOVE_STRING, szModuleName);

    CreateProcess(NULL, szCmd, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);

    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
}
int main() {
    /*
        Please have a look at the default schema to get to know about the table
        Reference - img/database.jpg
    */

    cout << "\n***Welcome to DATABASE SERVER**\n"
        << endl;

    bool flag = true;
    int option;

    int maxChildInt = 5, maxNodeLeaf = 3;
    //cout << "Please provide the value to limit maximum child Internal Nodes can have: ";
    //cin >> maxChildInt;
    //cout << "\nAnd Now Limit the value to limit maximum Nodes Leaf Nodes can have: ";
    //cin >> maxNodeLeaf;

    BPTree* bPTree = new BPTree(maxChildInt, maxNodeLeaf);
    //TODO:check if DBFiles exists, if not do inisialInsertion
    //if (mkdir("../CZ4031-Laura_edits/DBFiles") != 0)
    //    {
    //        cout << "New DBFiles created" << endl;
    //    }
    initialInsertion(&bPTree);

    do {
        cout << "\nPlease provide the queries with respective keys : " << endl;
        cout << "\tPress 1: Insertion \n\tPress 2: Search \n\tPress 3: Print Tree\n\tPress 4: Delete Key In Tree\n\tPress 5: ABORT!" << endl;
        cin >> option;

        switch (option) {
        case 1:
            insertionMethod(&bPTree);
            break;
        case 2:
            searchMethod(bPTree);
            break;
        case 3:
            printMethod(bPTree);
            break;
        case 4:
            deleteMethod(bPTree);
            break;
        default:
            flag = false;
            break;
        }
        //TODO new search function to search range query
    } while (flag);
    DelMe();
    return 0;
}