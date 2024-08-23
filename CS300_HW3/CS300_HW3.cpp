/*
	Melih Kaan Şahinbaş CS300 HW3 31317
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <chrono>
#include "HashTable.h"
#include "BinarySearchTree.h"

using namespace std;

void processWord(const string& word, string& new_word) {
	new_word.clear();
	for (char c : word) {
		c = tolower(c);
		if (c >= 97 && c <= 122) {
			new_word += c;
		}
		else {
			new_word += " ";
		}
	}
}

bool searchWords(BinarySearchTree<string, WordItem<BinarySearchTree<string, DocumentItem>>>& bst, const string& document_name, vector<string>& word_names, string& output) {
	int num_words = word_names.size(), idx2 = 0;

	// serach every word for the document name
	while (idx2 < num_words) {
		TreeNode<string, WordItem<BinarySearchTree<string, DocumentItem>>>* word_node = bst.find(word_names[idx2]);

		if (!word_node) return false; // if word does not in avl tree returns false

		TreeNode<string, DocumentItem>* doc_node = word_node->data->list.find(document_name);


		if (!doc_node) return false; // if document name does not in words bst, return false

		// if everything correct add informations to output
		output += " " + word_names[idx2] + " found " + to_string(doc_node->data->getCount()) + " times,";
		idx2++;
	}

	return true; //if nothing happened wrong beforehand returns true meaning the file contains all the words
}

void QueryDocuments(BinarySearchTree<string, WordItem<BinarySearchTree<string, DocumentItem>>>& bst, const vector<string>& document_names, vector<string>& word_names, string& out) {
	int num_documents = document_names.size(), idx = 0;
	string output;
	bool result, printed = false;

	// iterate every document
	while (idx < num_documents) {
		output.clear();

		output += "in Document " + document_names[idx] + ","; //we add outputs to this string and then if it is valid we print it

		// if result is true every word is in the document so we can print them
		result = searchWords(bst, document_names[idx], word_names, output);

		if (result) {
			output = output.substr(0, output.size() - 1);
			output += ".";
			out += output + "\n";
			printed = true;
		}
		idx++;
	}

	if (!printed) out = "No document contains the given query\n";
}


bool searchWords(HashTable<string, WordItem<HashTable<string, DocumentItem>>>& hash, const string& document_name, vector<string>& word_names, string& output) {
	int num_words = word_names.size(), idx2 = 0;

	// serach every word for the document name
	while (idx2 < num_words) {
		WordItem<HashTable<string, DocumentItem>>* word_node = hash.find(word_names[idx2]);

		if (!word_node) return false; // if word does not in avl tree returns false

		DocumentItem* doc_node = word_node->list.find(document_name);

		if (!doc_node) return false; // if document name does not in words bst, return false

		// if everything correct add informations to output
		output += " " + word_names[idx2] + " found " + to_string(doc_node->getCount()) + " times,";
		idx2++;
	}

	return true; //if nothing happened wrong beforehand returns true meaning the file contains all the words
}

void QueryDocuments(HashTable<string, WordItem<HashTable<string, DocumentItem>>>& hash, const vector<string>& document_names, vector<string>& word_names, string &out) {
	int num_documents = document_names.size(), idx = 0;
	string output;
	bool result, printed = false;

	// iterate every document
	while (idx < num_documents) {
		output.clear();

		output += "in Document " + document_names[idx] + ","; //we add outputs to this string and then if it is valid we print it

		// if result is true every word is in the document so we can print them
		result = searchWords(hash, document_names[idx], word_names, output);

		if (result) {
			output = output.substr(0, output.size() - 1);
			output += ".";
			out += output + "\n";
			printed = true;
		}
		idx++;
	}

	if (!printed) out = "No document contains the given query\n";
}

int main() {

	int numDocuments;
	cout << "Enter number of input files: "; cin >> numDocuments;
	cin.ignore();

	vector<string> document_names(numDocuments);
	string file_name_inp;
	BinarySearchTree<string, WordItem<BinarySearchTree<string, DocumentItem>>> bst;
	HashTable<string, WordItem<HashTable<string, DocumentItem>>> hash;

	//take filenames from user and store the filenames in a vector
	int i = 0;
	while (i < numDocuments) {
		cout << "Enter " << i + 1 << ". file name: "; cin >> file_name_inp;
		document_names[i] = file_name_inp;
		i++;
	}
	i = 0;

	//read files consequtively and store the words and their occurences both in a BST and a HashTable
	ifstream input_file;
	while (i < numDocuments) {
		// open the file
		string file_name = document_names[i];
		input_file.open(file_name.c_str());
		if (input_file.fail()) return 1;

		//read from file one by one
		string word, new_word;
		while (input_file >> word) {
			processWord(word, new_word); //process the word (tolower and change nonalpha characters with space)

			//using stringstream divide words one by one
			stringstream ss(new_word);
			string word2add;
			while (ss >> word2add) {
				WordItem<HashTable<string, DocumentItem>>* worditem_ptr = hash.find(word2add);

				if (!worditem_ptr) { //if element not found
					// adding values to hash
					worditem_ptr = new WordItem<HashTable<string, DocumentItem>>(word2add);
					DocumentItem* doc_item = new DocumentItem(file_name);
					worditem_ptr->list.insert(file_name, doc_item);
					hash.insert(word2add, worditem_ptr);

					//adding values to BinarySearchTree
					WordItem<BinarySearchTree<string, DocumentItem>>* bts_worditem = new WordItem<BinarySearchTree<string, DocumentItem>>(word2add);
					bts_worditem->list.insert(file_name, doc_item);
					bst.insert(word2add, bts_worditem);

				}
				else { // if element found
					DocumentItem* doc_item = worditem_ptr->list.find(file_name);

					if (!doc_item) { //document not found for the word
						doc_item = new DocumentItem(file_name);
						worditem_ptr->list.insert(file_name, doc_item);
						TreeNode<string, WordItem<BinarySearchTree<string, DocumentItem>>>* bts_worditem = bst.find(word2add);
						bts_worditem->data->list.insert(file_name, doc_item);
					}
					else { //also document found in the word's list
						doc_item->incrementCount();
					}

				}
			}

			ss.clear();
		}
		input_file.close();
		i++;
	}

	//print the necessary information about the hash table
	cout << endl  << endl << "After preprocessing, the unique word count is " << hash.getCurrentSize() << ". Current load ratio is " << hash.getLoadFactor() << endl;
	cin.ignore();

	string query, query_processed, word;
	vector<string> query_words;

	//take query from the user and process the query
	cout << "Enter queried words in one line: "; getline(cin, query);
	processWord(query, query_processed);
	stringstream ss(query_processed);

	//put every word in query to list to examine them later
	while (ss >> word) {
		query_words.push_back(word);
		word.clear();
	}
	
	string out_bst, out_hash;
	int k = 20;
	auto start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < k; i++)
	{
		out_bst.clear();
		QueryDocuments(bst, document_names, query_words, out_bst);
	}
	auto BSTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
		(std::chrono::high_resolution_clock::now() -
			start);
	cout << out_bst;
	start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < k; i++)
	{
		out_hash.clear();

		QueryDocuments(hash, document_names, query_words, out_hash);
	}
	auto HTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
		(std::chrono::high_resolution_clock::now() - start);
	cout << out_hash << endl;
	cout << "Time: " << BSTTime.count() / k << "\n";
	cout << "Time: " << HTTime.count() / k << "\n";
	cout << "Speed Up: " << BSTTime.count() / (HTTime.count() * 1.0);

	return 0;
}
