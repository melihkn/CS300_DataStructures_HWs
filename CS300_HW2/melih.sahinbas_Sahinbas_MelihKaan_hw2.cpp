/*
	Melih Kaan Sahinbas 31317 CS300 Homework 2
*/

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cctype>
#include "AVLTree.h"

using namespace std;

void processWord(const string& word, string& new_word) {
	new_word.clear();
	for (char c : word) {
		if (c >= 0 && c <= 255 && isalpha(c)) {
			c = tolower(c);
			new_word += c;
		}
	}
}

void toLower(const string& word, string& new_word) {
	new_word.clear();

	for (char c : word) {
		c = tolower(c);
		new_word += c;
	}
}

bool searchWords(AVLSearchTree<string, WordItem>& avl, const string& document_name, vector<string>& word_names, string& output) {
	int num_words = word_names.size(), idx2 = 0;

	// serach every word for the document name
	while (idx2 < num_words) {
		AVLNode<string, WordItem>* word_node = avl.find(word_names[idx2]);

		if (!word_node) return false; // if word does not in avl tree returns false

		TreeNode<string, DocumentItem>* doc_node = word_node->data->list.find(document_name);

		if (!doc_node) return false; // if document name does not in words bst, return false

		// if everything correct add informations to output
		output += " " + word_names[idx2] + " found " + to_string(doc_node->data->getCount()) + " times,";
		idx2++;
	}

	return true; //if nothing happened wrong beforehand returns true meaning the file contains all the words
}

void printWords(AVLSearchTree<string, WordItem>& avl, const vector<string>& document_names, vector<string>& word_names) {
	int num_documents = document_names.size(), idx = 0;
	string output;
	bool result,printed = false;

	// iterate every document
	while (idx < num_documents) {
		output.clear();

		output += "in Document " + document_names[idx] + ","; //we add outputs to this string and then if it is valid we print it

		// if result is true every word is in the document so we can print them
		result = searchWords(avl, document_names[idx], word_names, output);

		if (result) {
			output = output.substr(0, output.size() - 1);
			output += ".";
			cout << output << endl;
			printed = true;
		}
		idx++;
	}

	if (!printed) cout << "No document contains the given query" << endl;
}



int main() {

	int numDocuments;
	cout << "Enter number of input files: "; cin >> numDocuments;
	cin.ignore();
	vector<string> document_names(numDocuments);

	ifstream input_file;

	AVLSearchTree<string, WordItem> avl;

	// for every document entered process the files and insert them to trees
	for (int i = 0; i < numDocuments; i++) {
		string document_name;
		cout << "Enter " << i + 1 << ". document name: "; cin >> document_name;
		document_names[i] = document_name;

		input_file.open(document_name.c_str());

		if (input_file.fail()) {
			std::cerr << "Error opening file." << std::endl;
			return 1;
		}

		// read all the words by using while
		string word, word2add;
		while (input_file >> word) {
			processWord(word, word2add); //lowers and splits the nonalphabetical characters
			if (word2add == "") continue;
			AVLNode<string, WordItem>* current_node = avl.find(word2add);
			if (!current_node) { //means word is not in the tree
				WordItem* item = new WordItem(word2add);
				DocumentItem* doc = new DocumentItem(document_name, 1);
				item->list.insert(document_name, doc);
				avl.insert(word2add, item);
			}
			else { //word is in avl tree
				TreeNode<string, DocumentItem>* current_bst = current_node->data->list.find(document_name);

				if (!current_bst) { //there is word but the filename is not in their bst
					DocumentItem* new_doc = new DocumentItem(document_name, 1);
					current_node->data->list.insert(document_name, new_doc);

				}
				else { //word has this document so just increment the counter
					current_bst->data->incrementCount();
				}
			}
		}

		//to clear the inputstream
		input_file.seekg(0);
		input_file.close();
	}


	//avl.printTree();

	string user_query, first_word;
	vector<string> words2search;
	bool quit = false;

	cin.ignore();

	// take query inputs from user and process them
	do {
		user_query.clear();
		cout << endl << "Enter queried words in one line: "; getline(cin, user_query);

		// if user enters ENDOFINPUT break the loop otherwise make the 
		if (user_query == "ENDOFINPUT")
			quit = true;
		else {
			// analyze the query and implement it
			stringstream words(user_query);
			words >> first_word;

			if (first_word == "REMOVE") { //do the remove operations if the first word of the query is remove
				string word2remove;
				words >> word2remove;

				//check whether word is in avl or not
				AVLNode<string, WordItem>* check = avl.find(word2remove);

				if (!check)
					cout << "No document contains the given word" << endl;
				else {
					avl.remove(word2remove);
					cout << word2remove << " has been REMOVED" << endl;
				}
				//avl.printTree();
			}
			else { //just search the tree for given query

				// add every word to a vector after processing them
				string word2add;
				toLower(first_word, word2add);
				words2search.push_back(word2add);
				while (words >> first_word) {
					toLower(first_word, word2add);
					words2search.push_back(word2add);
					first_word.clear();
				}

				//this function prints output to the query entered by the user
				printWords(avl, document_names, words2search);
			}

			words2search.clear();
		}
		
	} while (!quit);

	return 0;
}
