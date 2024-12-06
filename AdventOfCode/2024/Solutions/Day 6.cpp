#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>

int CalculateUniqueSpaces(std::vector<std::string>* map, std::pair<int, int> pos, int moveX = 0, int moveY = -1);
bool IsPosInBounds(std::pair<int, int> pos, std::vector<std::string>* map);
void TurnRight(int* x, int* y);

int main()
{
	//Create an input stream and open the file
	const int day = 0;

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

	//Read the map into a vector, searching for the starting position as we go
	std::vector<std::string> map;
	int row = 0;
	std::pair<int, int> pos;
	while (!input.eof())
	{
		char temp[256];
		input.getline(temp, 256);
		std::string line(temp);

		if (line.empty())
			break;

		map.push_back(line);	//Add the line to the map
		int col = line.find('^');	//Look for starting pos
		if (col != std::string::npos)
			pos = std::pair<int, int>(col, row);	//If we find start pos, store it

		row++;
	}

	int uniquePos = CalculateUniqueSpaces(&map, pos);

	std::cout << "Unique positions visited: " << uniquePos << std::endl;
	exit(0);
}

int CalculateUniqueSpaces(std::vector<std::string>* map, std::pair<int, int> pos, int moveX, int moveY)
{
	//Set allows us to easily track visited positions and check if they already existed
	std::set<std::pair<int, int>> uniquePositions;
	static const char obstacle = '#';

	//Keep looping until we leave the map
	while (IsPosInBounds(pos, map))
	{
		uniquePositions.insert(pos);

		//Calculate the position form after we take a step
		std::pair<int, int> nextPos(pos.first + moveX, pos.second + moveY);
		if (!IsPosInBounds(nextPos, map))
			break;

		//If we are about to hit an abstacle, turn right
		if (map->at(nextPos.second).at(nextPos.first) == obstacle)
		{
			TurnRight(&moveX, &moveY);
			continue;	//Go back to loop again in case we need to do multiple turns, or we are about to exit the map now
		}

		//Our path is clear, take a step and record the position in the set
		pos = nextPos;
	}

	return uniquePositions.size();
}

bool IsPosInBounds(std::pair<int, int> pos, std::vector<std::string>* map)
{
	//Check if we are on a valid row (In bounds vertically)
	if (pos.second < 0 || pos.second >= map->size())
		return false;

	//Knowing that we are on a valid row, check if we are within the bounds of that row
	if (pos.first < 0 || pos.first > map->at(pos.second).size())
		return false;

	//Passed all bounds checks, position is valid
	return true;
}

void TurnRight(int* x, int* y)
{
	//Check if we are moving vertically
	if (*x == 0)
	{
		if (*y == -1)	//If moving up, switch to moving right
		{
			*x = 1;
			*y = 0;
			return;
		}

		//We must be moving down, so switch to moving left
		*x = -1;
		*y = 0;
		return;
	}

	if (*x == 1)	//If we are moving right, start moving down
	{
		*x = 0;
		*y = 1;
		return;
	}

	//We must be moving left, sostart moving up
	*x = 0;
	*y = -1;
	return;
}