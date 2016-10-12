#define _CRT_SECURE_NO_WARNINGS
#include <fstream>
#include <string>
#include <stdlib.h>     /* atoi */
#include <stdio.h>
#include "BST.h"
#include "HashTable.h"
#include "Heap.h"
#include <iostream>
using namespace std;

const char SEARCH = 'S',
		PRINT = 'P',
		INSERT = 'I',
		DELETE = 'D',
		STATISTICS = 'X',
		SHOW_MENU = 'M',
		SHOW_MOST_FREQUENTLY = 'F',
		QUIT = 'Q';
struct Manager
{
	BST<Flight*> *Obj_BST;
	Hash<Flight*> *Obj_Hash;
	Heap<string> *Obj_Heap;
};
void inputFunction(Manager *Head);
void addFlight(Manager *Head);
void delete_a_flight(Manager *Head);

int convert_length(string string_length);
int convert_time(string time);

void Search(Manager *Head);
void Search_Time(Manager *Head);
void Search_Code(Manager *Head);
void Search_Range(Manager *Head);

void Print(Manager *Head);
void Print_All_Flights(Manager *Head);
void Print_Indented_Tree(Manager *Head);
void Print_Hash_Table(Manager *Head);

void Show_Statistics(Manager *Head);
void Show_Most_Frequently(Manager* Head);
void Show_Menu();

char Get_Choice();

void Print(Manager *Head);


int main()
{
	Manager *Head;
	Head = new Manager;
	Head->Obj_BST =new BST<Flight*>;
	Head->Obj_Hash = new Hash<Flight*>;
	Head->Obj_Heap = new Heap < string > ;
	//readFile(Obj_BST);
	inputFunction(Head);

	Show_Menu();
	char inp;
	inp = '0';
	while ((inp = Get_Choice()) != QUIT)
	{
		switch (inp)
		{
		case INSERT:
			addFlight(Head);
			break;
		case DELETE:
			delete_a_flight(Head);
			break;
		case SEARCH:
			Search(Head);
			break;
		case PRINT:
			Print(Head);
			break;
		case STATISTICS:
			Show_Statistics(Head);
			break;
		case SHOW_MOST_FREQUENTLY:
			Show_Most_Frequently(Head);
			break;
		case SHOW_MENU:
			Show_Menu();
			break;


		}

	}

	return 0;
}

//********************************************************************
//Get_Choice()
//Get option input from user and check if it invalid
//Return the character that fits our choice of option.
//********************************************************************
char Get_Choice()
{
	string input = "";
	cout << "\nPlease enter your choice( S, P, I, D, F, X, M or Q):";
	cin >> input;
	while (input != "S" && input != "P" && input != "I" && input != "D" && input != "X" && input != "F" && input != "M" && input != "Q" && input != "s" && input != "p" && input != "f" && input != "i" && input != "d" && input != "x" && input != "m" && input != "q")
	{
		cout << "\nInvalid input! Please re-enter( S, P, I, D, F, X, M or Q):";
		cin >> input;
	};
	return toupper(input[0]);

}
//*********************************************************************
//Read_input_from file
//Then insert it into the binary tree by using input Object pointer to the BST<Flight*>
//input: a pointer to the tree
//*********************************************************************
void inputFunction(Manager *Head)
{
	char limiter(',');
	string tempString = "";
	
	int key=0;

	ifstream inFile;
	inFile.open("Project_input.txt");
	if (!inFile.is_open()){//If file is invalid or does not open, quits the program
		cout << "Input file not found, exiting. . ." << endl;

	}

	while (inFile.good())
	{
		Flight *dataIn;
		dataIn = new Flight;
		getline(inFile, dataIn->code, limiter);
		getline(inFile, dataIn->carrier, limiter);
		getline(inFile, tempString, limiter);

		tempString = tempString.replace(2, 1, "");
		dataIn->time = atoi(tempString.c_str());

		getline(inFile, tempString, limiter);
		dataIn->departure = tempString.substr(0, 3);
		dataIn->destination = tempString.substr(4, 3);


		getline(inFile, tempString, limiter);
		dataIn->num_passenger = atoi(tempString.c_str());

		getline(inFile, tempString);
		dataIn->length = convert_length(tempString);
		Head->Obj_BST->BST_Insert(dataIn);

		int Hash_index = Head->Obj_Hash->Hashkey(dataIn->code);
		Head->Obj_Hash->hash_Insert(Hash_index, dataIn);
	}

}

