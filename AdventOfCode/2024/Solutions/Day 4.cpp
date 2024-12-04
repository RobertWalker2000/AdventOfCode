#include <iostream>
#include <fstream>
#include <string>
#include <vector>

enum Direction : int {Up = 0, Down, Left, Right, UpLeft, UpRight, DownLeft, DownRight};

int CountXMAS(std::vector<std::string>* grid);
int CountCrossMAS(std::vector<std::string>* grid);
bool CheckXMASFromPoint(std::vector<std::string>* grid, int x, int y, Direction dir);
bool CheckCrossFromPoint(std::vector<std::string>* grid, int x, int y);

int main()
{
	//Create an input stream and open the file
	const int day = 4;

	std::string fileName;
	fileName.append("2024/Inputs/D");
	fileName.append(std::to_string(day));
	fileName.append(".txt");
	std::ifstream input;
	input.open(fileName);

	//Check the input stream is valid
	if (!input)
	{
		std::cout << "Unable to open file" << std::endl;
		exit(1);
	}

	std::vector<std::string> grid;
	while (!input.eof())
	{
		char line[256];
		input.getline(line, 256);
		std::string strLine(line);

		if(!strLine.empty())
		grid.push_back(strLine);
	}

	//Check that all lines are of equal length, and also add padding on either side to create a border (easy edge cases)
	for (int i = 0; i < grid.size(); i++)
	{
		grid[i].insert(grid[i].begin(), '.');
		grid[i].append(".");
		if (grid[i].size() != grid[0].size())
		{
			std::cout << "ERROR: Line " << i << " was of an unexpected size" << std::endl;
			exit(2);
		}
	}

	//Also apply the border to the top and bottom of the grid
	std::string bufferLine;
	bufferLine.append(grid[0].size(), '.');
	grid.insert(grid.begin(), bufferLine);
	grid.push_back(bufferLine);

	//Count up the occurences of "XMAS", then count occurences of CorssMAS
	//Both methods are destructive, but counting XMAS does not alter the data required to count CrossMAS
	//Methods could easily be made non-destructive by passing copies of the grid rather than a reference, but this would increase memory usage and is not required for the given tasks
	int xmas = CountXMAS(&grid);
	int crossMas = CountCrossMAS(&grid);

	std::cout << "XMAS found: " << xmas << std::endl;
	std::cout << "CrossMAS found: " << crossMas << std::endl;
	exit(0);
}

int CountXMAS(std::vector<std::string>* grid)
{
	int result = 0;

	//Loop through all the rows
	for (int y = 0; y < grid->size(); y++)
	{
		while (true)	//Loop until we have checked all "X"s on the line
		{
			//Find the position of the first "X" in the row
			int x = grid->at(y).find("X");

			//If no "X", we're finished with the row
			if (x == std::string::npos)
				break;;

			//Count how many times this "X" spells out the word "XMAS"
			for (int i = 0; i < 8; i++)
			{
				Direction dir = static_cast<Direction>(i);
				if (CheckXMASFromPoint(grid, x, y, dir))
					result++;
			}

			//Remove this "X" from the grid, as we no longer need it
			(*grid)[y][x] = '.';
		}
	}

	return result;
}

int CountCrossMAS(std::vector<std::string>* grid)
{
	int result = 0;

	//Loop through all the rows
	for (int y = 0; y < grid->size(); y++)
	{
		while (true)	//Loop until we have checked all "A"s on the line
		{
			//Find the position of the first "X" in the row
			int x = grid->at(y).find("A");

			//If no "A", we're finished with the row
			if (x == std::string::npos)
				break;

			//Check if this A is at the centre of a CrossMAS
			if (CheckCrossFromPoint(grid, x, y))
				result++;

			//Remove this "A" from the grid, as we no longer need it
			(*grid)[y][x] = '.';
		}
	}

	return result;
}

bool CheckXMASFromPoint(std::vector<std::string>* grid, int x, int y, Direction dir)
{
	int moveX = 0;
	int moveY = 0;

	//Set the direction of travel
	switch (dir)
	{
	case Direction::Up:
		moveX = 0;
		moveY = -1;
		break;
	case Direction::Down:
		moveX = 0;
		moveY = 1;
		break;
	case Direction::Left:
		moveX = -1;
		moveY = 0;
		break;
	case Direction::Right:
		moveX = 1;
		moveY = 0;
		break;
	case Direction::UpLeft:
		moveX = -1;
		moveY = -1;
		break;
	case Direction::UpRight:
		moveX = 1;
		moveY = -1;
		break;
	case Direction::DownLeft:
		moveX = -1;
		moveY = 1;
		break;
	case Direction::DownRight:
		moveX = 1;
		moveY = 1;
		break;
	}

	std::string Xmas = "XMAS";
	for (int i = 0; i < 4; i++)
	{
		//Check that we have the expected character. If not, the word is invalid
		if ((*grid)[y][x] != Xmas[i])
			return false;

		//The character was valid, move to the next one before checking again
		x += moveX;
		y += moveY;
	}

	//All checked characters were the expected value, we found a "XMAS"
	return true;
}

bool CheckCrossFromPoint(std::vector<std::string>* grid, int x, int y)
{
	std::string line1 = "...";
	std::string line2 = "...";

	//Build the first string from top-left to bottom-right
	line1[0] = ((*grid)[y - 1][x - 1]);
	line1[1] = ((*grid)[y][x]);
	line1[2] = ((*grid)[y + 1][x + 1]);

	//Build the second line from bottom-left to top-right
	line2[0] = ((*grid)[y + 1][x - 1]);
	line2[1] = ((*grid)[y][x]);
	line2[2] = ((*grid)[y - 1][x + 1]);

	//Check that line 1 spells "MAS"
	if (line1 != "MAS" && line1 != "SAM")
		return false;

	//Check that line 2 spells MAS
	if (line2 != "MAS" && line2 != "SAM")
		return false;

	//Both lines spell MAs, this is a CrossMAS
	return true;
}