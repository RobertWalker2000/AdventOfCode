#include <iostream>
#include <fstream>
#include <string>

int main()
{
	//Create an input stream and open the file
	std::ifstream inStream;
	inStream.open("Task 5 Input.txt");

	//Check the input stream is valid
	if (!inStream)
	{
		std::cout << "Unable to open file" << std::endl;
		exit(1);
	}

	//If we reach here, file was opened succesfully
	std::string currentLine;
	int value = 0;

	const int binaryLength = 12;
	int valueBin[binaryLength] = { 0 };

	//Loop through all values in file
	while(inStream >> currentLine)
	{
		//Store the current value as an int, converting from string to int with a binary input
		value = std::stoi(currentLine, 0, 2);

		//Check each bit, incrementing the corresponding bit in the array if the bit is true, decrementing if it is 0
		for (int bit = 0; bit < binaryLength; bit++)
		{
			//Binary string is reversed here
			int mask = 1 << bit;
			if (value & mask)
				valueBin[bit] += 1;
			else
				valueBin[bit] -= 1;
		}
	}

	//We are done reading from the file, so close it
	inStream.close();

	//Once we have looped through all values, check the valueBin
	//If an element is +ve, 1 was most common. -ve means 0 was most common
	//If any element of valueBin is 0 then 1 and 0 were equally common, which is an issue
	int gamma = 0;
	int epsilon = 0;

	for (int i = 0; i < binaryLength; i++)
	{
		//Binary string is reversed again here, so back the right way
		if (valueBin[i] > 0)
		{
			gamma += 1 << i;
		}
		else if (valueBin[i] < 0)
		{
			epsilon += 1 << i;
		}
		else
		{
			std::cout << "Invalid result, element " << i << " had equal count of 1 and 0" << std::endl;
			exit(1);
		}
	}

	//Produce and output final result
	int power = gamma * epsilon;
	std::cout << "Power consumption: " << power << std::endl;

	exit(0);
}
