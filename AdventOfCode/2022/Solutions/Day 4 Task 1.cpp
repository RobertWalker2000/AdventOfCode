#include <iostream>
#include <fstream>
#include <string>

bool ReadLine(std::ifstream* input, int values[4]);
bool HasContainedSet(const int ranges[4]);
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
			if (HasContainedSet(ranges))
				count++;
	}

	std::cout << "Number of pairs where one set is fully contained:" << count << std::endl;
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

//Returns true if either set is fully contained within the other
bool HasContainedSet(const int ranges[4])
{
	//If either the 2 upper limits or the 2 lower limits are the same, the larger set will always fully contain the smaller set
	if (ranges[0] == ranges[2] || ranges[1] == ranges[3])
		return true;

	if (ranges[0] < ranges[2])
	{
		if (ranges[1] > ranges[3])	//Set 1 starts before and ends after set 2, therefore set 1 contains set 2
			return true;
	}
	else
	{
		if (ranges[3] > ranges[1])	//Set 2 starts before and ends after set 1, therefore set 2 contains set 1
			return true;
	}

	//None of the above criteria were met, therefore neither set fully contains the other
	return false;
}

bool HasOverlap(const int ranges[4])
{

}