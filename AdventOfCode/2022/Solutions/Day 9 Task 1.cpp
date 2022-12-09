#include <iostream>
#include <fstream>
#include <string>
#include <set>

struct Coordinates
{
	Coordinates(int inX, int inY) { x = inX; y = inY; }

	int x;
	int y;

	bool operator==(const Coordinates other) const { return (x == other.x && y == other.y); }
	bool operator >(const Coordinates other) const { return ((x == other.x) ? y > other.y : x > other.x); }
	bool operator <(const Coordinates other) const { return ((x == other.x) ? y < other.y : x < other.x); }
	Coordinates operator+(const Coordinates other) const { return Coordinates((x + other.x), y + other.y); }
	Coordinates operator-(const Coordinates other) const { return Coordinates((x - other.x), y - other.y); }
};

Coordinates GetDirectionVector(char letter);

int main()
{
	//Create an input stream and open the file
	const int day = 9;
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

	Coordinates head(0, 0);
	Coordinates tail(0, 0);
	Coordinates direction(0, 0);
	int steps = 0;
	std::set<Coordinates> tailPositions;

	//Make sure to record the starting position of the tail
	tailPositions.insert(tail);
	while (!input.eof())
	{
		direction = GetDirectionVector(input.get());	//Get the normalised direction vector
		if (direction == Coordinates(0, 0))	//Check we have a valid direction
			break;

		input.get();	//Skip the blank space

		char stepsChar[5];
		input.getline(stepsChar, 256);
		steps = atoi(stepsChar);	//Get the number of steps, converted from char to int

		for (int i = 0; i < steps; i++)
		{
			head = head + direction;

			//Check whether the tail has gone out of range of the head, and if so move it and track its new position
			if (abs(head.x - tail.x) > 1 || abs(head.y - tail.y) > 1)
			{
				tail = head - direction;
				tailPositions.insert(tail);
			}
		}
	}

	std::cout << "No. of unique positions visited by tail: " << tailPositions.size() << std::endl;

	exit(0);
}

Coordinates GetDirectionVector(char letter)
{
	Coordinates result(0, 0);

	if (letter == 'R')
		result.x = 1;
	else if (letter == 'L')
		result.x = -1;
	else if (letter == 'U')
		result.y = 1;
	else if (letter == 'D')
		result.y = -1;
	else
		std::cout << "ERROR: Invalid direction" << std::endl;

	return result;
}