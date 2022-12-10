#include <iostream>
#include <fstream>
#include <string>

int main()
{
	//Create an input stream and open the file
	const int day = 10;
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

	int valToAdd = 0;
	int x = 1;
	int cycle = 1;
	int total = 0;
	while (!input.eof())
	{
		if (valToAdd == 0)
		{
			//Read in the instructions for the cycle
			char line[10];
			input.getline(line, 256);
			std::string lineStr = line;
			if (lineStr != "noop" && lineStr.size() > 0)
			{
				std::string value = lineStr.substr(5, 256);
				int num = stoi(value);
				valToAdd = num;
			}
		}
		else
		{
			x += valToAdd;
			valToAdd = 0;
		}

		cycle++;

		//Check the x value if appropriate
		if (((cycle + 20) % 40) == 0)
		{
			total += (x * cycle);
		}
	}

	std::cout << "Total sum of checked x values: " << total << std::endl;
	exit(0);
}
