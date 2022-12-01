#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

int main()
{
	//Create an input stream and open the file
	const int day = 1;
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

	char currentLine[10];
	int currentCalories = 0;
	int maxCalories = 0;
	while (!input.eof())
	{
		input.getline(currentLine, 256);

		//If the line is empty, check the total calories against the max and update if appropriate
		if (currentLine[0] == '\0')
		{
			maxCalories = std::max(currentCalories, maxCalories);
			currentCalories = 0;
		}
		else	//If the line contains a value, add it to the running total
			currentCalories += atoi(currentLine);
	}

	std::cout << "Highest Calorie Count: " << maxCalories << std::endl;
	exit(0);
}
