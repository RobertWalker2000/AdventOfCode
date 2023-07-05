#include <iostream>
#include <fstream>
#include <string>

int main()
{
	//Create an input stream and open the file
	const int day = 3;

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

	int counter = 0;
	while (!input.eof())
	{
		std::string side;
		int a, b, c;

		//Read in the first side
		input >> side;
		if (side.length() == 0)
			break;
		a = std::stoi(side);

		//Read in the second side
		input >> side;
		if (side.length() == 0)
			break;
		b = std::stoi(side);

		//Read in the last side
		input >> side;
		if (side.length() == 0)
			break;
		c = std::stoi(side);

		bool isValid = true;

		isValid = isValid && ((a + b) > c);
		isValid = isValid && ((a + c) > b);
		isValid = isValid && ((b + c) > a);

		if (isValid)
			counter++;
	}

	std::cout << "Number of valid triangles: " << counter << std::endl;
	exit(0);
}
