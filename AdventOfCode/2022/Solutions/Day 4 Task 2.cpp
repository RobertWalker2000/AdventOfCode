#include <iostream>
#include <fstream>
#include <string>

bool ReadLine(std::ifstream* input, int values[4]);
bool HasOverlap(const int ranges[4]);

int main()
{
	//Create an input stream and open the file
	const int day = 4;
	const int task = 1;

	std::string fileName;
	fileName.append("2022/Inputs/D");
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
		exit(1);
	}

	int count = 0;
	while (!input.eof())
	{
		int ranges[4];
		if(ReadLine(&input, ranges))
			if (HasOverlap(ranges))
				count++;
	}

	std::cout << "Number of pairs with overlapping sets:" << count << std::endl;
	exit(0);
}

//Reads in a line from the input file and separates it into the 4 range limits
bool ReadLine(std::ifstream* input, int values[4])
{
	char buffer[10];
	
	//Check we have a valid line of numbers to read
	if (input->peek() < '0' || input->peek() > '9')
		return false;

	input->getline(buffer, 256, '-');
	values[0] = atoi(buffer);

	input->getline(buffer, 256, ',');
	values[1] = atoi(buffer);

	input->getline(buffer, 256, '-');
	values[2] = atoi(buffer);

	input->getline(buffer, 256);
	values[3] = atoi(buffer);

	return true;
}

//Returns true if there is any overlap at all between the sets
bool HasOverlap(const int ranges[4])
{
	//Whenever an overlap occurs, at least one set will contain at least one endpoint from the other
	//Therefore, check each endpoint against the opposite range

	if (ranges[0] >= ranges[2] && ranges[0] <= ranges[3])
		return true;

	if (ranges[1] >= ranges[2] && ranges[1] <= ranges[3])
		return true;

	if (ranges[2] >= ranges[0] && ranges[2] <= ranges[1])
		return true;

	if (ranges[3] >= ranges[0] && ranges[3] <= ranges[1])
		return true;

	//No end point was contained in the other set, so no overlaps
	return false;
}