#include <iostream>
#include <fstream>
#include <forward_list>

struct Coords
{
	int x = 0;
	int y = 0;

	bool operator==(Coords other)
	{
		if (x == other.x && y == other.y)
			return true;

		return false;
	}

	bool operator<(Coords other)
	{
		//Sort by x first
		if (x < other.x)
			return true;

//If x is equal, sort by y
if (x == other.x && y < other.y)
	return true;

return false;
	}
};

int main()
{
	std::fstream input;
	input.open("Day 13 Input.txt");
	//input.open("Test Input.txt");
	if (!input)
	{
		std::cout << "ERROR: Couldn't open file" << std::endl;
		return 1;
	}

	std::forward_list<Coords> points;

	while (true)
	{
		Coords newPoint;
		char inputChar[5];
		input.getline(inputChar, 256, ',');
		newPoint.x = atoi(inputChar);
		input.getline(inputChar, 256, '\n');
		newPoint.y = atoi(inputChar);

		points.push_front(newPoint);

		if (input.peek() == '\n')
			break;
	}

	//Get rid of any duplicate points
	points.sort();
	points.unique();

	while (!input.eof())
	{
		//Ignore the line up to the = sign
		input.ignore(256, '=');

		//Go back 2 characters so we can get the axis
		input.unget();
		input.unget();

		char axis = input.get();		//Read in the axis letter
		input.get();					//Skip the = sign

		//Read in the value of the axis and convert it to an int
		char valueChar[5];
		input.getline(valueChar, 256, '\n');
		int axisValue = atoi(valueChar);

		if (axis == 'x')
		{
			for (std::forward_list<Coords>::iterator currentPoint = points.begin(); currentPoint != points.end(); currentPoint++)
			{
				if (currentPoint->x > axisValue)
				{
					int difference = currentPoint->x - axisValue;
					currentPoint->x = axisValue - difference;
				}
			}
		}
		else if (axis == 'y')
		{
			for (std::forward_list<Coords>::iterator currentPoint = points.begin(); currentPoint != points.end(); currentPoint++)
			{
				if (currentPoint->y > axisValue)
				{
					int difference = currentPoint->y - axisValue;
					currentPoint->y = axisValue - difference;
				}
			}
		}

		points.sort();
		points.unique();
	}

	const int xMax = 40;
	const int yMax = 6;
	bool grid[xMax][yMax] = { false };	//Array size taken from last fold along each axis
	for (std::forward_list<Coords>::iterator currentPoint = points.begin(); currentPoint != points.end(); currentPoint++)
	{
		int x = currentPoint->x;
		int y = currentPoint->y;

		grid[x][y] = true;
	}

	for (int i = 0; i < yMax; i++)
	{
		for (int j = 0; j < xMax; j++)
		{
			if (grid[j][i])
				std::cout << "#";
			else
				std::cout << " ";
		}
		std::cout << std::endl;
	}

	int numPoints = std::distance(points.begin(), points.end());
	std::cout << std::endl << "Number of visible points: " << numPoints << std::endl;
	return 0;
}