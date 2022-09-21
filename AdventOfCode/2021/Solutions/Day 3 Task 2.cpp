#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct Reading
{
	int value = 0;
	bool isValid = false;
};

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
	std::vector<Reading> readings;

	const int binaryLength = 12;

	//Loop through all values in file
	while(inStream >> currentLine)
	{
		//Store the current value as an int, converting from string to int with a binary input
		value = std::stoi(currentLine, 0, 2);

		//Convert the value into a reading and push it onto the vector
		Reading tempReading;
		tempReading.value = value;
		tempReading.isValid = true;
		readings.push_back(tempReading);

	}

	//We are done reading from the file, so close it
	inStream.close();

	int oxygenIndex = 0;
	//Check each bit, incrementing the corresponding bit in the array if the bit is true, decrementing if it is 0
	for (int bit = binaryLength - 1; bit >= 0; bit--)
	{
		int mask = 1 << bit;
		int counter = 0;
		int validCount = 0;

		//Find the most popular value in the current bit
		for (int i = 0; i < readings.size(); i++)
		{
			if (readings[i].isValid)
			{
				if (readings[i].value & mask)
					counter++;
				else
					counter--;
			}
		}

		//Check the most popular value
		bool verify = false;
		if (counter >= 0)
			verify = true;

		//Chek all valid entries again, updating their valid flag
		for (int i = 0; i < readings.size(); i++)
		{
			if (readings[i].isValid)
			{
				if (readings[i].value & mask)
					readings[i].isValid = verify;
				else
					readings[i].isValid = !verify;
			}
		}

		for (int i = 0; i < readings.size(); i++)
		{
			if (readings[i].isValid)
			{
				//Count how many valid readings are left, and the index of the last valid reading
				validCount++;
				oxygenIndex = i;
			}
		}

		if (validCount == 1)
			break;
	}
	int oxygen = readings[oxygenIndex].value;

	//Set all readings back to true
	for (int i = 0; i < readings.size(); i++)
		readings[i].isValid = true;

	int carbonIndex = 0;
	//Check each bit, incrementing the corresponding bit in the array if the bit is true, decrementing if it is 0
	for (int bit = binaryLength - 1; bit >= 0; bit--)
	{
		int mask = 1 << bit;
		int counter = 0;
		int validCount = 0;

		//Find the most popular value in the current bit
		for (int i = 0; i < readings.size(); i++)
		{
			if (readings[i].isValid)
			{
				if (readings[i].value & mask)
					counter++;
				else
					counter--;
			}
		}

		//Check the most popular value
		bool verify = false;
		if (counter < 0)
			verify = true;

		//Chek all valid entries again, updating their valid flag
		for (int i = 0; i < readings.size(); i++)
		{
			if (readings[i].isValid)
			{
				if (readings[i].value & mask)
					readings[i].isValid = verify;
				else
					readings[i].isValid = !verify;
			}
		}

		for (int i = 0; i < readings.size(); i++)
		{
			if (readings[i].isValid)
			{
				//Count how many valid readings are left, and the index of the last valid reading
				validCount++;
				carbonIndex = i;
			}
		}

		if (validCount == 1)
			break;
	}
	int carbon = readings[carbonIndex].value;

	//Produce and output final result
	std::cout << "Oxygen: " << oxygen << std::endl;
	std::cout << "Carbon: " << carbon << std::endl;
	std::cout << "Life Support Rating: " << oxygen * carbon << std::endl;
	exit(0);
}
