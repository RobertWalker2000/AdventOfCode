#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "BingoBoard.h"

int main()
{
	std::fstream inputFile;
	inputFile.open("Task 7 Input.txt");

	if (!inputFile)
	{
		std::cout << "Couldn't open the input file" << std::endl;
		return(1);
	}

	//Skip the first line so we can populate the boards first
	inputFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	/*std::string calledNumbers;
	inputFile >> calledNumbers;*/

	//Create and populate boards using the input file
	std::vector<BingoBoard> boards;
	while (!inputFile.eof())
	{
		BingoBoard tempBoard;
		tempBoard.PopulateBoard(&inputFile);
		boards.push_back(tempBoard);
	}

	//Reset the flags and return to the start of the file
	inputFile.clear();
	inputFile.seekg(0);

	int winningBoards = 0;	//Track how many boards have won
	int lastBoard = -1;		//Track which board won last
	while (true)
	{
		char peekChar;
		peekChar = inputFile.peek();
		if (peekChar == '\n')
		{
			std::cout << "Finished calling numbers" << std::endl;
			break;
		}

		int calledNumber;
		char calledChar[3];
		inputFile.getline(calledChar, 256, ',');
		calledNumber = atoi(calledChar);
		
		//Loop through all boards, marking off the called number, and checking for a winning board
		for (int i = 0; i < boards.size(); i++)
		{
			boards[i].MarkNumber(calledNumber);

			if (boards[i].AssignWinOrder(winningBoards))
			{
				winningBoards++;
				lastBoard = i;
			}
		}

		//Check if the last board has won yet, and if so exit the loop
		if (winningBoards == boards.size())
		{
			std::cout << "All boards have won!" << std::endl;
			break;
		}
	}

	inputFile.close();

	//If we reached here, we have a winning board
	std::cout << "Last winning board score: " << boards[lastBoard].GetScore() << std::endl;

	return(0);
}