//=================================== addFlight method ==============================================
// asks user to input the data of the flight to add
// it then calls the private member function from the Binary Search Tree class
// It has no return value; only appends the new data to the tree
//===================================================================================================
void addFlight(Manager *Head)
{
	
	string tempString,string_time, string_length;
	int key=0;
	string decision = "Y";

	while (decision == "Y")
	{
		Flight *dataInput;
		dataInput = new Flight;
		// input flight code
		cout << "Please enter the Flight Code : ";
		cin.ignore();
		getline(cin, dataInput->code);

		if (Head->Obj_Hash->check_available_slot(dataInput->code))
		{
			// input the carrier name
			cout << "\nPlease enter the Carrier Name : ";
			getline(cin, dataInput->carrier);

			// input the departure time
			cout << "\nPlease enter the Depature Time : ";
			getline(cin, string_time);
			string_time = string_time.replace(2, 1, "");
			dataInput->time = atoi(tempString.c_str());

			// input the airport of depature
			cout << "\nPlease enter the Airport of Depature : ";
			getline(cin, dataInput->departure);

			// input the airport of arrival
			cout << "\nPlease enter the Airport of Arrival : ";
			getline(cin, dataInput->destination);

			// input the maximum number of passengers
			cout << "\nPlease enter the Number of Passengers : ";
			cin >> dataInput->num_passenger; //because it is an integer
			cin.ignore();

			// input the arrival time
			cout << "\nPlease enter the Length of Flight : ";
			getline(cin, string_length);
			dataInput->length = convert_length(string_length);
			int Hash_index = Head->Obj_Hash->Hashkey(dataInput->code);
			Head->Obj_Hash->hash_Insert(Hash_index, dataInput);
			Head->Obj_BST->BST_Insert(dataInput);
			cout << "Flight " << dataInput->code << "was inserted to hash table!\n\n";
		}
			//IF we cant insert to hash table, then do nothing with the BST.
		else 
			cout << "Flight " << dataInput->code << "can not be inserted because lack of memory.\n\n";

		
		
		cout << "Would you like to enter a new flight? " << endl;
		cout << "Enter 'Y' to add another flight, or anything else to stop. " << endl;
		cin >> decision;
	};
}
void delete_a_flight(Manager *Head)

{
	string code;
	cout << "Enter the key to delete the flight of:" << endl;
	cin.ignore();
	getline(cin, code);
	int time = 0;
	//Delele from Hash table
	if (!Head->Obj_Hash->hash_Delete(code, time))//If cant find data, then do nothing.
		cout << "Flight " << code << " cant be found!\n\n";
	else//Delete from Tree
		Head->Obj_BST->delete_a_node(time, code);

}

int convert_length(string string_length)
{
	int hour, min;
	string temphour, tempmin;
	temphour = string_length.substr(0, 2);
	tempmin = string_length.substr(3, 2);
	hour = atoi(temphour.c_str());
	min = atoi(tempmin.c_str());
	return ((hour * 60) + min);
}

void Search(Manager *Head)
{
	string input;
	cout << "What do you want to search for?" << endl;
	cout << "C - Flight Code. " << endl; 
	cout << "T - Time of departure." << endl;
	cout << "R - All flights in a range of time." << endl;
	cin.ignore();
	cout << "Please enter:";
	getline(cin, input);
	while (input != "Q" && input != "C" && input != "T" && input != "R" && input!= "q" && input != "c" && input != "t" && input != "r")
	{
		cout << "\nInvalid input! Please re-enter(C, T, or R):";
		cin >> input;
	}
	char choice = toupper(input[0]);
	switch (choice)
	{
	case 'C':
		Search_Code(Head);
		break;
	case 'T':
		Search_Time(Head);
		break;
	case 'R':
		Search_Range(Head);
		break;
	case QUIT:
		break;
	}

};



