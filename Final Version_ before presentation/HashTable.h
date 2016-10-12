#ifndef HASHING_H
#define HASHING_H
#include <string>
#include <iostream>
using namespace std;

const int  max_collision = 3;



template<class T>
class Hash
{
private:
	struct Bucket
	{
		T Data[max_collision];
		int count;
	};
	int prime_table_size = 31;
	Bucket** Hash_Table;
	int number_key;
	int number_full_bucket;
	int number_collision;
	double load_factor;


public:
	Hash();
	~Hash();
	int Hashkey(string DataIn);
	bool hash_Insert(int index, T node_DataInput);
	bool hash_Search(string code);
	bool hash_Delete(string code, int &time);
	void print_Hash_table();
	void display_Hash();
	int New_Prime_Number();
	void print_Statistics();
	void Update_Load_Factor();
	void creat_tempHashTable(int old_hash, int numb_key);
	void Rehash(Bucket **temp_HashTable, int old_hash, int num_keys);
	void Print_a_Flight(int index, int b_index);
	bool check_available_slot(string code);

//private:
	//void print_a_flight(T address);
};

//Constructor

template<class T>
Hash<T>::Hash()
{
	Hash_Table = new Bucket*[prime_table_size]; 
	for (int i = 0; i < prime_table_size; i++)
	{
		Hash_Table[i] = new Bucket;
		Hash_Table[i]->count = 0;
		for (int j = 0; j < max_collision; j++)
			Hash_Table[i]->Data[j] = NULL;
		
	};
	number_key = 0;
	number_full_bucket = 0;
	number_collision = 0;
}

//Destructor
template<class T>
Hash<T>::~Hash()
{
	for (int i = 0; i < prime_table_size; i++)
		delete Hash_Table[i];
	number_key = 0;
}

//Convert a key from User input
template<class T>
int Hash<T>::Hashkey(string DataIn)
{
	int sum_ascii = 0;
	int n = DataIn.length();
	for (int i = 0; i < n; i++)
		sum_ascii += (int)DataIn[i];
	return (sum_ascii % prime_table_size);
}
template<class T>
bool Hash<T>::check_available_slot(string code)
{
	int index = Hashkey(code);
	if (Hash_Table[index]->count < max_collision)
		return true;
	else
		return false;
}
//Put data into HashTable by numerical key
template<class T>
bool Hash<T>::hash_Insert(int index, T node_DataInput)
{
	//if the Bucket is full, then stop insert the data in that bucket
	if (Hash_Table[index]->count < max_collision)
	{
		int pos = Hash_Table[index]->count;
		if (pos >= 1)
			number_collision++;
		if (pos == 0)
			number_key++;
		Hash_Table[index]->Data[pos] = node_DataInput;
		Hash_Table[index]->count++;
		if (Hash_Table[index]->count == max_collision)
			number_full_bucket++;
	}
	//REHASH PART
	Update_Load_Factor();
	cout << load_factor << endl;
	if (load_factor> 75.0 )
	{
		cout << "Start-rehash"<<endl;
		int Old_Prime = prime_table_size;
		prime_table_size = New_Prime_Number();
		creat_tempHashTable( Old_Prime, number_key );
	}
	return true;
}
//SEARCH FOR A KEY IN THE HASH TABLE
//INPUT: A STRING KEY ->CONVERT TO INDEX OF THE HASH TABLE
//OUTPUT: INFORMATION ABOUT THAT KEY IF EXIST.
template<class T>
bool Hash<T>::hash_Search(string code)
{
	int index = Hashkey(code);
	bool found = false;
	for (int j = 0; j < Hash_Table[index]->count; j++)
	{
		if (Hash_Table[index]->Data[j]->code == code)
		{	
			found = true;
			Print_a_Flight(index, j);
					}
	}
	return found;
}
//DISPLAY ALL DATA WHICH IS EXIST IN THE HASH TABLE
//NO INPUT AND OUTPUT, JUST DISPLAY ALL DATA.
template<class T>
void Hash<T>::display_Hash()
{
	for (int i = 0; i < prime_table_size; i++)
	{
		for (int j = 0; j < Hash_Table[i]->count; j++)
		{
			Print_a_Flight(i, j);
		}	
	}
}
//PRINT ALL OF DATA IN HASH TABLE
//NO INPUT AND OUTPUT, JUST PRINT THE DATA
//LEAVE BLANKS IF THERE IS NO DATA IN A BUCKET
template<class T>
void Hash<T>::print_Hash_table()
{
	for (int i = 0; i < prime_table_size; i++)
	{
		cout << "Bucket " << i <<"\n";
		for (int j = 0; j < max_collision; j++)
			if (Hash_Table[i]->Data[j] != NULL)
				cout << "\t Location " << j << ":"  << Hash_Table[i]->Data[j]->code << endl;
			else
				cout << "\t Location " << j << ":" << endl;
		
	}

}
//Show statistics, such as number of collisions, load factor (number of keys / number of
//buckets), number of full buckets, etc.
//No input and output, just display the statistics
template <class T>
void Hash<T>::print_Statistics()
{
	Update_Load_Factor();
	cout << "Number of keys: " << number_key << endl;
	cout << "Number of collisions: " << number_collision << endl;
	cout << "Number of full buckets: " << number_full_bucket << endl;
	cout << "Load factor: " << load_factor <<"% " << endl;
	cout << "Next Prime Number :"<< New_Prime_Number()<< endl;

}
template <class T>
bool Hash<T>::hash_Delete(string code,int &time)
{
	int index = Hashkey(code);
	for (int j = 0; j < Hash_Table[index]->count; j++)
	{
		if (Hash_Table[index]->Data[j]->code == code)
		{
			
			//Save the time of the flight so we can find that flight in the BST
			time = Hash_Table[index]->Data[j]->time;
			int count = Hash_Table[index]->count;
			Hash_Table[index]->Data[j] = NULL;
			for (int k = j; k < count-1; k++)
				Hash_Table[index]->Data[k] = Hash_Table[index]->Data[k + 1];
			Hash_Table[index]->count--;
			return true;
		}
	}
	return false;
}
template<class T>
void Hash<T>::Update_Load_Factor()
{
	load_factor = (number_key / (double)prime_table_size) * 100;

}

