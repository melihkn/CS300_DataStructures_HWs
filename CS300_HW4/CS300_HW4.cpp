/*
	CS300 HW4 Spring 2023-2024 Melih Kaan Þahinbaþ 31317
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>


using namespace std;

struct user {
	string name;
	string surname;
	string phone_no;
	string country;

	user() {}

	user(string name, string surname, string phone_no, string country) : name(name), surname(surname), phone_no(phone_no), country(country) {}

	// Overload the < operator
	bool operator<(const user& other) const {
		if (name < other.name) return true;
		if (name > other.name) return false;
		return surname < other.surname;
	}

	// Overload the <= operator
	bool operator<=(const user& other) const {
		if (name < other.name) return true;
		if (name > other.name) return false;
		return surname <= other.surname;
	}

};

template <class Comparable>
void swap(Comparable& obj1,
	Comparable& obj2)
{
	Comparable tmp = obj1;
	obj1 = obj2;
	obj2 = tmp;
}

template <class Comparable>
void insertionSort(vector <Comparable>& a, int left, int right)
{
	int j;
	// loop over the passes
	for (int p = left + 1; p < right; p++)
	{
		Comparable tmp = a[p];
		// loop over the elements
		for (j = p; j > 0 && tmp < a[j - 1]; j--)
			a[j] = a[j - 1];
		a[j] = tmp;
	}
}

int leftChild(int i)
{
	return 2 * i + 1;
}


template <class Comparable>  // for deleteMax
// a is the array, i is the position to percolate down from
// n is the logical size of the array
void percDown(vector<Comparable>& a, int i, int n)
{
	int child;
	Comparable tmp;

	for (tmp = a[i]; leftChild(i) < n; i = child)
	{
		child = leftChild(i);
		if (child != n - 1 && a[child] < a[child + 1])
			child++;
		if (a[child] > tmp)
			a[i] = a[child];
		else
			break;
	}
	a[i] = tmp;
}


template <class Comparable>
void heapsort(vector<Comparable>& a)
{
	// buildHeap
	for (int i = a.size() / 2; i >= 0; i--)
		percDown(a, i, a.size());

	// sort
	for (int j = a.size() - 1; j > 0; j--)
	{
		swap(a[0], a[j]);    // swap max to the last pos.
		percDown(a, 0, j); // re-form the heap
	}
}


/**
		 * Internal method that merges two sorted halves of a subarray.
		 * a is an array of Comparable items.
		 * tmpArray is an array to place the merged result.
		 * leftPos is the left-most index of the subarray.
		 * rightPos is the index of the start of the second half.
		 * rightEnd is the right-most index of the subarray.
		 */
template <class T>
void merge(vector<T>& arr, int left, int mid, int right) {
	int start1 = left;
	int start2 = mid + 1;

	// If the direct merge is already sorted
	if (arr[mid] <= arr[start2]) {
		return;
	}

	// Two pointers to maintain start of both arrays to merge
	while (start1 <= mid && start2 <= right) {
		// If element 1 is in the right place
		if (arr[start1] <= arr[start2]) {
			start1++;
		}
		else {
			T value = arr[start2];
			int index = start2;

			// Shift all the elements between element 1 and element 2 right by 1
			while (index != start1) {
				arr[index] = arr[index - 1];
				index--;
			}
			arr[start1] = value;

			// Update pointers
			start1++;
			mid++;
			start2++;
		}
	}
}

// Function to implement merge sort
template <typename T>
void mergeSort(vector<T>& arr, int left, int right) {
	if (left < right) {
		int mid = left + (right - left) / 2;

		// Sort first and second halves
		mergeSort(arr, left, mid);
		mergeSort(arr, mid + 1, right);

		// Merge sorted halves
		merge(arr, left, mid, right);
	}
}

template <class Comparable>
void mergeSort(vector<Comparable>& a)
{
	mergeSort(a, 0, a.size() - 1);
}



/** Return median of left, center, and right.
	*   Order these and hide the pivot.
  */
