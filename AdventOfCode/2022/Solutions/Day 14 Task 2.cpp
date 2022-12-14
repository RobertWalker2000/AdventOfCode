#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <algorithm>

struct Coords
{
	Coords() {};
	Coords(int inX, int inY) { x = inX; y = inY; }
	int x, y;

	bool operator<(Coords other) const { return((x == other.x) ? y < other.y : x < other.x); }
	bool operator==(Coords other) const { return (x == other.x && y == other.y); }
	Coords operator+(Coords other) const { return (Coords(x + other.x, y + other.y)); }
	Coords operator-(Coords other) const { return (Coords(x - other.x, y - other.y)); }
};

int FillCave(std::set<Coords> *blocks, const int lowPoint);

int main()
{
	//Create an input stream and open the file
	const int day = 14;
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

	std::set<Coords> blocks;
	int lowPoint = 0;
	while (!input.eof())
	{
		Coords start(0, 0);
		Coords end(0, 0);
		std::string line;
		std::string chunk;
		char temp[255];
		input.getline(temp, 256);
		line = temp;

		//Check we have a valid line to work with
		if (line[0] < '0' || line[0] > '9')
			break;

		int index = line.find(',');
		chunk = line.substr(0, index);
		line.erase(0, index + 1);
		start.x = stoi(chunk);

		index = line.find(' ');
		chunk = line.substr(0, index);
		line.erase(0, index + 1);
		start.y = stoi(chunk);

		while (!line.empty())
		{
			line.erase(0, 3);	//Remove the "-> " to reach the next coordinates
			//Read in the x coords
			int index = line.find(',');
			chunk = line.substr(0, index);
			line.erase(0, index + 1);
			end.x = stoi(chunk);

			//Read in the y coords
			index = line.find(' ');
			if (index == -1)	//Check if this is the end of the line or not
			{
				end.y = stoi(line);
				line.clear();
			}
			else
			{
				chunk = line.substr(0, index);
				line.erase(0, index + 1);
				end.y = stoi(chunk);
			}

			//Find the driection to go from start to end
			Coords direction(0, 0);
			if (start.x == end.x)
			{
				if (start.y < end.y)
					direction.y = 1;
				else
					direction.y = -1;
			}
			else
			{
				if (start.x < end.x)
					direction.x = 1;
				else
					direction.x = -1;
			}

			//Populate the list with all points between start and end
			while (!(start == end))
			{
				blocks.insert(start);
				start = start + direction;
			}
			blocks.insert(start);	//Ensure the last point in the list is never missed

			//Check and update the lowest point
			//Don't check each spot, just the end points as this still guarantees we find the lowest point
			if (start.y > lowPoint)
				lowPoint = start.y;
		}
	}

	lowPoint += 2;
	int numParticles = FillCave(&blocks, lowPoint);
	std::cout << "Number of sand particles before reaching abyss: " << numParticles << std::endl;
	exit(0);
}

int FillCave(std::set<Coords> *blocks, const int floor)
{
	int particles = 0;
	const Coords startPoint(500, 0);
	while (particles < INT_MAX)
	{
		particles++;
		Coords sand = startPoint;

		while (sand.y < floor - 1)
		{
			//Try and move sand straight down
			Coords checkPoint = sand + Coords(0, 1);
			if (blocks->find(checkPoint) == blocks->end())
			{
				sand = checkPoint;
				continue;
			}

			//Try and move sand down and left
			checkPoint = sand + Coords(-1, 1);
			if (blocks->find(checkPoint) == blocks->end())
			{
				sand = checkPoint;
				continue;
			}

			//Try and move sand down and right
			checkPoint = sand + Coords(1, 1);
			if (blocks->find(checkPoint) == blocks->end())
			{
				sand = checkPoint;
				continue;
			}

			//If we reach here, the sand can't move
			break;
		}

		blocks->insert(sand);
		//Check if we reached the abyss
		if (sand == startPoint)
			return particles;
	}

	//If we reach here, we never reached the abyss
	return -1;
}