//Search a flight code in the tree until the user enter 'Q' or 'q'
void Search_Time(Manager *Head)
{
	string temp_time;
	cout << "What is the departure time that you want to find?('Q' or 'q' to Quit)";
	cin >> temp_time;
	int search_BST = convert_time(temp_time);

	if (!(Head->Obj_BST->BST_Search(search_BST)))
		cout << "There is not exist a flight which has time departure " << temp_time << ".\n\n";

}
void Search_Code(Manager *Head)
{
	string temp_code;
	cout << "What is the flight code you want to find?('Q' or 'q' to Quit)";
	cin >> temp_code;
	if (!(Head->Obj_Hash->hash_Search(temp_code)))
		cout << "There is not exist a flight which has code " << temp_code << " .\n\n";
	else
		Head->Obj_Heap->insertheap(temp_code);

}

void Search_Range(Manager *Head)
{
	int a, b;
	string tempString1, tempString2;
	cout << "Enter the range of time that you want to search flights.";
	cout << "( between " << Head->Obj_BST->get_earliest_flight() << " and " << Head->Obj_BST->get_last_flight() << ") : \n";
	cout << "Enter lower bound:"; cin >> tempString1;
	a = convert_time(tempString1);

	cout << "Enter upper bound:"; cin >> tempString2;
	b = convert_time(tempString2);
	
	if (!Head->Obj_BST->Search_Range(a, b))
		cout << "Cant find any flight from that range of time!";
	cout << "\n\n";
}

void Print(Manager *Head)
{
	string input;
	cout << "What do you want to print?" << endl;
	cout << "A - Print all flights. " << endl;
	cout << "T - BST Indented Tree. " << endl;
	cout << "H - Hash Table." << endl;
	cout << "Q - Quit." << endl;
	cin.ignore();
	cout << "Please enter an option:";
	getline(cin, input);
	while (input != "Q" && input != "A" && input != "T" && input != "H" && input != "q" && input != "a"  && input != "t" && input != "h")
	{
		cout << "\nInvalid input! Please re-enter(T or H):";
		cin >> input;
	}
	char choice = toupper(input[0]);
	switch (choice)
	{
	case 'A':
		Print_All_Flights(Head);
		break;
	case 'T':
		Print_Indented_Tree(Head);
		break;
	case 'H':
		Print_Hash_Table(Head);
		break;
	case QUIT:
		break;
	}

}
void Print_All_Flights(Manager *Head)
{
	Head->Obj_Hash->display_Hash();
}
//Print indented tree that we can visually recognize.
void Print_Indented_Tree(Manager *Head)
{
	if (!Head->Obj_BST->Print_Indented_List_Tree())
		cout << "Binary Search Tree does not exist!";

}
void Print_Hash_Table(Manager *Head)
{
	Head->Obj_Hash->print_Hash_table();
}
void Show_Statistics(Manager *Head)
{
	Head->Obj_Hash->print_Statistics();
}

//Print the menu to the screen.
void Show_Menu()
{
	cout << "Please select an option from the menu:\n";
	cout << "S - Search Flights.\n";
	cout << "P - Print Flights.\n";
	cout << "I - Insert a flight.\n";
	cout << "D - Delete a flight.\n";
	cout << "X - Show Statistics of Hash table.\n";
	cout << "F - Show most frequently searched.\n";
	cout << "M - Show the Menu.\n";
	cout << "Q - Quit the Program.\n";
}
void Show_Most_Frequently(Manager* Head)
{
	if (Head->Obj_Heap->getTop() != "")
		cout<<"Most frequently searched item is: "<< Head->Obj_Heap->getTop()<<"\n\n";
	else cout << "There is not any flight was searched.\n\n";


}

int convert_time(string time)
{
	int length;
	int convert;
	length = time.length();
	if (length == 5)
		time = time.replace(2, 1, "");
	if (length == 4)
		time = time.replace(1, 1, "");
	convert = atoi(time.c_str());
	return convert;
}
