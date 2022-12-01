#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

void UpdateCaloriesList(int list[3], int calories);

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
	int maxCalories[3] = { 0 };
	while (!input.eof())
	{
		input.getline(currentLine, 256);

		//If the line is empty, check the total calories against the max and update if appropriate
		if (currentLine[0] == '\0')
		{
			UpdateCaloriesList(maxCalories, currentCalories);
			currentCalories = 0;
		}
		else	//If the line contains a value, add it to the running total
			currentCalories += atoi(currentLine);
	}

	int totalCalories = maxCalories[0] + maxCalories[1] + maxCalories[2];
	std::cout << "Total Calories (Top 3 Combined): " << totalCalories << std::endl;
	exit(0);
}

//Compares the current calories to the calories in the list, inserting it into the appropriate position if it is in the top 3
void UpdateCaloriesList(int list[3], int calories)
{
	if (calories > list[2])	//Start with the lowest number and work up to prevent unnecessary checks if the number is not in the top 3
	{
		if (calories > list[1])
		{
			if (calories > list[0])
			{
				list[2] = list[1];
				list[1] = list[0];
				list[0] = calories;
			}
			else
			{
				list[2] = list[1];
				list[1] = calories;
			}
		}
		else
		{
			list[2] = calories;
		}
	}
}
