/*
	Created by Melih Kaan Sahinbas 31317 on 10.03.2003
*/

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <random>


using namespace std;


// struct to store relevant information about cell in maze
struct cell {
	int x;
	int y;
	bool l;
	bool r;
	bool u;
	bool d;
	bool visited;

	cell(int x, int y) : x(x), y(y), visited(0), l(1), r(1), d(1), u(1) {}

};


// template stack class to both generate maze and find path
template <typename T>
class Stack {
private:
	// keep data with pointer because we might change the value in the maze so we prevent copying and an change with using stack also will be implemented in original maze
	struct node {
		T* data;
		node* next;
	};
	node* top;
public:
	Stack() {
		top = nullptr;
	}

	~Stack() {
		node* temp = top;

		while (top) {
			top = temp->next;
			delete temp;
			temp = nullptr; temp = top;
		}
	}

	T* getTop() {
		return top->data;
	}

	bool isEmpty() {
		return top == nullptr;
	}

	void push(cell* value) {
		node* new_node = new node;
		new_node->data = value;
		new_node->next = top;
		top = new_node;
	}

	void pop() {
		if (isEmpty()) return;

		node* temp;
		if (!top->next) { //tek eleman varsa
			temp = top;
			delete temp;
			top = nullptr;
		}
		else {
			temp = top;
			top = top->next;
			delete temp;
			temp = nullptr;
		}
	}

	void printStack(ostream& out) {

		if (isEmpty())
			return;

		T* temp = getTop();
		pop();

		printStack(out);

		out << temp->x << " " << temp->y << endl;

		push(temp);
	}


};


// this function prints cell informations to a txt file
void vector2file(ofstream& out, vector<vector<cell>>& maze) {
	int y = maze.size(), x = maze[0].size();
	cell* current_node = nullptr;

	out << y << " " << x << endl;

	for (int i = 0; i < y; i++) {
		for (int j = 0; j < x; j++) {
			current_node = &maze[y - i - 1][j];
			out << "x=" << current_node->x << " " << "y=" << current_node->y << " " << "l=" << current_node->l << " "
				<< "r=" << current_node->r << " " << "u=" << current_node->u << " " << "d=" << current_node->d << endl;
			/*cout << "x=" << current_node->x << " " << "y=" << current_node->y << " " << "l=" << current_node->l << " "
				<< "r=" << current_node->r << " " << "u=" << current_node->u << " " << "d=" << current_node->d << endl;*/
		}
	}
}


//check whether given coordinate is in the scope of maze or not
bool is_valid_coordinate(int x, int y, int rows, int cols) {
	return (x >= 0 && x < cols&& y >= 0 && y < rows);
}

// this function finds possible directions of a cell in generation part, stores addresses of possible direction in a vector
void findPossibleDirections(vector<vector<cell>>& maze, cell* current_cell, vector<cell*>& possible_directions) {
	int row_size = maze.size(); // y
	int col_size = maze[0].size(); // x
	int x = 0, y = 0;

	// in order to prevent errors if current_cell is nullptr
	if (current_cell) {
		y = current_cell->y; x = current_cell->x;
	}

	if (is_valid_coordinate(x + 1, row_size - 1 - y, row_size, col_size)) {
		// Can go right
		if (maze[row_size - 1 - y][x + 1].visited == false) {
			possible_directions.push_back(&maze[row_size - 1 - y][x + 1]);
		}
	}

	if (is_valid_coordinate(x - 1, row_size - 1 - y, row_size, col_size)) {
		// Can go left
		if (maze[row_size - 1 - y][x - 1].visited == false) {
			possible_directions.push_back(&maze[row_size - 1 - y][x - 1]);
		}
	}

	if (is_valid_coordinate(x, row_size - y, row_size, col_size)) {
		// Can go down
		if (maze[row_size - y][x].visited == false) {
			possible_directions.push_back(&maze[row_size - y][x]);
		}
	}

	if (is_valid_coordinate(x, row_size - y - 2, row_size, col_size)) {
		// Can go up
		if (maze[row_size - y - 2][x].visited == false) {
			possible_directions.push_back(&maze[row_size - y - 2][x]);
		}
	}

}

