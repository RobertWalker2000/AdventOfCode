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

	int winningBoard = -1;	//Track which board is the winner
	while (true)
	{
		char peekChar;
		peekChar = inputFile.peek();
		if (peekChar == '\n')
		{
			std::cout << "Error: Finished calling numbers but no board won" << std::endl;
			return(2);
		}

		int calledNumber;
		char calledChar[3];
		inputFile.getline(calledChar, 256, ',');
		calledNumber = atoi(calledChar);
		std::cout << calledNumber << std::endl;
		
		//Loop through all boards, marking off the called number, and checking for a winning board
		for (int i = 0; i < boards.size(); i++)
		{
			boards[i].MarkNumber(calledNumber);

			if (boards[i].HasWon())
			{
				//Track the winning board. If there are multiple, throw an error
				if (winningBoard == -1)
				{
					winningBoard = i;
				}
				else
				{
					std::cout << "Error: multiple winning boards" << std::endl;
					return(3);
				}
			}

		}

		//If we reach here, we have a winning board, no need to call another number
		if (winningBoard != -1)
			break;
	}

	inputFile.close();

	//If we reached here, we have a winning board
	std::cout << "Winning score: " << boards[winningBoard].GetScore() << std::endl;

	return(0);
}