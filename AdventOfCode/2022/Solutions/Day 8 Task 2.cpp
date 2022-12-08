#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int FindLargestScenicScore(std::vector<std::vector<int>>* trees);

int main()
{
	//Create an input stream and open the file
	const int day = 8;
	const int task = 1;

	std::string fileName;
	fileName.append("2022/Inputs/D");
	fileName.append(std::to_string(day));
	fileName.append("T");
	fileName.append(std::to_string(task));
	fileName.append(".txt");
	std::ifstream input;
	input.open(fileName);

	//Check the input stream is valid
	if (!input)
	{
		std::cout << "Unable to open file" << std::endl;
		exit(1);
	}

	std::vector<std::vector<int>> trees;
	while (!input.eof())
	{
		std::vector<int> currentRow;
		char currentChar = 0;

		//Populate a row of trees by reading in from file
		do
		{
			currentChar = input.get();

			if (currentChar == '\n')
			{
				trees.push_back(currentRow);	//Once the row is complete, add it to the trees vector and start a new row
				break;
			}
			else if (currentChar >= '0' && currentChar <= '9')
			{
				int currentNum = currentChar - '0';	//Convert the char to its numerical equivalent
				currentRow.push_back(currentNum);
			}

		} while (currentChar >= '0' && currentChar <= '9');	//Ensures we stop looping when we have an invalid char, even if it is not a newLine
	}

	int scenicScore = FindLargestScenicScore(&trees);

	std::cout << "Number of visible trees: " << scenicScore << std::endl;
	exit(0);
}

int FindLargestScenicScore(std::vector<std::vector<int>>* trees)
{
	int rows = trees->size();
	int columns = trees->at(1).size();
	int highScore = 0;

	for (int row = 1; row < rows - 1; row++)
	{
		for (int col = 1; col < columns - 1; col++)
		{
			int height = trees->at(row).at(col);
			int totalScore = 1;
			int lineScore = 0;
			int x = col;
			int y = row;

			//Check out left
			x--;
			while (x >= 0)
			{
				lineScore++;
				if (trees->at(y).at(x) < height)	//If the tree is shorter than the starting tree, keep checking further out
				{
					x--;
					continue;
				}

				//If the tree blocks the view, stop checking this line
				break;
			}
			
			//Update the current score
			totalScore *= lineScore;

			//Reset appropriate variables
			x = col;
			lineScore = 0;

			//Check out right
			x++;
			while (x < columns)
			{
				lineScore++;
				if (trees->at(y).at(x) < height)	//If the tree is shorter than the starting tree, keep checking further out
				{
					x++;
					continue;
				}

				//If the tree blocks the view, stop checking this line
				break;
			}

			//Update the current score
			totalScore *= lineScore;

			//Reset appropriate variables
			x = col;
			lineScore = 0;

			//Check up
			y--;
			while (y >= 0)
			{
				lineScore++;
				if (trees->at(y).at(x) < height)	//If the tree is shorter than the starting tree, keep checking further out
				{
					y--;
					continue;
				}

				//If the tree blocks the view, stop checking this line
				break;
			}

			//Update the current score
			totalScore *= lineScore;

			//Reset appropriate variables
			y = row;
			lineScore = 0;

			//Check down
			y++;
			while (y < rows)
			{
				lineScore++;
				if (trees->at(y).at(x) < height)	//If the tree is shorter than the starting tree, keep checking further out
				{
					y++;
					continue;
				}

				//If the tree blocks the view, stop checking this line
				break;
			}

			//Update the current score
			totalScore *= lineScore;

			if (totalScore > highScore)
				highScore = totalScore;
		}
	}

	return highScore;
}