// this function destroy the walls while generating the maze, used pointers so original maze also changed
void destroyWalls(cell* current_node, cell* next_node) {
	int curr_x = current_node->x, curr_y = current_node->y, next_x = next_node->x, next_y = next_node->y;

	if (curr_x > next_x) {
		current_node->l = false;
		next_node->r = false;
	}
	else if (curr_x < next_x) {
		current_node->r = false;
		next_node->l = false;
	}
	else if (curr_y > next_y) {
		current_node->d = false;
		next_node->u = false;
	}
	else {
		current_node->u = false;
		next_node->d = false;
	}
}

int getRandomNumber(int min, int max) {
	// Initialize random number generator with a seed
	std::random_device rd;
	std::mt19937 gen(rd());

	// Define the range
	std::uniform_int_distribution<> dis(min, max);

	// Generate and return the random number
	return dis(gen);
}


// this function generates maze
void generateMaze_(vector<vector<cell>>& maze, Stack<cell>& maze_stack, const int& size) {
	cell* current_node = nullptr;
	cell* next_node = nullptr;
	int row_size = maze.size(), idx, visited_count = 1;

	current_node = &(maze[row_size - 1][0]);
	current_node->visited = true;
	maze_stack.push(current_node); //push first element

	vector<cell*> possible_directions;

	while (!maze_stack.isEmpty()) {
		current_node = maze_stack.getTop();
		maze_stack.pop();
		findPossibleDirections(maze, current_node, possible_directions);

		// if there is/are possible way/s randomly go to one of them
		if (possible_directions.size() != 0) {
			idx = getRandomNumber(0, possible_directions.size() - 1);
			next_node = possible_directions[idx];
			destroyWalls(current_node, next_node); // destroy walls between current and next nodes
			next_node->visited = true;
			visited_count++;
			maze_stack.push(current_node); maze_stack.push(next_node);
		}
		possible_directions.clear();
	}

}


void fillEmptyMatrix(vector<vector<cell>>& maze) {
	int row = maze.size(), col = maze[0].size();

	for (int i = 0; i < col; i++) {
		for (int j = 0; j < row; j++) {
			maze[j][i].x = i;
			maze[j][i].y = row - j - 1;
		}
	}
}

// this function reads a txt file and generates maze aftwerwards to find a path
void fillMatrix(vector<vector<cell>>& new_maze, ifstream& input_file) {
	// get first line which includes dimensions
	string line;
	getline(input_file, line);
	stringstream dimensions(line);
	int y, x, dim_y, dim_x;
	dimensions >> dim_y >> dim_x;
	char temp;
	bool l, r, u, d;

	// read each line one by one and divide accordingly to create copy of the maze
	while (getline(input_file, line)) {
		stringstream lines(line);

		lines >> temp >> temp >> x >> temp >> temp >> y >> temp >> temp >> l >> temp >> temp >> r >> temp >> temp >> u >> temp >> temp >> d;

		new_maze[y][x].l = l;
		new_maze[y][x].r = r;
		new_maze[y][x].u = u;
		new_maze[y][x].d = d;
		new_maze[y][x].x = x;
		new_maze[y][x].y = y;
	}
}

