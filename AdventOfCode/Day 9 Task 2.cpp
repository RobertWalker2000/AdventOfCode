#include <iostream>
#include <fstream>

static const int Columns = 100;
static const int Rows = 100;

void FindHigherSurroundings(int heights[Columns][Rows], bool basinPart[Columns][Rows], int x, int y);
int CalculateBasinSize(bool basinParts[Columns][Rows]);
void CompareBasinSizes(int sizes[3], int newSize);

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
	int basinSizes[3] = { 0 };
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

			//If this is a low point, calculate the area of its basin
			if (center < left && center < right && center < up && center < down)
			{
				bool basinParts[Columns][Rows] = { false };
				FindHigherSurroundings(grid, basinParts, i, j);
				int basinSize = CalculateBasinSize(basinParts);
				CompareBasinSizes(basinSizes, basinSize);
			}
		}
	}

	int total = basinSizes[0];
	total *= basinSizes[1];
	total *= basinSizes[2];
	std::cout << "Product of the 3 largest basin sizes: " << total << std::endl;
	return 0;
}

void FindHigherSurroundings(int heights[Columns][Rows], bool basinPart[Columns][Rows], int column, int row)
{
	//Get and store the height of the point we are checking
	//If the height is 9, it is not part of any basin
	int currentHeight = heights[column][row];
	if (currentHeight >= 9)
		return;

	//When we check a point for new basin parts, we also add that point as part of the basin
	basinPart[column][row] = true;

	//Check each side (if its in range) and find if its higher. If so, its part of the basin
	//When a new part of the basin is found, it is checked to find if it has any adjacent high points
	int x, y;

	//Left
	x = column - 1;
	y = row;
	if (x >= 0 && x < Columns && y >= 0 && y < Rows)
	{
		int compareHeight = heights[x][y];
		if (compareHeight > currentHeight)
			FindHigherSurroundings(heights, basinPart, x, y);
	}

	//Right
	x = column + 1;
	y = row;
	if (x >= 0 && x < Columns && y >= 0 && y < Rows)
	{
		int compareHeight = heights[x][y];
		if (compareHeight > currentHeight)
			FindHigherSurroundings(heights, basinPart, x, y);
	}

	//Up
	x = column;
	y = row - 1;
	if (x >= 0 && x < Columns && y >= 0 && y < Rows)
	{
		int compareHeight = heights[x][y];
		if (compareHeight > currentHeight)
			FindHigherSurroundings(heights, basinPart, x, y);
	}

	//Down
	x = column;
	y = row + 1;
	if (x >= 0 && x < Columns && y >= 0 && y < Rows)
	{
		int compareHeight = heights[x][y];
		if (compareHeight > currentHeight)
			FindHigherSurroundings(heights, basinPart, x, y);
	}
}

int CalculateBasinSize(bool basinParts[Columns][Rows])
{
	int total = 0;
	for (int i = 0; i < Columns; i++)
	{
		for (int j = 0; j < Rows; j++)
		{
			if (basinParts[i][j])
				total++;
		}
	}

	return total;
}

void CompareBasinSizes(int sizes[3], int newSize)
{
	if (newSize > sizes[0])
	{
		if (newSize > sizes[1])
		{
			if (newSize > sizes[2])	//New size is largest
			{
				sizes[0] = sizes[1];
				sizes[1] = sizes[2];
				sizes[2] = newSize;
			}
			else	//New size is larger than size 1 but smaller than size 2
			{
				sizes[0] = sizes[1];
				sizes[1] = newSize;
			}
		}
		else	//New size is larger than size 0 but smaller than size 1
		{
			sizes[0] = newSize;
		}
	}
}