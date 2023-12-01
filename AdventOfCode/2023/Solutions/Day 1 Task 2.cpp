#include <iostream>
#include <fstream>
#include <string>

std::string DigitiseNumbers(const std::string in);

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

		line = DigitiseNumbers(line);

		value[0] = line[line.find_first_of("0123456789")];
		value[1] = line[line.find_last_of("0123456789")];

		total += atoi(value);
	}

	std::cout << "Sum of calibration values: " << total << std::endl;
	exit(0);
}

//Converts the word-form instances of the numbers with their arabic digit counterparts, ready to be parsed by the original function
std::string DigitiseNumbers(const std::string input)
{
	std::string words[9] = { "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };

	std::string result = input;
	//Find each word in the above array, and replace it with the digit version
	for (int i = 0; i < 9; i++)
	{
		//Ensure we get every instance of a given word
		while (true)
		{
			size_t pos = result.find(words[i], 0);
			if (pos == std::string::npos)
				break;

			//Only need to replace one letter of the word as this is enough to prevent the word from being found again
			//Additionally, the position of the digits overall does not matter, only pos in relation to each other, therefore the leftover letters change nothing
			//Changing the 2nd letter of the word also allows us to find/update words which share a first/last letter with another number
			result[pos + 1] = '1' + i;
		}
	}

	return result;
}