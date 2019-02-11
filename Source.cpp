///// SUDOKU SOLVER /////
/*
	CREATED:		SEPTEMBER 23 2017
	LAST MODIFIED:	SEPTEMBER 24 2017

	WRITTEN BY TWALFORD.

	INCLUDES TWO SOLVING METHODS:
	 1 - INTUITIVE (SMART)
		 USES THE "NAKED SINGLE" STRATEGY.
	 2 - BRUTE-FORCE (SIMPLE)
		 A BREADTH FIRST SEARCH.

	THE FOLLOWING STRATEGIES COULD BE IMPLEMENTED TO THE 
	INTUITIVE SOLVING METHOD:
	 - "HIDDEN SINGLE"
	 - "NAKED PAIR"
	 - "HIDDEN PAIR"
	 - "POINTING PAIR"
	 - "CLAIMING PAIR"
	 - "NAKED TRIPLE"
	 - "NAKED QUAD"
	 - "X-WING"

*/

#include <string>
#include <iostream>
#include <list>
#include <math.h>
#include <ctime>

using namespace std;

class Cell {
public:
	int value; // The value of the cell.
	bool placeholder[9] = {false, false, false, false, false, false, false, false, false}; // PlaceHolder for possible values.
	list<int> notTried;

	void InitNotTried() {
		notTried.push_back(1);
		notTried.push_back(2);
		notTried.push_back(3);
		notTried.push_back(4);
		notTried.push_back(5);
		notTried.push_back(6);
		notTried.push_back(7);
		notTried.push_back(8);
		notTried.push_back(9);
	}
};

struct Coord {
	int x;
	int y;
};

bool CheckComplete(Cell g[9][9]);
bool AllocateSingles(Cell g[9][9]);
void SmartSolve(Cell g[9][9]);
void Fillgrid(Cell g[9][9]);
void PrintSudoku(Cell g[9][9]);
void UpdatePlaceholders(Cell g[9][9]);
void SimpleSolve(Cell g[9][9]);

void main() {

	Cell grid[9][9];
	Fillgrid(grid);


	int choice = 0;

	cout << " === MENU ===\n";
	cout << " 0 - Quit\n";
	cout << " 1 - SmartSolve\n";
	cout << " 2 - SimpleSolve\n";
	cin >> choice;

	int start_s = 0;
	int stop_s = 0;

	switch (choice)
	{
	case 1:
		cout << "Your sudoku should look like this: \n";
		PrintSudoku(grid);

		start_s = clock();

		cout << "solving (smart)...\n";
		SmartSolve(grid);
		cout << "finished solving.\n";

		stop_s = clock();
		cout << "time: " << (stop_s - start_s) / double(CLOCKS_PER_SEC) * 1000 << endl;

		cout << "here is my solution!\n";
		PrintSudoku(grid);
		break;
	case 2: 
		cout << "Your sudoku should look like this: \n";
		PrintSudoku(grid);

		start_s = clock();
	
		cout << "solving (simple)...\n";
		SimpleSolve(grid);
		cout << "finished solving.\n";

		stop_s = clock();
		cout << "time: " << (stop_s - start_s) / double(CLOCKS_PER_SEC) * 1000 << endl;

		cout << "here is my solution!\n";
		PrintSudoku(grid);
		break;
	default:
		break;
	}


	/*
	cout << "Enter numbers from top left to bottom right. use 0 for empty cells.";

	int input = -1;
	for (int y = 0; y < 9; y++) {
		for (int x = 0; x < 9; x++) { 
			cout << "\n" << x << "," << y << " > ";
			cin >> input;
			grid[x][y].value = input;
		}
	}
	*/

	system("pause");

	return;
}

