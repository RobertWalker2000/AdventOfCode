#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>

using Coords = std::pair<int, int>;
using DirCoords = std::pair < std::pair<int, int>, std::pair<int, int>>;

std::set<Coords> CalculateUniqueSpaces(std::vector<std::string>* map, Coords pos);
int CalculateLoopingObstacles(std::vector<std::string>* map, std::set<Coords> possibleObstructions, Coords startPos);
bool DoesMapLoop(std::vector<std::string>* map, Coords pos);
bool IsPosInBounds(Coords pos, std::vector<std::string>* map);
void TurnRight(int* x, int* y);

int main()
{
	//Create an input stream and open the file
	const int day = 6;

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
	Coords pos;
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
			pos = Coords(col, row);	//If we find start pos, store it

		row++;
	}

	std::set<Coords> uniquePositions = CalculateUniqueSpaces(&map, pos);
	int possibleLoops = CalculateLoopingObstacles(&map, uniquePositions, pos);

	std::cout << "Unique positions visited: " << uniquePositions.size() << std::endl;
	std::cout << "Potential ways to create aloop: " << possibleLoops << std::endl;
	exit(0);
}

std::set<Coords> CalculateUniqueSpaces(std::vector<std::string>* map, Coords pos)
{
	//Set allows us to easily track visited positions and check if they already existed
	std::set<Coords> uniquePositions;
	static const char obstacle = '#';

	int moveX = 0;
	int moveY = -1;

	//Keep looping until we leave the map
	while (IsPosInBounds(pos, map))
	{
		uniquePositions.insert(pos);

		//Calculate the position form after we take a step
		Coords nextPos(pos.first + moveX, pos.second + moveY);
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

	return uniquePositions;
}

bool IsPosInBounds(Coords pos, std::vector<std::string>* map)
{
	//Check if we are on a valid row (In bounds vertically)
	if (pos.second < 0 || pos.second >= map->size())
		return false;

	//Knowing that we are on a valid row, check if we are within the bounds of that row
	if (pos.first < 0 || pos.first >= map->at(pos.second).size())
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

int CalculateLoopingObstacles(std::vector<std::string>* map, std::set<Coords> possibleObstructions, Coords startPos)
{
	//We can't obstruct the starting position, so remove it form the set
	possibleObstructions.erase(startPos);

	//Obstruction would only make a difference if it is on the existing path
	//Loop through all existing positions and check if an obstruction in that position creates a loop
	int loops = 0;
	static const char obstruction = '#';
	static const char clearSpace = '.';
	for (std::set<Coords>::iterator it = possibleObstructions.begin(); it != possibleObstructions.end(); it++)
	{
		//Update the current posiiton to be an obstruction
		map->at(it->second).at(it->first) = obstruction;

		//Check if the new map loops
		if (DoesMapLoop(map, startPos))
			loops++;

		//Remove the new obstruction to restore the original map
		map->at(it->second).at(it->first) = clearSpace;
	}

	return loops;
}

bool DoesMapLoop(std::vector<std::string>* map, Coords pos)
{
	//A set that tracks the positions we have visited, and the direction we were facing
	//If we ever end up in the same position *and* direction, we are in a loop
	std::set<DirCoords> stepsTaken;

	int moveX = 0;
	int moveY = -1;

	static const char obstacle = '#';

	//Keep looping until we leave the map
	while (IsPosInBounds(pos, map))
	{
		//Create a DirCoords that represents the current position and direction
		Coords dir(moveX, moveY);
		DirCoords dc;
		dc.first = pos;
		dc.second = dir;

		//Try inserting this DirCoords into the set. If it already exists, we are in a loop
		if (!stepsTaken.insert(dc).second)
			return true;

		//Calculate the position form after we take a step
		Coords nextPos(pos.first + moveX, pos.second + moveY);
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

	//We exited the map without entering a loop
	return false;
}