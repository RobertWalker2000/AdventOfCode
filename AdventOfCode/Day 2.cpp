#include <iostream>
#include <fstream>
#include <string>

int main()
{
	//Create an input stream and open the file
	std::ifstream inStream;
	inStream.open("Task 3 Input.txt");

	//Check the input stream is valid
	if (!inStream)
	{
		std::cout << "Unable to open file" << std::endl;
		exit(1);
	}

	//If we reach here, file was opened succesfully
	std::string direction;
	int distance = 0;

	int forward = 0;
	int depth = 0;
	int aim = 0;

	//Loop through all values in the file, assigning the newest value to val4
	while (inStream >> direction)
	{
		inStream >> distance;

		if (direction == "forward")
		{
			forward += distance;
			depth += aim * distance;
		}
		else if (direction == "down")
		{
			aim += distance;
		}
		else if (direction == "up")
		{
			aim -= distance;
		}
		else
		{
			std::cout << "An invalid direction was read in, abort" << std::endl;
			exit(1);
		}
	}

	//We are done reading from the file, so close it
	inStream.close();

	//Output results, with a verification value to ensure every value was checked
	std::cout << "Forward distance: " << forward << std::endl;
	std::cout << "Depth: " << depth << std::endl;
	std::cout << "Multiplied answer: " << forward * depth << std::endl;

	exit(0);
}
