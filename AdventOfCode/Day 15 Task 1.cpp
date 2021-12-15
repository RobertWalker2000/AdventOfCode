#include <iostream>
#include <fstream>
#include <vector>

#define COLUMNS 100
#define ROWS 100
#define CharToInt(x) x - 48

struct PathNode
{
	int dangerLevel;
	int totalDanger = INT_MAX;
};

struct Coords
{
	int x;
	int y;
};

void CheckAdjacentSpaces(PathNode cave[COLUMNS][ROWS], std::vector<Coords>* nodes, const int col, const int row);
bool CheckSpace(PathNode cave[COLUMNS][ROWS], const int newDanger, const int col, const int row);

int main()
{
	std::fstream input;
	input.open("Day 15 Input.txt");
	//input.open("Test Input.txt");
	if (!input)
	{
		std::cout << "ERROR: Couldn't open file" << std::endl;
		return 1;
	}

	//Populate the cave array with the correct danger levels
	PathNode cave[COLUMNS][ROWS];
	for (int row = 0; row < ROWS; row++)
	{
		for (int col = 0; col < COLUMNS; col++)
		{
			//Read the danger level into the appropriate cell
			char riskChar = input.get();
			cave[row][col].dangerLevel = CharToInt(riskChar);
		}
		input.get();	//Ignore the new line character
	}

	//Vectors to track which nodes we need to check
	std::vector<Coords> nodesToCheck;
	std::vector<Coords> updatedNodes;

	//Start pathfinding from the end point
	Coords endPoint;
	endPoint.x = COLUMNS - 1;
	endPoint.y = ROWS - 1;
	cave[endPoint.x][endPoint.y].totalDanger = cave[endPoint.x][endPoint.y].dangerLevel;
	nodesToCheck.push_back(endPoint);

	//Loop through the co-ordinates until we run out of new nodes to check
	do
	{
		updatedNodes.clear();

		//Check the spaces adjacent to each node in the 'nodesToCheck' vector
		while (!nodesToCheck.empty())
		{
			Coords currentCoords = nodesToCheck.back();
			CheckAdjacentSpaces(cave, &updatedNodes, currentCoords.x, currentCoords.y);
			nodesToCheck.pop_back();
		}

		nodesToCheck = updatedNodes;
	} while (!updatedNodes.empty());

	//We never enter the starting position, so its own danger level shouldn't be counted for the total
	int safestPath = cave[0][0].totalDanger - cave[0][0].dangerLevel;

	std::cout << "Safest path danger level: " << safestPath << std::endl;
	return 0;
}

void CheckAdjacentSpaces(PathNode cave[COLUMNS][ROWS], std::vector<Coords>* nodes, const int col, const int row)
{
	int nodeDanger = cave[col][row].totalDanger;

	//Get the co-ordinates of an adjacent space
	Coords otherNode;

	//Left
	otherNode.x = col - 1;
	otherNode.y = row;
	if (CheckSpace(cave, nodeDanger, otherNode.x, otherNode.y))	//If we succesfully update a node, push it onto the vector to check next time
		nodes->push_back(otherNode);

	//Right
	otherNode.x = col + 1;
	otherNode.y = row;
	if (CheckSpace(cave, nodeDanger, otherNode.x, otherNode.y))	//If we succesfully update a node, push it onto the vector to check next time
		nodes->push_back(otherNode);

	//Up
	otherNode.x = col;
	otherNode.y = row - 1;
	if (CheckSpace(cave, nodeDanger, otherNode.x, otherNode.y))	//If we succesfully update a node, push it onto the vector to check next time
		nodes->push_back(otherNode);

	//Down
	otherNode.x = col;
	otherNode.y = row + 1;
	if (CheckSpace(cave, nodeDanger, otherNode.x, otherNode.y))	//If we succesfully update a node, push it onto the vector to check next time
		nodes->push_back(otherNode);

}

//If the new total danger of a node would be lower than its current total, that value is updated and returns true. Otherwise, returns false
bool CheckSpace(PathNode cave[COLUMNS][ROWS], const int previousDanger, const int x, const int y)
{
	//Check the co-ordinates are in bounds
	if (x >= 0 && x < COLUMNS && y >= 0 && y < ROWS)
	{
		//Calculate the total danger level the node would have if entered from the previous node
		int newDanger = previousDanger + cave[x][y].dangerLevel;

		//If the new danger is lower than the old total, update the total
		if (cave[x][y].totalDanger > newDanger)
		{
			cave[x][y].totalDanger = newDanger;
			return true;
		}
	}

	return false;
}