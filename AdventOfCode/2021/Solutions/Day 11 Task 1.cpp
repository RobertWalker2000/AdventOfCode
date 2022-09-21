#include <iostream>
#include <fstream>

#define COLUMNS 10
#define ROWS 10

void IncrementAllOctopus(int octopus[COLUMNS][ROWS]);
int CheckFlashes(int octopus[COLUMNS][ROWS]);
void Flash(int octopus[COLUMNS][ROWS], int x, int y);
void TryIncrement(int octopus[COLUMNS][ROWS], int x, int y);
void ResetFlashedOctopus(int octopus[COLUMNS][ROWS]);

int main()
{
	std::fstream input;
	input.open("Day 11 Input.txt");
	//input.open("Test Input.txt");
	if (!input)
	{
		std::cout << "ERROR: Couldn't open file" << std::endl;
		return 1;
	}

	int octopus[COLUMNS][ROWS];

	for (int i = 0; i < COLUMNS; i++)
	{
		for (int j = 0; j < ROWS; j++)
		{
			char inputChar[2];
			inputChar[0] = input.get();
			octopus[i][j] = atoi(inputChar);
		}
		//Skip the new line character
		input.get();
	}

	int syncFlash = 0;	//Track how many steps we have done in order to find when all octopus flash together
	while (true)
	{
		syncFlash++;
		IncrementAllOctopus(octopus);
		int numFlashes = CheckFlashes(octopus);
		ResetFlashedOctopus(octopus);

		if (numFlashes == (ROWS * COLUMNS))
			break;
	}

	std::cout << "All octopus flash on step " << syncFlash << std::endl;
	return 0;
}

void IncrementAllOctopus(int octopus[COLUMNS][ROWS])
{
	for (int i = 0; i < COLUMNS; i++)
	{
		for (int j = 0; j < ROWS; j++)
		{
			octopus[i][j]++;
		}
	}
}

int CheckFlashes(int octopus[COLUMNS][ROWS])
{
	int totalFlashes = 0;
	int numFlashes = 0;
	do
	{
		numFlashes = 0;	//Reset the number of flashes at the start of each loop
		for (int i = 0; i < COLUMNS; i++)
		{
			for (int j = 0; j < ROWS; j++)
			{
				if (octopus[i][j] > 9)
				{
					//If an octopus has a power level greater than 9, it flashes
					Flash(octopus, i, j);
					numFlashes++;
				}
			}
		}
		//Track the total number of flashes to return as an output
		totalFlashes += numFlashes;
	} while (numFlashes != 0);

	return totalFlashes;
}

void Flash(int octopus[COLUMNS][ROWS], int x, int y)
{
	//Starting top left, moving clockwise, increment all adjacent octopus
	TryIncrement(octopus, x - 1, y - 1);	//Top left
	TryIncrement(octopus, x , y - 1);		//Top middle
	TryIncrement(octopus, x + 1, y - 1);	//Top right
	TryIncrement(octopus, x + 1, y);		//Middle right
	TryIncrement(octopus, x + 1, y + 1);	//Bottom right
	TryIncrement(octopus, x, y + 1);		//Bottom middle
	TryIncrement(octopus, x - 1, y + 1);	//Bottom left
	TryIncrement(octopus, x - 1, y);		//Middle left

	//After flashing, set our value to the minimum int value to prevent us from flashing again
	octopus[x][y] = INT_MIN;
}

void TryIncrement(int octopus[COLUMNS][ROWS], int column, int row)
{
	//Check that all the indices are in bounds before incrementing the octopus
	if (column >= 0 && column < COLUMNS && row >= 0 && row < ROWS)
		octopus[column][row]++;
}

//Set all octopus with an energy below 0 to 0
void ResetFlashedOctopus(int octopus[COLUMNS][ROWS])
{
	for (int i = 0; i < COLUMNS; i++)
	{
		for (int j = 0; j < ROWS; j++)
		{
			if (octopus[i][j] < 0)
				octopus[i][j] = 0;
		}
	}
}