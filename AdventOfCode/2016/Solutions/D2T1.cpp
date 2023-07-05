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
	std::pair<int, int> pos(1, 1);
	int keypad[3][3]{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
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

			if (dir == 'R' && pos.first < 2)
				pos.first++;
			else if (dir == 'L' && pos.first > 0)
				pos.first--;
			else if (dir == 'U' && pos.second > 0)
				pos.second--;
			else if (dir == 'D' && pos.second < 2)
				pos.second++;
		}

		passcode.append(std::to_string(keypad[pos.second][pos.first]));
	}

	std::cout << "Passcode: " << passcode << std::endl;
	exit(0);
}
