#include <iostream>
#include <fstream>
#include <set>

struct Coords
{
	int x = -1;
	int y = -1;

	bool hasOverlap = false;

	bool operator==(Coords other)
	{
		return (x == other.x && y == other.y);
	}

};

bool operator<(const Coords &lhs, const Coords &rhs)
{
	if (lhs.x == rhs.x)
	{
		if (lhs.y < rhs.y)
			return true;
		else
			return false;
	}
	else if (lhs.x < rhs.x)
		return true;
	else
		return false;
}

void ParseLine(std::fstream* input, int* x1, int* x2, int* y1, int* y2);
void LogCoord(int x, int y, std::set<Coords>* coords, int* counter);

int main()
{
	std::fstream inputFile;
	inputFile.open("Task 9 Input.txt");
	//inputFile.open("Test Input.txt");

	if (!inputFile)
	{
		std::cout << "Unable to open file" << std::endl;
		return 1;
	}

	std::set<Coords> coordsSet;

	int x1 = 0;
	int y1 = 0;
	int x2 = 0;
	int y2 = 0;
	int overlapCounter = 0;

	while (!inputFile.eof())
	{
		ParseLine(&inputFile, &x1, &x2, &y1, &y2);
		
		int x = x1;
		int y = y1;

		if (x1 == x2)
		{
			if (y1 < y2)
			{
				while (y <= y2)
				{
					LogCoord(x, y, &coordsSet, &overlapCounter);
					y++;
				}
			}
			else
			{
				while (y >= y2)
				{
					LogCoord(x, y, &coordsSet, &overlapCounter);
					y--;
				}
			}
		}
		else if (y1 == y2)
		{
			if (x1 < x2)
			{
				while (x <= x2)
				{
					LogCoord(x, y, &coordsSet, &overlapCounter);
					x++;
				}
			}
			else
			{
				while (x >= x2)
				{
					LogCoord(x, y, &coordsSet, &overlapCounter);
					x--;
				}
			}
		}
	}

	std::cout << "Positions with 2 or more vents: " << overlapCounter << std::endl;
	return 0;
}

void ParseLine(std::fstream* file, int* x1, int* x2, int* y1, int* y2)
{
	char inputChar[4];

	file->getline(inputChar, 256, ',');
	*x1 = atoi(inputChar);

	file->getline(inputChar, 256, ' ');
	*y1 = atoi(inputChar);

	inputChar[0] = file->get();
	inputChar[1] = file->get();
	inputChar[2] = file->get();

	file->getline(inputChar, 256, ',');
	*x2 = atoi(inputChar);

	file->getline(inputChar, 256, '\n');
	*y2 = atoi(inputChar);
}

void LogCoord(int x, int y, std::set<Coords>* coords, int* counter)
{
	//Put the coordinates into a coord struct
	Coords co;
	co.x = x;
	co.y = y;
	co.hasOverlap = false;

	//Try inserting the coordinates into the set
	auto result = coords->insert(co);

	//If false, the element was already in the set
	//Update the overlap value if it has not been overlapped yet
	if (!result.second && !result.first->hasOverlap)
	{
		Coords replacement = *result.first;
		replacement.hasOverlap = true;
		coords->erase(result.first);
		coords->insert(replacement);
		*counter = *counter + 1;
	}
}