template <class Comparable>
const Comparable& median3(vector<Comparable>& a,
	int left, int right)
{
	int center = (left + right) / 2;
	if (a[center] < a[left])
		swap(a[left], a[center]);
	if (a[right] < a[left])
		swap(a[left], a[right]);
	if (a[right] < a[center])
		swap(a[center], a[right]);

	// Place pivot at position right - 1
	swap(a[center], a[right - 1]);
	return a[right - 1];
}

template <class Comparable>
void quicksort(vector<Comparable>& a,
	int left, int right)
{
	if (left + 10 <= right)
	{
		Comparable pivot = median3(a, left, right);
		int i = left, j = right - 1;
		for (; ; )
		{
			while (a[++i] < pivot) {}

			while (pivot < a[--j]) {}

			if (i < j)
				swap(a[i], a[j]);
			else
				break;
		}
		swap(a[i], a[right - 1]);   // Restore pivot

		quicksort(a, left, i - 1);       // Sort small elements
		quicksort(a, i + 1, right);    // Sort large elements
	}
	else  // Do an insertion sort on the subarray
		insertionSort(a, left, right);
}


template <class Comparable>
void quicksort(vector<Comparable>& a)
{
	quicksort(a, 0, a.size() - 1);
}

template <typename T>
void sequentialSearch(vector<T>& sorted, const string& key) {
	int size = sorted.size();

	for (int i = 0; i < size; i++) {
		if (sorted[i].name.find(key) != string::npos) {
			cout << sorted[i] << endl;
		}
	}
}

template <typename T>
bool contains(T& val1, string& val2) {
	//if constexpr (std::is_same<T, std::string>) {
		return val1.name.find(val2) != string::npos;
	//}
}


template <typename T>
void binarySearch(vector<T>& sorted, string& key) {
	int left = 0, right = sorted.size()-1;

	while (left <= right) {
		int mid = left + (right - left) / 2;

		// Check if key is present at mid
		if (contains(sorted[mid], key)) {
			int idx = mid - 1;
			while (contains(sorted[idx], key)) {
				idx--;
			}
			idx++;
			while (0 <= idx && idx < sorted.size() && contains(sorted[idx], key)) {
				cout << sorted[idx].name << " " << sorted[idx].surname << " " << sorted[idx].phone_no << " " << sorted[idx].country << " " << endl;
				idx++;
			}

			left = right + 1;
				
		}

		// If key greater, ignore left half
		if (sorted[mid] < key) {
			left = mid + 1;
		}
		// If key is smaller, ignore right half
		else {
			right = mid - 1;
		}
	}

}






template <typename T>
void printVector(vector<T>& vec) {
	int size = vec.size();

	for (int i = 0; i < size; i++) {
		cout << vec[i] << endl;
	}
}

void toUpperCase(string& str) {
	transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return toupper(c); });
}


int main() {

	// taking filename and query from the user
	string filename, query, word;
	cout << "Please enter the contact file name:" << endl; cin >> filename;
	cout << "Please enter the word to be queried:" << endl; cin >> query; toUpperCase(query);

	//opening the file
	ifstream file;
	file.open(filename.c_str());

	//check if file opening fails
	if (file.fail()) {
		cerr << "Error: Failed to open the file \"" << filename << "\"." << endl;
		return 1;
	}

	//create the contact vector and fill it from the file
	vector<user> contact_vector;
	vector<string> elements;
	string word2add;

	while (getline(file, word)) {
		toUpperCase(word);
		stringstream ss(word);
		while (ss >> word2add)
			elements.push_back(word2add);
		user person(elements[0], elements[1], elements[2], elements[3]);
		contact_vector.push_back(person);
	}

	//copy the vector into different vectors;
	vector<user> insertion_vector = contact_vector;
	vector<user> quick_vector = contact_vector;
	vector<user> merge_vector = contact_vector;
	vector<user> heap_vector = contact_vector;

	
	insertionSort(insertion_vector, 0, insertion_vector.size());
	quicksort(quick_vector);
	heapsort(heap_vector);
	mergeSort(merge_vector);

	//printVector(insertion_vector);

	binarySearch(insertion_vector, query);
	sequentialSearch(insertion_vector, query);






	return 0;
}