// this function finds possible ways fro a cell to another to find a path between two cell
void findPossibleWays(vector<vector<cell>>& new_maze, cell* current_node, vector<cell*>& possible_ways) {

	int x = 0, y = 0;
	int size = new_maze.size();

	if (current_node) { //prevent errors when current_node is nullptr
		y = current_node->y;
		x = current_node->x;

		//check neighbour cell if there isn't any wall between them check 
		//whether cell is visited or not and add to possible_ways vector their address accordingly
		if (!current_node->l) {
			if (!new_maze[y][x - 1].visited) {
				possible_ways.push_back(&new_maze[y][x - 1]);
			}
		}
		if (!current_node->r) {
			if (!new_maze[y][x + 1].visited) {
				possible_ways.push_back(&new_maze[y][x + 1]);
			}
		}
		if (!current_node->u) {
			if (!new_maze[y + 1][x].visited) {
				possible_ways.push_back(&new_maze[y + 1][x]);
			}
		}
		if (!current_node->d) {
			if (!new_maze[y - 1][x].visited) {
				possible_ways.push_back(&new_maze[y - 1][x]);
			}
		}
	}
}


// this function finds path between to cell in a given maze
void pathFinder(vector<vector<cell>>& new_maze, Stack<cell>& maze_stack, const int& e_x, const int& e_y, const int& o_x, const int& o_y) {
	cell* current_node = nullptr;
	cell* next_node = nullptr;
	int size = new_maze.size(), idx;

	// initialize stack with the starting point
	current_node = &new_maze[e_y][e_x];
	maze_stack.push(current_node);
	current_node->visited = true;

	vector<cell*> possible_ways;

	//find a path from starting point to end point
	while (!(current_node->x == o_x && current_node->y == o_y)) {
		findPossibleWays(new_maze, current_node, possible_ways);

		if (possible_ways.size() != 0) {
			idx = getRandomNumber(0, possible_ways.size() - 1);
			next_node = possible_ways[idx];
			next_node->visited = true;
			maze_stack.push(next_node);
			possible_ways.clear(); //clear the possible_ways vector because we use same vector and if vector is not cleared it might have previous cells' possible ways too
		}
		else {
			maze_stack.pop();
		}

		current_node = maze_stack.getTop();

	}
}



int main() {
	// take required input from the user (assume that input formats are correct)
	int M, N, K;
	cout << "Enter the number of mazes: "; cin >> K;
	cout << "Enter the number of rows and columns (M and N): "; cin >> M >> N;

	ofstream output_file; ifstream input_file; //generate input and output files
	string filename;

	int i = 1;

	//loop to generate the desired amount of mazes
	while (i <= K) {
		filename = "maze_" + to_string(i) + ".txt";
		output_file.open(filename);

		Stack<cell> maze_stack;
		int size = M * N;
		// create matrix 
		vector<vector<cell>> maze(M, vector<cell>(N, cell(0, 0)));
		fillEmptyMatrix(maze);

		//generate the maze
		generateMaze_(maze, maze_stack, size);

		vector2file(output_file, maze);
		i++;
		output_file.close();
	}

	cout << "All mazes are generated." << endl << endl;

	// take maze number from user to find a path
	string maze_number;
	cout << "Enter a maze ID between 1 to " << K << " inclusive to find a path : "; cin >> maze_number;

	// open the file to copy maze from file to vector
	filename = "maze_" + maze_number + ".txt";
	input_file.open(filename.c_str());
	

	// create vector and fill it from file
	vector<vector<cell>> new_maze(M, vector<cell>(N, cell(0, 0)));
	fillMatrix(new_maze, input_file);

	input_file.seekg(0);
	input_file.close();

	// take coordinates of end and starting points from the user
	int e_x, e_y, o_x, o_y;

	cout << "Enter x and y coordinates of the entry points (x,y) or (column,row): "; cin >> e_x >> e_y;
	cout << "Enter x and y coordinates of the exit points (x,y) or (column,row): "; cin >> o_x >> o_y;

	// initialize cell and call pathfinder function
	Stack<cell> maze_stack;
	pathFinder(new_maze, maze_stack, e_x, e_y, o_x, o_y);
	string path_file_name = "maze_" + maze_number + "_path_" + to_string(e_x) + "_" +
		to_string(e_y) + "_" + to_string(o_x) + "_" + to_string(o_y) + ".txt";

	// create output file and print the path from stack to file
	output_file.open(path_file_name.c_str());
	maze_stack.printStack(output_file);


	return 0;
}