// Solve the sudoku by brute force with a BFS.
// Breadth-First-Search
void SimpleSolve(Cell g[9][9]) {

	int printCount = 0;

	list<Coord> emptyCells;
	list<Coord> usedCells;

	//Add empty cells to the list
	for (int y = 0; y < 9; y++) {
		for (int x = 0; x < 9; x++) {
			//Check if the cell is empty
			if (g[x][y].value == 0) {
				g[x][y].InitNotTried();
				Coord c;
				c.x = x;
				c.y = y;
				emptyCells.push_back(c);
			}
		}
	}

	while(emptyCells.size() != 0)
	{
		Coord c;
		c.x = emptyCells.front().x;
		c.y = emptyCells.front().y;

		bool viable = true;
		bool impossible = false;
		int tryingValue = -1;
			
		if (g[c.x][c.y].value != 9) {
			tryingValue = g[c.x][c.y].notTried.front();
			g[c.x][c.y].notTried.pop_front();
		}
		else {
			viable = false;
		}


		// Check if that value is viable.
		// Check horizontal
		for (int i = 0; i < 9; i++) {
			if (g[i][c.y].value == tryingValue) {
				viable = false;
				break;
			}
		}
		// Check vertical
		if (viable == true) {
			for (int j = 0; j < 9; j++) {
				if (g[c.x][j].value == tryingValue) {
					viable = false;
					break;
				}
			}
		}
		// Check 3x3
		if (viable == true) {
			// Calculate which bigCell the cell is in
			int bigX;
			int bigY;
			switch (c.x) {
			case 0:
			case 1:
			case 2:
				bigX = 0;
				break;
			case 3:
			case 4:
			case 5:
				bigX = 1;
				break;
			case 6:
			case 7:
			case 8:
				bigX = 2;
				break;
			}
			switch (c.y) {
			case 0:
			case 1:
			case 2:
				bigY = 0;
				break;
			case 3:
			case 4:
			case 5:
				bigY = 1;
				break;
			case 6:
			case 7:
			case 8:
				bigY = 2;
				break;
			}
			// Check each of the cells in the BigCell
			for (int j = bigY * 3; j < bigY * 3 + 3; j++) {
				for (int i = bigX * 3; i < bigX * 3 + 3; i++) {
					if (g[i][j].value == tryingValue) {
						viable = false;
						break;
					}
				}
			}
		}

		// Apply value if viable
		if (viable == true) {
			g[c.x][c.y].value = tryingValue;
			emptyCells.pop_front();
			usedCells.push_front(c);

			// Display the current sudoku every one hundredth 
			// iterations of this block
			//if (printCount % 100 == 0) {
				//system("cls");
				//cout << endl;
				//PrintSudoku(g);
			//}
			printCount++;

		}
		else if (g[c.x][c.y].notTried.size() == 0) {
			// Couldn't find a viable value
			// Reset the cell and go back to the previous cell
			// by putting it at the start of the empty cell list. 
			g[c.x][c.y].value = 0;
			g[c.x][c.y].InitNotTried();

			if (usedCells.size() == 0) {
				cout << "ERROR: usedCells list empty!\n";
				break;
			}
			emptyCells.push_front(usedCells.front());
			usedCells.pop_front();
		}

	}




	//print the list of empty cells
	/*
	for each (Coord c in emptyCells)
	{
		cout << c.x << "," << c.y << endl;
	}
	*/

				//g[0][0].value = g[0][0].notTried.back();
				//g[0][0].notTried.pop_back();
}
// Returns a boolean for successfulness
// True if one or more values has been added to the grid.
// False if not.
// This function will check each cell for a lone placeholder
// and subsequently update a cell's value if it finds any.
// Parameter is Cell[][].
bool AllocateSingles(Cell g[9][9]) {
	bool success = false;
	for (int y = 0; y < 9; y++) {
		for (int x = 0; x < 9; x++) {
			int pCount = 0;
			int pValue = 0;
			for (int p = 0; p < 9; p++) {
				if (g[x][y].placeholder[p] == true) {
					pCount++;
					pValue = p;
				}
			}
			if (pCount == 1) {
				g[x][y].value = pValue + 1;
				success = true;
			}
		}
	}
	return success;
}
// Prints the Cell array to the screen.
// Parameter is Cell[][].
void PrintSudoku(Cell g[9][9]) {
	for (int y = 0; y < 9; y++) {
		if (y % 3 == 0) {
		cout << "  - - -   - - -   - - -  \n";
		}
		for (int x = 0; x < 9; x++) {
			if (x % 3 == 0) {
				cout << "| ";
			} 

			if (g[x][y].value == 0) {
				cout << "  ";
			}
			else {
				cout << g[x][y].value << " ";
			}
		}
		cout << "|\n";
	}
	cout << "  - - -   - - -   - - -  \n";
	
}
// Updates the placeholder values in every cell
// Only checks against Cell Values in the same Row, Column,
// and 3x3 square;
// Parameter is Cell[][].
void UpdatePlaceholders(Cell g[9][9]) {
	for (int n = 1; n < 10; n++) {
		for (int y = 0; y < 9; y++) {
			for (int x = 0; x < 9; x++) {
				//Check if the cell is empty
				if (g[x][y].value == 0) {
					//Check horizontal for n
					g[x][y].placeholder[n - 1] = true;
					for (int i = 0; i < 9; i++) {
						if (g[i][y].value == n) {
							g[x][y].placeholder[n - 1] = false;
							break;
						}
					}
					//Check vertical for n only if placeholder is still true
					if (g[x][y].placeholder[n - 1] == true) {
						for (int j = 0; j < 9; j++) {
							if (g[x][j].value == n) {
								g[x][y].placeholder[n - 1] = false;
								break;
							}
						}
					}
					//Check current 3x3 for n only if placeholder is still true
					if (g[x][y].placeholder[n - 1] == true) {

						//Calculate which BigCell the cell is in
						int bigX;
						int bigY;
						switch (x) {
						case 0:
						case 1:
						case 2:
							bigX = 0;
							break;
						case 3:
						case 4:
						case 5:
							bigX = 1;
							break;
						case 6:
						case 7:
						case 8:
							bigX = 2;
							break;
						}
						switch (y) {
						case 0:
						case 1:
						case 2:
							bigY = 0;
							break;
						case 3:
						case 4:
						case 5:
							bigY = 1;
							break;
						case 6:
						case 7:
						case 8:
							bigY = 2;
							break;
						}

						//Check each of the cells in the BigCell
						for (int j = bigY * 3; j < bigY * 3 + 3; j++) {
							for (int i = bigX * 3; i < bigX * 3 + 3; i++) {
								if (g[i][j].value == n) {
									g[x][y].placeholder[n - 1] = false;
									break;
								}
							}
						}
					}


				}
			}
		}
	}
}
// Initialises the values in each cell.
// Blank cells are represented by 0.
// Manually edit this function to change the sudoku.
// Parameter is Cell[][].
void Fillgrid(Cell g[9][9]) {
	g[0][0].value = 8;
	g[1][0].value = 0;
	g[2][0].value = 0;
	g[3][0].value = 0;
	g[4][0].value = 3;
	g[5][0].value = 0;
	g[6][0].value = 0;
	g[7][0].value = 0;
	g[8][0].value = 7;

	g[0][1].value = 0;
	g[1][1].value = 0;
	g[2][1].value = 0;
	g[3][1].value = 9;
	g[4][1].value = 4;
	g[5][1].value = 0;
	g[6][1].value = 3;
	g[7][1].value = 0;
	g[8][1].value = 0;

	g[0][2].value = 0;
	g[1][2].value = 0;
	g[2][2].value = 0;
	g[3][2].value = 0;
	g[4][2].value = 0;
	g[5][2].value = 0;
	g[6][2].value = 0;
	g[7][2].value = 0;
	g[8][2].value = 0;

	g[0][3].value = 7;
	g[1][3].value = 0;
	g[2][3].value = 4;
	g[3][3].value = 0;
	g[4][3].value = 8;
	g[5][3].value = 5;
	g[6][3].value = 0;
	g[7][3].value = 0;
	g[8][3].value = 0;

	g[0][4].value = 0;
	g[1][4].value = 0;
	g[2][4].value = 6;
	g[3][4].value = 0;
	g[4][4].value = 9;
	g[5][4].value = 4;
	g[6][4].value = 0;
	g[7][4].value = 1;
	g[8][4].value = 0;

	g[0][5].value = 0;
	g[1][5].value = 0;
	g[2][5].value = 0;
	g[3][5].value = 0;
	g[4][5].value = 7;
	g[5][5].value = 0;
	g[6][5].value = 0;
	g[7][5].value = 5;
	g[8][5].value = 8;

	g[0][6].value = 2;
	g[1][6].value = 7;
	g[2][6].value = 0;
	g[3][6].value = 0;
	g[4][6].value = 0;
	g[5][6].value = 0;
	g[6][6].value = 0;
	g[7][6].value = 6;
	g[8][6].value = 4;

	g[0][7].value = 6;
	g[1][7].value = 0;
	g[2][7].value = 0;
	g[3][7].value = 0;
	g[4][7].value = 0;
	g[5][7].value = 0;
	g[6][7].value = 8;
	g[7][7].value = 0;
	g[8][7].value = 5;

	g[0][8].value = 0;
	g[1][8].value = 0;
	g[2][8].value = 0;
	g[3][8].value = 0;
	g[4][8].value = 0;
	g[5][8].value = 6;
	g[6][8].value = 0;
	g[7][8].value = 9;
	g[8][8].value = 3;
}
// Attempts to solve a given sudoku.
// Parameter is Cell[][].
void SmartSolve(Cell g[9][9]) {

	int loopCounter = 0;
	while (!CheckComplete(g)){
		// DO ALL THE SOVLING HERE //

		//STEP 1
		// input possible placeholders
		UpdatePlaceholders(g);

		//STEP 2
		// input values on single placeholders
		if (!AllocateSingles(g)) {
			cout << "Could not find any single placeholders.\n";
			break;
			//STEP 3
			// 
		}

		//Output iteration status
		string strCompleteStatus = CheckComplete(g) ? " Complete\n" : " Not complete\n";
		cout << loopCounter++ << strCompleteStatus;
		if (loopCounter > 20) 
			break;
	}

	//Output placeholders
	/*for (int x = 0; x < 9; x++) {
		cout << "\n Placeholders in " << x << ",0: \n";
		for (int i = 0; i < 9; i++) {
			if (g[x][0].placeholder[i] == true)
				cout << i + 1 << endl;
		}
	}*/

	return;
}
// Returns a boolean for completeness of the sudoku.
// True if no cells contain 0 (emtpy).
// False if not
// Parameter is Cell[][].
bool CheckComplete(Cell g[9][9])
{
	for (int y = 0; y < 9; y++) {
		for (int x = 0; x < 9; x++) {
			if (g[x][y].value == 0) {
				return false;
			}
		}
	}
	return true;
}

	/* BLANK GRID
	g[0][0].value = 0;
	g[1][0].value = 0;
	g[2][0].value = 0;
	g[3][0].value = 0;
	g[4][0].value = 0;
	g[5][0].value = 0;
	g[6][0].value = 0;
	g[7][0].value = 0;
	g[8][0].value = 0;

	g[0][1].value = 0;
	g[1][1].value = 0;
	g[2][1].value = 0;
	g[3][1].value = 0;
	g[4][1].value = 0;
	g[5][1].value = 0;
	g[6][1].value = 0;
	g[7][1].value = 0;
	g[8][1].value = 0;

	g[0][2].value = 0;
	g[1][2].value = 0;
	g[2][2].value = 0;
	g[3][2].value = 0;
	g[4][2].value = 0;
	g[5][2].value = 0;
	g[6][2].value = 0;
	g[7][2].value = 0;
	g[8][2].value = 0;

	g[0][3].value = 0;
	g[1][3].value = 0;
	g[2][3].value = 0;
	g[3][3].value = 0;
	g[4][3].value = 0;
	g[5][3].value = 0;
	g[6][3].value = 0;
	g[7][3].value = 0;
	g[8][3].value = 0;

	g[0][4].value = 0;
	g[1][4].value = 0;
	g[2][4].value = 0;
	g[3][4].value = 0;
	g[4][4].value = 0;
	g[5][4].value = 0;
	g[6][4].value = 0;
	g[7][4].value = 0;
	g[8][4].value = 0;

	g[0][5].value = 0;
	g[1][5].value = 0;
	g[2][5].value = 0;
	g[3][5].value = 0;
	g[4][5].value = 0;
	g[5][5].value = 0;
	g[6][5].value = 0;
	g[7][5].value = 0;
	g[8][5].value = 0;

	g[0][6].value = 0;
	g[1][6].value = 0;
	g[2][6].value = 0;
	g[3][6].value = 0;
	g[4][6].value = 0;
	g[5][6].value = 0;
	g[6][6].value = 0;
	g[7][6].value = 0;
	g[8][6].value = 0;

	g[0][7].value = 0;
	g[1][7].value = 0;
	g[2][7].value = 0;
	g[3][7].value = 0;
	g[4][7].value = 0;
	g[5][7].value = 0;
	g[6][7].value = 0;
	g[7][7].value = 0;
	g[8][7].value = 0;

	g[0][8].value = 0;
	g[1][8].value = 0;
	g[2][8].value = 0;
	g[3][8].value = 0;
	g[4][8].value = 0;
	g[5][8].value = 0;
	g[6][8].value = 0;
	g[7][8].value = 0;
	g[8][8].value = 0;
	*/