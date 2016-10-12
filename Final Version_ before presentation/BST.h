#ifndef BST_H
#define BST_H


#include <string>
#include <iostream>
#include <iomanip>
using namespace std;

struct Flight
{
	string code;//Flight code
	string carrier;//Airline brand
	int time;//Time of departure
	string departure;//Departure place
	string destination;//Destination place
	int num_passenger;//Number of passengers
	int length;//Length of the flight(hours)
};
template<class T>
class BST
{
private:
	struct BST_Node
	{
		T data;
		BST_Node *left;
		BST_Node *right;
	};
	
	BST_Node *root;
	
	int count;
	
	//Earliest and last time departure
	int earliest;
	int last;

public:
	//Constructor
	BST();
	
	//Destructor
	~BST();

	void BST_Insert(T DataIn);
	bool BST_Search(int target);

	
	bool delete_a_node(int time,string code);

	void Print_a_Node(BST_Node *address);
	bool Print_Indented_List_Tree() ;
	bool Search_Range(int a, int b);


	int get_last_flight();
	int get_earliest_flight();
	void _BST_Destroy(BST_Node *root);
private:
	void _delete_a_node(int time,string code);
	void _Print_Indented_List_Tree(BST_Node *node, int level=0) ;
	void _Search_Range(BST_Node *root, int a, int b, bool &check);
};

//Constructor
template <class T>
BST<T>::BST()
{
	root = NULL;
	count = 0;
	last = 0;
	earliest = 2400;
}
//Insert node to binary search tree.
//Input: a set of a flight data
template <class T>
void BST<T>::BST_Insert(T dataIn)
{
	
	BST_Node *pWalk = NULL;
	BST_Node *pParents = NULL;
	BST_Node *newNode;
	newNode = new BST_Node;
	if (dataIn->time < earliest)
		earliest = dataIn->time;
	if (dataIn->time > last)
		last = dataIn->time;


	newNode->data = dataIn;
	newNode->left = NULL;
	newNode->right = NULL;

	if (!root)
		root = newNode;
	else
	{
		pWalk = root;
		while (pWalk)
		{
			pParents = pWalk;
			if (pParents->data->time >= newNode->data->time)
				pWalk = pWalk->left;
			else
				pWalk = pWalk->right;
		}
		if (pParents->data->time >= newNode->data->time)
			pParents->left = newNode;
		else
			pParents->right = newNode;
	}
	count++;
}
//Search in Binary Tree
//Input: a string has flight code that we want to search.
template <class T>
bool BST<T>::BST_Search(int target)
{
	BST_Node *pWalk;
	BST_Node *pParents;

	if (!root)
		return false;
	else
	{
		pWalk = root;
		while (pWalk)
		{
			pParents = pWalk;
			if (target <= pParents->data->time)
			{
				if (target == pParents->data->time)
				{
					Print_a_Node(pParents);
					return true;
				}
				else
					pWalk = pWalk->left;
			}
			else
				pWalk = pWalk->right;
		}
		return false;
	}
}

//Print a node of a tree (only print data of that node).
template <class T>
void BST<T>::Print_a_Node(BST_Node *address)
{
	cout << address->data->code << ", ";
	cout << address->data->carrier << ", ";
	cout << "Time: " << address->data->time << ", ";
	cout << address->data->departure << " - ";
	cout << address->data->destination << ", ";
	cout << address->data->num_passenger << " passengers, ";
	cout << address->data->length << " minutes.\n\n";
}

//==========================================================================================================
//==========================================Print_Indented_Tree=============================================
//There are a function that initially call another Print indented tree function and give it the starting point is root.
//=>return false when there is no root which means the tree is not exist.
//The second function has an input which is a the root of the tree that we want to print.
//It returns nothing, just prints the indented tree that we can really see its content.
//==========================================================================================================

template <class T>
bool BST<T>::Print_Indented_List_Tree()
{
	if (!root) return false;
	else
	{
		cout << ' ';
		_Print_Indented_List_Tree(root, 0);
		return true;
	}
}
template <class T>
void BST<T>::_Print_Indented_List_Tree(BST_Node *node, int level)
{
	if (node != NULL)
	{
		if (node->right) _Print_Indented_List_Tree(node->right, level + 1);
		if (level)
		{
			cout << setw(level * 5);
		}
		cout << level << '.' << node->data->code << "," << node->data->time << "\n ";
		if (node->left) _Print_Indented_List_Tree(node->left, level + 1);
	}

}
//Delete
template <class T>
bool BST<T>::delete_a_node(int key,string code)
{
	if (root)
	{
		_delete_a_node(key,code);
	}
	return (root) ? true : false;
}


