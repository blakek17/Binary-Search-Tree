//--------------------------------------------------------------------
//
//  Laboratory 11, In-lab Exercise 1                     database.cpp
//
//  (Shell) Indexed accounts database program
//
//--------------------------------------------------------------------

// Builds a binary search tree index for the account records in the
// text file accounts.dat.

#include <iostream>
#include <fstream>
#include <string>
#include "BSTree.cpp"

using namespace std;

//--------------------------------------------------------------------
//
// Declarations specifying the accounts database
//

const int nameLength = 11;   // Maximum number of characters in
								  //   a name
const long bytesPerRecord = 38;   // Number of bytes used to store
								  //   each record in the accounts
								  //   database file

struct AccountRecord
{
	int acctID;                   // Account identifier
	char firstName[nameLength],   // Name of account holder
		lastName[nameLength];
	double balance;               // Account balance
};

//--------------------------------------------------------------------
//
// Declaration specifying the database index
//

struct IndexEntry
{
	int acctID;              // (Key) Account identifier
	long recNum;             // Record number

	int getKey() const
	{
		return acctID;
	}   // Return key field
};

//--------------------------------------------------------------------

int main()
{
	ifstream acctFile("accounts.dat");   // Accounts database file
	AccountRecord acctRec;                // Account record
	BSTree<IndexEntry, int> index;         // Database index
	IndexEntry entry;                     // Index entry
	int searchID;                         // User input account ID
	long recNum;                          // Record number
	string line;

	// Iterate through the database records. For each record, read the
	// account ID and add the (account ID, record number) pair to the
	// index.
	while (acctFile >> recNum >> searchID) {
		entry.recNum = recNum;
		entry.acctID = searchID;
		index.insert(entry);
		getline(acctFile, line);
	}

	// Output the account IDs in ascending order.
	cout << "Valid account IDs:" << endl;
	index.writeKeys();

	// Clear the status flags for the database file.
	acctFile.clear();
	acctFile.seekg(0, ios::beg);

	// Read an account ID from the keyboard and output the
	// corresponding record.
	while (true) {
		cout << "Enter an account ID to view that account's information." << endl
			<< "Enter '0' to exit the program." << endl;
		cin >> searchID;
		if (searchID == 0) { break; }
		else if (cin && index.retrieve(searchID, entry)) {
			for (int fileLine = 0; fileLine < entry.recNum; fileLine++) {
				getline(acctFile, line);
			}
			if (acctFile >> recNum >> acctRec.acctID
				>> acctRec.firstName >> acctRec.lastName >> acctRec.balance) {
				cout << endl << "Account ID: " << acctRec.acctID << endl
					<< "First Name: " << acctRec.firstName << endl
					<< "Last Name: " << acctRec.lastName << endl
					<< "Balance: $ " << acctRec.balance << endl << endl;
			}
			else { cout << "Errors found in record" << endl << endl; }
		}
		else { 
			cout << endl << "This account number is not valid." << endl << endl;
			cin.clear();
			cin.ignore(100, '\n');
		}
		cout << "Valid account IDs:" << endl;
		index.writeKeys();
	}

	return 0;
}
