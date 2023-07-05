#include <iostream>
#include <fstream>
#include <string>

int main()
{
	//Create an input stream and open the file
	const int day = 0;

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

	exit(0);
}