template<class T>
void BST<T>::_delete_a_node(int time, string code)
{
	// Find the item corresponding to the inserted Key
	bool found = false;
	BST_Node* previous = NULL;
	BST_Node* current = root;
	if (current == NULL)
	{
		cout << "The tree is empty. Can not delete from empty tree." << endl;
		return;
	}
	while (current != NULL)
	{
		if (current->data->time == time)
		{
			found = true;
			break;
		}
		else
		{
			previous = current;
			if (time > (current->data->time))
			
				current = current->right;
			else
				current = current->left;
		}
	}
	if (!found)
	{
		cout << "The flight with code "<<code<< " and the time departure " << time << " is not in the tree.";
		return;
	}


	// CASE 1: Only 1 child

	if ((current->left == NULL && current->right != NULL) || (current->left != NULL
		&& current->right == NULL))
	{

		// When there is only 1 right child, no left child
		if (current->left == NULL && current->right != NULL)
		{
			// test to see if previous's left is equal to the input BinaryNode
			if (previous->left == current)
			{
				//previous's right is now current's right
				previous->left = current->right;
				delete current;
				current = NULL;
				cout << "The flight with code " << code << " and the time departure " << time << " has been deleted. " << endl;
			}
			else
			{
				// if previous's right is equal to the input BinaryNode
				previous->right = current->right;
				delete current;
				current = NULL;
				cout << "The flight with code " << code << " and the time departure " << time << " has been deleted. " << endl;
			}
		}
		else
		{
			if (previous->left == current)
			{
				//previous's left is now current's left
				previous->left = current->left;
				delete current;
				current = NULL;
				cout << "The flight with code " << code << " and the time departure " << time << " has been deleted. " << endl;

			}
			else
			{
				previous->right = current->left;
				delete current;
				current = NULL;
				cout << "The flight with code " << code << " and the time departure " << time << " has been deleted. " << endl;
			}
		}
		return;
	}
	// CASE 2: Deleting a leaf with NO children
	if (current->left == NULL && current->right == NULL)
	{
		if (previous->left == current)
			previous->left = NULL;
		else
			previous->right = NULL;
		delete current;
		cout << "The flight with code " << code << " and the time departure " << time << " has been deleted. " << endl;
		return;
	}
	//CASE 3: Parent node has 2 children
	// Replace current with smallest of the right subtree
	if ((current->left != NULL) && (current->right != NULL))
	{
		BST_Node* temp = current->right;
		if ((current->left == NULL) && (current->right == NULL))
		{
			current = temp;
			delete temp;
			current->right = NULL;
			cout << "The flight with code " << code << " and the time departure " << time << " has been deleted. " << endl;
		}
		else //right child has children
		{// if the node's right has left child, keep moving left
			// until you reach the smallest
			if ((current->right)->left != NULL)
			{
				BST_Node* leftPrev;
				BST_Node* leftCurr;
				leftPrev = current->right;
				leftCurr = (current->right)->left;
				while (leftCurr->left != NULL)
				{
					// keep traversing all the way left until you hit NULL
					leftPrev = leftCurr;
					leftCurr = leftCurr->left;
				}
				current->data = leftCurr->data;
				delete leftCurr;
				leftPrev->left = NULL;
				cout << "The flight with code " << code << " and the time departure " << time << " has been deleted. " << endl;
			}
			else
			{
				BST_Node* tempNode = current->right;
				current->data = tempNode->data;
				current->right = temp->right;
				delete tempNode;
				cout << "The flight with code " << code << " and the time departure " << time << " has been deleted. " << endl;
			}
		}
		return;
	}
}
//==========================================================================================================
//===================================SEARCH ELEMENTS IN GIVEN RANGE=========================================
//There are a function that initially call another Search range Funtion and give it the starting point is root.
//=>return false when there is no root which means the tree is not exist.
//The second function has an input which is a the root of the tree that we want to find.
//It returns nothing, just prints out all of elements in a given range.
//==========================================================================================================
template <class T>
bool BST<T>::Search_Range( int a, int b)
{
	bool check = false;
	if (!root)
		return false;
	else
		_Search_Range(root, a, b, check);
	if (check == true)
		return true;
	else 
		return false;
}
template <class T>
void BST<T>::_Search_Range(BST_Node* root,int a, int b,bool &check)
{
	if (root)
	{
		if (a < root->data->time)
			_Search_Range(root->left, a, b, check);
		if (a <= root->data->time && b >= root->data->time)
		{
			cout << root->data->code << " ";
			check = true;
		}
		if (b > root->data->time)
			_Search_Range(root->right, a, b, check);

	}
}
template <class T>
int BST<T>::get_last_flight()
{
	return last;
}
template <class T>
int BST<T>::get_earliest_flight()
{
	return earliest;
}
template <class T>
BST<T>::~BST()
{
	if (root)
		_BST_Destroy(root);
}

/**~*~*
This function traverses the binary tree in postorder and deletes every node
*~**/
template <class T>
void BST<T>::_BST_Destroy(BST_Node *root)
{
	if (root)
	{
		_BST_Destroy(root->left);
		_BST_Destroy(root->right);
	}
	delete root;

	return;
}


#endif