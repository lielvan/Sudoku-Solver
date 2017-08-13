// Liel van der Hoeven - SudokuSolver Backtracking

#include "stdafx.h"
#include <stdlib.h>
#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <fstream>
using namespace std;

struct SudokuID
{
	int arr[9][9];
	int ID;
};
// Program Methods Declaration
bool checkRowRule(int sudoku[9][9], int n, int row);
bool checkColumnRule(int sudoku[9][9], int n, int col);
bool checkBoxRule(int sudoku[9][9], int n, int row, int col);
bool solve(int sudoku[9][9]);
bool findMissingNum(int sudoku[9][9], int &row, int &col);

bool checkRowRule(int sudoku[9][9], int n, int row) // Check if ROW already has desired number
{
	int tempRow;
	for (int y = 0; y < 9; y++)
	{
		tempRow = sudoku[row][y];
		if (tempRow == n)
			return true;
	}
	return false;
}

bool checkColumnRule(int sudoku[9][9], int n, int col) // Check if COLUMN already has desired number
{
	int tempCol;
	for (int x = 0; x < 9; x++)
	{
		tempCol = sudoku[x][col];
		if (tempCol == n)
			return true;
	}
	return false;
}

bool checkBoxRule(int sudoku[9][9], int n, int row, int col)
{
	// Forumla that makes sure the validation starts at the beginning of 
	//  the 3x3 matrix and ends at the end of it, no matter where you start in it
	int startRow = row - (row % 3);
	int startCol = col - (col % 3);
	int endRow = startRow + 3;
	int endCol = startCol + 3;
	int tempMatrix;

	for (int row = startRow; row < endRow; row++)
	{
		for (int col = startCol; col < endCol; col++)
		{
			tempMatrix = sudoku[row][col];
			if (tempMatrix == n)
				return true;
		}
	}
	return false;
}

bool findMissingNum(int sudoku[9][9], int &row, int &col) // Find missing numbers in given 3x3 matrix
{
	for (row = 0; row < 9; row++)
	{
		for (col = 0; col < 9; col++)
		{
			if (sudoku[row][col] == 0)
			{
				return true;
			}
		}
	}
	return false;
}

bool solve(int sudoku[9][9])
{
	int row, col;
	if (!findMissingNum(sudoku, row, col))
		return true;
	for (int possibleNum = 1; possibleNum < 10; possibleNum++)
	{
		// Making sure possibleNum passes all conditions
		if (!checkRowRule(sudoku, possibleNum, row) && !checkColumnRule(sudoku, possibleNum, col) && !checkBoxRule(sudoku, possibleNum, row, col))
		{
			/* Core of the backtracking algorithm for this sudoku solver
				First - use the for-loop to get all possible numbers
				Second - checks if that value is acceptable, if so, continute to next spot
					If later on, that value deems to be the wrong number for that spot, we go
					back a step and assign that spot back to 0
				Finally - if all values in all boxes, are correct, true should be sent back to the original
				recursion call and we know the puzzle was completed successfully
			*/
			sudoku[row][col] = possibleNum; // First step
			if (solve(sudoku)) // Second step
			{
				return true;
			}
			sudoku[row][col] = 0; // Second step part 2 (if value is in wrong spot)
		}
	}
	return false;

}

void print(int sudoku[9][9])
{
	for (int x = 0; x < 9; x++)
	{
		for (int y = 0; y < 9; y++)
		{
			cout << sudoku[x][y] << " ";
		}
		cout << endl;
	}
}

int main()
{
	int sudoku[9][9] = { 7,6,0,0,0,9,1,0,0,
		0,0,5,0,0,4,7,0,0,
		2,0,4,1,0,8,0,0,9,
		0,0,0,0,0,0,0,0,2,
		0,0,0,6,0,0,0,0,0,
		0,8,7,2,0,0,6,0,0,
		0,9,3,0,0,0,8,0,0,
		0,0,0,0,1,0,4,0,0,
		0,7,0,8,5,0,0,0,0 };

	vector<int> tempSudoku;
	list<SudokuID> sudokus;
	string num;
	int intNum;
	int count = 1;
	fstream file("Sudoku Puzzles.txt");
	while (file >> num)
	{
		SudokuID s;
		string out;
		for (int i = 0; i < num.size(); i++)
		{
			if (num[i] == '{' || num[i] == '}') // If current iterator is not on a sudoku number, break
			{
				break;
			}
			if (num[i] != ',' && num[i] != ' ')
			{
				// Convert char to int and push into vector
				out += num[i];
				intNum = num[i] - '0';
				tempSudoku.push_back(intNum);
			}
		}

		// Try to populate the array of sudoku
		if (num[0] != '{' && num[0] != '}')
		{
			for (int r = 0; r < 9; r++)
			{
				for (int c = 0; c < 9; c++)
				{
					if (!tempSudoku.empty() && num[c] != '{' &&  num[c] != '}')
					{
						// Adding values to sudoku 2D array and removing that value from temp
						s.arr[r][c] = tempSudoku.front();
						tempSudoku.erase(tempSudoku.begin());
					}
				}
			}
			// Keeping up with ID for each sudoku in file
			s.ID = count;
			count++;
			sudokus.push_back(s);
		}
	}

	cout << endl;

	// Solve and print all sudokus in list
	for (auto s : sudokus)
	{
		solve(s.arr);
		cout << "Sudoku ID - " << s.ID << endl;
		print(s.arr);
		cout << endl;
	}
	file.close();
	return 0;
}