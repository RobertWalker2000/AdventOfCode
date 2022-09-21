#include <iostream>
#include <fstream>
#include <vector>

#define COLUMNS 100
#define ROWS 100
#define REPEATS 5
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

void CheckAdjacentSpaces(PathNode cave[COLUMNS * REPEATS][ROWS + 1], std::vector<Coords>* nodes, const int col, const int row);
bool CheckExtendedSpace(PathNode cave[COLUMNS * REPEATS][ROWS + 1], const int newDanger, const int col, const int row);
void CreateFirstRow(PathNode extendedNodes[COLUMNS * REPEATS][ROWS + 1], PathNode firstCave[COLUMNS][ROWS]);
void DoPathfinding(PathNode extendedNodes[COLUMNS * REPEATS][ROWS + 1], std::vector<Coords>* nodesToCheck);
void IncrementNodes(PathNode extendedNodes[COLUMNS * REPEATS][ROWS + 1]);
void SetCheckRow(std::vector<Coords>* nodes);

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
	PathNode firstCave[COLUMNS][ROWS];
	for (int row = 0; row < ROWS; row++)
	{
		for (int col = 0; col < COLUMNS; col++)
		{
			//Read the danger level into the appropriate cell
			char riskChar = input.get();
			firstCave[row][col].dangerLevel = CharToInt(riskChar);
		}
		input.get();	//Ignore the new line character
	}

	//Vectors to track which nodes we need to check
	std::vector<Coords> nodesToCheck;
	std::vector<Coords> updatedNodes;

	//Start pathfinding from the end point
	Coords startPoint;
	startPoint.x = 0;
	startPoint.y = 0;
	firstCave[0][0].totalDanger = 0;
	
	nodesToCheck.push_back(startPoint);

	PathNode extendedNodes[COLUMNS * REPEATS][ROWS + 1];
	extendedNodes[0][0].totalDanger = 0;

	//Check the first row
	CreateFirstRow(extendedNodes, firstCave);
	DoPathfinding(extendedNodes, &nodesToCheck);

	//Check all middle rows
	for (int i = 1; i < REPEATS - 1; i++)
	{
		IncrementNodes(extendedNodes);
		SetCheckRow(&nodesToCheck);
		DoPathfinding(extendedNodes, &nodesToCheck);
	}

	//Increment to the last row, but with the bottom row being effectively blocked off
	IncrementNodes(extendedNodes);
	SetCheckRow(&nodesToCheck);
	for (int i = 0; i < COLUMNS * REPEATS; i++)
	{
		extendedNodes[i][ROWS].dangerLevel = 100;
		extendedNodes[i][ROWS].totalDanger = INT_MAX;
	}
	DoPathfinding(extendedNodes, &nodesToCheck);


	int danger = extendedNodes[(COLUMNS * REPEATS) - 1][ROWS - 1].totalDanger;
	std::cout << "Safest path danger level: " << danger << std::endl;
	return 0;
}

void CheckAdjacentSpaces(PathNode cave[COLUMNS * REPEATS][ROWS + 1], std::vector<Coords>* nodes, const int col, const int row)
{
	int nodeDanger = cave[col][row].totalDanger;

	//Get the co-ordinates of an adjacent space
	Coords otherNode;

	//Left
	otherNode.x = col - 1;
	otherNode.y = row;
	if (CheckExtendedSpace(cave, nodeDanger, otherNode.x, otherNode.y))	//If we succesfully update a node, push it onto the vector to check next time
		nodes->push_back(otherNode);

	//Right
	otherNode.x = col + 1;
	otherNode.y = row;
	if (CheckExtendedSpace(cave, nodeDanger, otherNode.x, otherNode.y))	//If we succesfully update a node, push it onto the vector to check next time
		nodes->push_back(otherNode);

	//Up
	otherNode.x = col;
	otherNode.y = row - 1;
	if (CheckExtendedSpace(cave, nodeDanger, otherNode.x, otherNode.y))	//If we succesfully update a node, push it onto the vector to check next time
		nodes->push_back(otherNode);

	//Down
	otherNode.x = col;
	otherNode.y = row + 1;
	if (CheckExtendedSpace(cave, nodeDanger, otherNode.x, otherNode.y))	//If we succesfully update a node, push it onto the vector to check next time
		nodes->push_back(otherNode);

}

//If the new total danger of a node would be lower than its current total, that value is updated and returns true. Otherwise, returns false
bool CheckExtendedSpace(PathNode cave[COLUMNS * REPEATS][ROWS + 1], const int previousDanger, const int x, const int y)
{
	//Check the co-ordinates are in bounds
	if (x >= 0 && x < COLUMNS * REPEATS && y >= 0 && y <= ROWS)
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

void CreateFirstRow(PathNode extendedNodes[COLUMNS * REPEATS][ROWS + 1], PathNode firstCave[COLUMNS][ROWS])
{
	//Create a graph represeinting a slice of the total graph.
	//This graph represents the 5 chunks at the top of the graph
	for (int i = 0; i < COLUMNS; i++)
	{
		for (int j = 0; j < ROWS; j++)
		{
			for (int loops = 0; loops < REPEATS; loops++)
			{
				int x = i + (loops * COLUMNS);
				int y = j;
				extendedNodes[x][y].dangerLevel = firstCave[i][j].dangerLevel + loops;
				while (extendedNodes[x][y].dangerLevel > 9)
					extendedNodes[x][y].dangerLevel -= 9;
			}
		}
	}

	//Populate the last row of the array
	for (int i = 0; i < COLUMNS * REPEATS; i++)
	{
		int x = i;
		int y = ROWS;
		extendedNodes[x][y].dangerLevel = extendedNodes[x][0].dangerLevel + 1;
		while (extendedNodes[x][y].dangerLevel > 9)
			extendedNodes[x][y].dangerLevel -= 9;
	}
}

void DoPathfinding(PathNode extendedNodes[COLUMNS * REPEATS][ROWS + 1], std::vector<Coords>* nodesToCheck)
{
	std::vector<Coords> updatedNodes;
	do
	{
		updatedNodes.clear();

		while (!nodesToCheck->empty())
		{
			Coords currentNode = nodesToCheck->back();
			CheckAdjacentSpaces(extendedNodes, &updatedNodes, currentNode.x, currentNode.y);
			nodesToCheck->pop_back();
		}

		*nodesToCheck = updatedNodes;
	} while (!updatedNodes.empty());
}

void IncrementNodes(PathNode extendedNodes[COLUMNS * REPEATS][ROWS + 1])
{
	//Copy the last line's total danger value over to the first line
	for (int i = 0; i < COLUMNS * REPEATS; i++)
	{
		extendedNodes[i][0].totalDanger = extendedNodes[i][ROWS].totalDanger;

		extendedNodes[i][0].dangerLevel++;
		while (extendedNodes[i][0].dangerLevel > 9)
			extendedNodes[i][0].dangerLevel -= 9;
	}

	//Increase the danger level of all nodes by 1
	for (int i = 0; i < COLUMNS * REPEATS; i++)
	{
		for (int j = 1; j < ROWS + 1; j++)
		{
			extendedNodes[i][j].totalDanger = INT_MAX;
			extendedNodes[i][j].dangerLevel++;

			while (extendedNodes[i][j].dangerLevel > 9)
				extendedNodes[i][j].dangerLevel -= 9;
		}
	}
}

void SetCheckRow(std::vector<Coords>* nodes)
{
	Coords co;
	co.y = 0;
	for (int i = 0; i < COLUMNS * REPEATS; i++)
	{
		co.x = i;
		nodes->push_back(co);
	}
}