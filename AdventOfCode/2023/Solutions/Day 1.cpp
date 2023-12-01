#include <iostream>
#include <fstream>
#include <string>

int main()
{
	//Create an input stream and open the file
	const int day = 1;

	std::string fileName;
	fileName.append("2023/Inputs/D");
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

	int total = 0;
	while (!input.eof())
	{
		char temp[256];
		char value[2];
		input.getline(temp, 256);

		std::string line(temp);
		if (line.empty())
			break;

		value[0] = line[line.find_first_of("0123456789")];
		value[1] = line[line.find_last_of("0123456789")];

		total += atoi(value);
	}

	std::cout << "Sum of calibration values: " << total << std::endl;
	exit(0);
}
