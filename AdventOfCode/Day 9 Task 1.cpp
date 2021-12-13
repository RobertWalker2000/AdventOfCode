#include <iostream>
#include <fstream>

int main()
{
	std::fstream input;
	input.open("Day 9 Input.txt");
	//input.open("Test Input.txt");
	if (!input)
	{
		std::cout << "ERROR: Couldn't open file" << std::endl;
		return 1;
	}

	const int Rows = 100;
	const int Columns = 100;
	int grid[Columns][Rows];
	char numChar[2] = "0";

	int currentCol = 0;
	int currentRow = 0;

	//Populate the array
	while (currentCol < Columns && currentRow < Rows)
	{
		grid[currentCol][currentRow] = (input.get() - 48);	//Subtracting 48 gives the actual value of the number, rather than its ASCII code

		if (currentRow == 99 && currentCol == 99)
			int test = 6;

		if (input.peek() == '\n')
		{
			input.get();
			currentCol = 0;
			currentRow++;
		}
		else
		{
			currentCol++;
		}
	}

	//Check for low points---------------------------------
	int riskLevels = 0;
	int center = 0, up = 0, left = 0, down = 0, right = 0;

	for (int i = 0; i < Columns; i++)
	{
		for (int j = 0; j < Rows; j++)
		{
			center = up = left = down = right = 10; //Initialise all the positions to a height of 10. This means if we check against them they won't prevent a low point
			center = grid[i][j];

			//Left
			int x = i - 1;
			int y = j;
			if (x >= 0 && x < Columns && y >= 0 && y < Rows)	//Check our indices are in bounds
				left = grid[x][y];

			//Right
			x = i + 1;
			y = j;
			if (x >= 0 && x < Columns && y >= 0 && y < Rows)	//Check our indices are in bounds
				right = grid[x][y];

			//Up
			x = i;
			y = j - 1;
			if (x >= 0 && x < Columns && y >= 0 && y < Rows)	//Check our indices are in bounds
				up = grid[x][y];

			//Down
			x = i;
			y = j + 1;
			if (x >= 0 && x < Columns && y >= 0 && y < Rows)	//Check our indices are in bounds
				down = grid[x][y];

			//Check for a low point. If we find one, add its danger score to our total
			if (center < left && center < right && center < up && center < down)
				riskLevels += (center + 1);
		}
	}

	std::cout << "Total sum of all risk levels in low points: " << riskLevels << std::endl;
	return 0;
}