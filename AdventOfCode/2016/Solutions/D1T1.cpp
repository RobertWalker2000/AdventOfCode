#include <iostream>
#include <fstream>
#include <string>

int Turn(int currentDir, char turnDir);

int main()
{
	//Create an input stream and open the file
	const int day = 1;

	std::string fileName;
	fileName.append("2016/Inputs/D");
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

	int direction = 1;
	int horDist = 0;
	int vertDist = 0;
	while (!input.eof())
	{
		//Get the turn direction
		char turnDir = input.get();
		direction = Turn(direction, turnDir);

		//Get the walking distance
		char distChar[10];
		input.getline(distChar, 256, ',');
		int distance = atoi(distChar);

		//Move
		switch (direction)
		{
		case 1: vertDist += distance;
			break;
		case 2: horDist += distance;
			break;
		case 3: vertDist -= distance;
			break;
		case 4: horDist -= distance;
			break;
		default: std::cout << "ERROR: Unexpected walk direction" << std::endl;
			break;
		}

		//Clear the space
		input.get();
	}

	std::cout << "Total distance: " << abs(horDist) + abs(vertDist) << std::endl;
	exit(0);
}

int Turn(int currentDir, char turnDir)
{
	if (turnDir == 'R')
	{
		currentDir++;
		if (currentDir > 4)
			currentDir = 1;
	}
	else if (turnDir == 'L')
	{
		currentDir--;
		if (currentDir < 1)
			currentDir = 4;
	}
	else
	{
		std::cout << "ERROR: Unexpected turn direction" << std::endl;
	}

	return currentDir;
}