template<class T>
int Hash<T>::New_Prime_Number()
{
	int test = prime_table_size ;
	test = 2 * test;
	bool found;
	int count;
	do
	{
		found = false;
		count = 0;
		int i = 1;
		while (count<2 && i<test)
		{	
			if ((test % i)==0)
				count++;
			i++;
		}
		if ( (count ==1) && (i==test))
			found=true;
		else test++;
		
	} while (!found);
	
	
	return test;
}
template<class T>
void Hash<T>::creat_tempHashTable(int old_hash, int num_keys)
{
	Bucket ** temp_HashTable = new Bucket*[prime_table_size];

	for (int i = 0; i < prime_table_size; i++)
	{
		temp_HashTable[i] = new Bucket;
		temp_HashTable[i]->count = 0;
		for (int j = 0; j < max_collision; j++)
			temp_HashTable[i]->Data[j] = NULL;
	}
	Rehash(temp_HashTable, old_hash, num_keys);
	delete[]Hash_Table;
	Hash_Table = temp_HashTable;
}
template<class T>
void Hash<T>::Rehash(Bucket **temp_HashTable, int old_hash, int num_keys)
{
	for (int index = 0; index < old_hash; index++)
	{
		for (int j = 0; j < max_collision; j++)
		{
			if (Hash_Table[index]->Data[j] != NULL)
			{ 
				
				int new_index = Hashkey(Hash_Table[index]->Data[j]->code);
				int pos = temp_HashTable[new_index]->count;
				cout << Hash_Table[index]->Data[j]->code << " " << index << "  => " << new_index << endl;
				temp_HashTable[new_index]->Data[pos] = Hash_Table[index]->Data[j];
				temp_HashTable[new_index]->count++;

			}
		}
	}
}
template<class T>
void Hash<T>::Print_a_Flight(int index, int b_index)
{
	cout << left << setw(20) << "Flight Code: " << right << setw(20) << Hash_Table[index]->Data[b_index]->code << endl;
	cout << left << setw(20) << "Brand: " << right << setw(20) << Hash_Table[index]->Data[b_index]->carrier << endl;
	cout << left << setw(20) << "Time of Departure: " << right << setw(20) << Hash_Table[index]->Data[b_index]->time << endl;
	cout << left << setw(20) << "Departure: " << right << setw(20) << Hash_Table[index]->Data[b_index]->departure << endl;
	cout << left << setw(20) << "Destination: " << right << setw(20) << Hash_Table[index]->Data[b_index]->destination << endl;
	cout << left << setw(20) << "Numb of Passenger: " << right << setw(20) << Hash_Table[index]->Data[b_index]->num_passenger << endl;
	cout << left << setw(20) << "Duration (minutes): " << right << setw(20) << Hash_Table[index]->Data[b_index]->length << endl<< endl;
	cout << "----------------------------------------\n" << endl;
}
/*
template <class T>
void Hash<T>::print_a_flight(T address)
{
	cout << address->code << ", " << address->carrier << ", Departure: " << address->departure << "Arrival: " << address->destination << endl ;
	count << "Time: " << address->time << "Estimated length of flight: " << address->length << " minutes.\n";
}
*/
#endif
