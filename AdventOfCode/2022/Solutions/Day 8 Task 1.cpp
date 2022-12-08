#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int CountVisibleTrees(std::vector<std::vector<int>>* trees);

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

	int visTrees = CountVisibleTrees(&trees);

	std::cout << "Number of visible trees: " << visTrees << std::endl;
	exit(0);
}

int CountVisibleTrees(std::vector<std::vector<int>>* trees)
{
	int rows = trees->size();
	int columns = trees->at(1).size();

	//Build an vector of bools to match the trees, which will be used to track which are visible
	std::vector<std::vector<bool>> isVisible;
	std::vector<bool> blankRow;
	for (int i = 0; i < columns; i++)
		blankRow.push_back(false);

	for (int i = 0; i < rows; i++)
		isVisible.push_back(blankRow);

	//Check columns (up and down)
	for (int col = 0; col < columns; col++)
	{
		int tallestTree = -1;
		//Check from top
		for (int row = 0; row < rows; row++)
		{
			//If the current tree is taller than anything we have checked so far, it is visible
			int height = trees->at(row).at(col);
			if (height > tallestTree)
			{
				isVisible[row][col] = true;
				tallestTree = height;
			}
		}

		tallestTree = -1;
		//Check from bottom
		for (int row = rows - 1; row >= 0; row--)
		{
			//If the current tree is taller than anything we have checked so far, it is visible
			int height = trees->at(row).at(col);
			if (height > tallestTree)
			{
				isVisible[row][col] = true;
				tallestTree = height;
			}
		}
	}

	//Check rows (left and right)
	for (int row = 0; row < rows; row++)
	{
		int tallestTree = -1;
		//Check from the left
		for (int col = 0; col < columns; col++)
		{
			//If the current tree is taller than anything we have checked so far, it is visible
			int height = trees->at(row).at(col);
			if (height > tallestTree)
			{
				isVisible[row][col] = true;
				tallestTree = height;
			}
		}

		tallestTree = -1;
		//Check from the right
		for (int col = columns - 1; col >= 0; col--)
		{
			//If the current tree is taller than anything we have checked so far, it is visible
			int height = trees->at(row).at(col);
			if (height > tallestTree)
			{
				isVisible[row][col] = true;
				tallestTree = height;
			}
		}
	}

	//Count the visible trees
	int result = 0;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			if (isVisible[i][j])
			{
				result++;
			}
		}
	}

	return result;
}
