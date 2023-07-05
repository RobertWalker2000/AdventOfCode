#include <iostream>
#include <fstream>
#include <string>

int main()
{
	//Create an input stream and open the file
	const int day = 2;

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

	std::string passcode = "";
	std::pair<int, int> pos(3, 1);
	char keypad[7][7]{	'X', 'X', 'X', 'X', 'X', 'X', 'X',
						'X', 'X', 'X', '1', 'X', 'X', 'X',
						'X', 'X', '2', '3', '4', 'X', 'X', 
						'X', '5', '6', '7', '8', '9', 'X', 
						'X', 'X', 'A', 'B', 'C', 'X', 'X', 
						'X', 'X', 'X', 'D', 'X', 'X', 'X', 
						'X', 'X', 'X', 'X', 'X', 'X', 'X' };

	while (!input.eof())
	{
		std::string line;
		input >> line;

		if (line.size() == 0)
			break;

		while (!line.empty())
		{
			char dir = line[0];
			line.erase(0, 1);
			std::pair<int, int> newPos = pos;

			if (dir == 'R')
				newPos.first++;
			else if (dir == 'L')
				newPos.first--;
			else if (dir == 'U')
				newPos.second--;
			else if (dir == 'D')
				newPos.second++;

			if (keypad[newPos.second][newPos.first] != 'X')
				pos = newPos;
		}

		passcode.append(1, keypad[pos.second][pos.first]);
	}

	std::cout << "Passcode: " << passcode << std::endl;
	exit(0);
}
