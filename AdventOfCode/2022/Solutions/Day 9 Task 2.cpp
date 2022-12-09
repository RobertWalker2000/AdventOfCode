#include <iostream>
#include <fstream>
#include <string>
#include <set>

struct Coordinates
{
	Coordinates() { x = 0; y = 0; };
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
Coordinates FindNewKnotPosition(Coordinates head, Coordinates tail);

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

	Coordinates knots[10] = { Coordinates(0, 0) };
	Coordinates direction(0, 0);
	int steps = 0;
	std::set<Coordinates> tailPositions;

	//Make sure to record the starting position of the tail
	tailPositions.insert(knots[9]);
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
			//Move the head knot
			knots[0] = knots[0] + direction;

			//Check each knot in the chain against the knot ahead of it, moving if necessary
			for (int currentKnot = 1; currentKnot < 10; currentKnot++)
			{
				Coordinates leadKnot = knots[currentKnot - 1];
				Coordinates trailKnot = knots[currentKnot];
				knots[currentKnot] = FindNewKnotPosition(leadKnot, trailKnot);
			}

			//Track the tail's current position
			tailPositions.insert(knots[9]);
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

//Updates the tail knots position to follow the head knot, based on which axis it is further away on
Coordinates FindNewKnotPosition(const Coordinates head, const Coordinates tail)
{
	Coordinates newPos = tail;
	int horDist = abs(head.x - tail.x);
	int verDist = abs(head.y - tail.y);

	//Don't need to move if neither distance is greater than 1
	if (horDist <= 1 && verDist <= 1)
		return newPos;

	//Find which axis has the greater distance between knots, then move the tail to be in line with the head on the opposite axis
	if (horDist > verDist)	//Move horizontally behind the head knot
	{
		newPos.y = head.y;

		if (tail.x > head.x)
			newPos.x = head.x + 1;
		else if (tail.x < head.x)
			newPos.x = head.x - 1;
		else
			std::cout << "ERROR: UNEXPECTED X POSITIONS" << std::endl;
	}
	else if (verDist > horDist)	//Move vertically behind the head knot
	{
		newPos.x = head.x;

		if (tail.y > head.y)
			newPos.y = head.y + 1;
		else if (tail.y < head.y)
			newPos.y = head.y - 1;
		else
			std::cout << "ERROR: UNEXPECTED Y POSITIONS" << std::endl;
	}
	else	//Distances are equal, move diagonally behind the head knot
	{
		if (tail.x > head.x)
			newPos.x = head.x + 1;
		else if (tail.x < head.x)
			newPos.x = head.x - 1;
		else
			std::cout << "ERROR: UNEXPECTED X POSITIONS" << std::endl;

		if (tail.y > head.y)
			newPos.y = head.y + 1;
		else if (tail.y < head.y)
			newPos.y = head.y - 1;
		else
			std::cout << "ERROR: UNEXPECTED Y POSITIONS" << std::endl;
	}

	return newPos;
}