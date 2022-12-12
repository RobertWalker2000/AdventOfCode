#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>

typedef std::pair<int, int> Coords;

void ProcessSpot(std::vector<std::vector<char>> *maze, std::vector<std::vector<int>> *routeLength, std::queue<Coords> *spots);

int main()
{
	//Create an input stream and open the file
	const int day = 12;
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

	std::vector<std::vector<char>> maze;
	std::vector<char> line;
	Coords start;
	Coords end;
	int x = 0;
	int y = 0;

	//Read the height map in from file
	while (!input.eof())
	{
		char value = input.get();

		if (value == '\n')
		{
			maze.push_back(line);
			line.clear();
			x = 0;
			y++;
		}
		else
		{
			if (value == 'S')
			{
				line.push_back('a');
				start = Coords(y, x);
			}
			else if (value == 'E')
			{
				line.push_back('z');
				end = Coords(y, x);
			}
			else if (value >= 'a' && value <= 'z')
			{
				line.push_back(value);
			}
			x++;
		}
	}

	//Populate a copy of the maze with the route engths rather than spot heights
	std::vector<std::vector<int>> route;
	for (int i = 0; i < maze.size(); i++)
	{
		std::vector<int> temp;
		for (int j = 0; j < maze[i].size(); j++)
			temp.push_back(INT_MAX);

		route.push_back(temp);
	}
	//Initialise the start point to be 0
	route[start.first][start.second] = 0;

	//Start on the start point and loop until we have no more spots to check
	std::queue<Coords> nextSpot;
	nextSpot.push(start);

	while (!nextSpot.empty())
	{
		ProcessSpot(&maze, &route, &nextSpot);
	}
	
	int numSteps = route[end.first][end.second];
	std::cout << "Steps required to reach destination: " << numSteps << std::endl;
	exit(0);
}

void ProcessSpot(std::vector<std::vector<char>> *maze, std::vector<std::vector<int>> *routeLength, std::queue<Coords> *spots)
{
	const Coords currentSpot = spots->front();
	spots->pop();
	const int checkLength = (routeLength->at(currentSpot.first).at(currentSpot.second)) + 1;

	//Check spot above
	Coords checkSpot = currentSpot;
	checkSpot.first--;
	if (checkSpot.first >= 0)	//Check we are in bounds
	{
		if(routeLength->at(checkSpot.first).at(checkSpot.second) > checkLength)	//Check that this is the shortest known path to the spot
			if ((maze->at(checkSpot.first).at(checkSpot.second) - maze->at(currentSpot.first).at(currentSpot.second) <= 1))	//Check the spot is no more than 1 higher than the current spot
			{
				routeLength->at(checkSpot.first).at(checkSpot.second) = checkLength;
				spots->push(checkSpot);
			}
	}

	//Check spot below
	checkSpot = currentSpot;
	checkSpot.first++;
	if (checkSpot.first < maze->size())	//Check we are in bounds
	{
		if (routeLength->at(checkSpot.first).at(checkSpot.second) > checkLength)	//Check that this is the shortest known path to the spot
			if ((maze->at(checkSpot.first).at(checkSpot.second) - maze->at(currentSpot.first).at(currentSpot.second) <= 1))	//Check the spot is no more than 1 higher than the current spot
			{
				routeLength->at(checkSpot.first).at(checkSpot.second) = checkLength;
				spots->push(checkSpot);
			}
	}

	//Check spot left
	checkSpot = currentSpot;
	checkSpot.second--;
	if (checkSpot.second >= 0)	//Check we are in bounds
	{
		if (routeLength->at(checkSpot.first).at(checkSpot.second) > checkLength)	//Check that this is the shortest known path to the spot
			if ((maze->at(checkSpot.first).at(checkSpot.second) - maze->at(currentSpot.first).at(currentSpot.second) <= 1))	//Check the spot is no more than 1 higher than the current spot
			{
				routeLength->at(checkSpot.first).at(checkSpot.second) = checkLength;
				spots->push(checkSpot);
			}
	}

	//Check spot right
	checkSpot = currentSpot;
	checkSpot.second++;
	if (checkSpot.second < maze->at(0).size())	//Check we are in bounds
	{
		if (routeLength->at(checkSpot.first).at(checkSpot.second) > checkLength)	//Check that this is the shortest known path to the spot
			if ((maze->at(checkSpot.first).at(checkSpot.second) - maze->at(currentSpot.first).at(currentSpot.second) <= 1))	//Check the spot is no more than 1 higher than the current spot
			{
				routeLength->at(checkSpot.first).at(checkSpot.second) = checkLength;
				spots->push(checkSpot);
			}
	}
}
