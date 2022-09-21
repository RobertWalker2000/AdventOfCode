#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int main()
{
	//Create an input stream and open the file
	const int day = 1;
	const int task = 1;

	std::string fileName;
	fileName.append("2015/Inputs/D");
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
		return 1;
	}

	std::vector<char> instructions;
	int floor = 0;
	int count = 0;
	while (!input.eof())
	{
		count++;
		char currentChar = input.get();

		if (currentChar == '(')
			floor++;
		else if (currentChar == ')')
			floor--;
		else
		{
			std::cout << "ERROR:Unexpected character in input stream" << std::endl;
			continue;
		}

		instructions.push_back(currentChar);

		if (floor < 0)
			break;
	}


	//We are done reading from the file, so close it
	input.close();

	//Output results, with a verification value to ensure every value was checked
	std::cout << "Entered basement at step: " << count << std::endl;

	return 0;
}
