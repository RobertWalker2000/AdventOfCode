#include <iostream>
#include <fstream>
#include <string>
#include <vector>

bool IsReportSafe(char input[256]);
bool CheckAscending(std::vector<int>* list);
bool CheckDescending(std::vector<int>* list);

int main()
{
	//Create an input stream and open the file
	const int day = 2;

	std::string fileName;
	fileName.append("2024/Inputs/D");
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

	int safeCount = 0;
	int genCount = 0;
	while (!input.eof())
	{
		//Read the line into a string
		char line[256];
		input.getline(line, 256);

		genCount++;
		if (IsReportSafe(line))
			safeCount++;
	}

	std::cout << "Total number of safe readings: " << safeCount << std::endl;
	exit(0);
}

bool IsReportSafe(char input[256])
{
	std::string report(input);

	if (report == "56 57 60 63 65 67 68 70")
		int checker = 0;

	//If the string is empty, we are finished
	if (report.empty())
		return false;

	//Append a space to the end to standardise the formatting (All numbers are followed by a space)
	report.append(" ");

	//Parse the string and read out each number in order
	std::vector<int> nums;
	while (!report.empty())
	{
		int index = report.find(' ');
		nums.push_back(std::stoi(report.substr(0, index)));
		report.erase(0, index + 1);
	}

	//Check which way the list should be checked
	if (nums.at(1) > nums.at(0))
		return CheckAscending(&nums);
	else
		return CheckDescending(&nums);
}

bool CheckAscending(std::vector<int>* list)
{
	int test1 = list->at(0);
	int test2 = list->at(1);
	//Loop through all elements in the list, except the last one. (Last element has nothing to compare against)
	for (int i = 0; i < list->size() - 1; i++)
	{
		if (list->at(i + 1) < list->at(i) + 1 || list->at(i + 1) > list->at(i) + 3)
			return false;
	}

	//All elements passed the test. List is safe
	return true;
}

bool CheckDescending(std::vector<int>* list)
{
	for (int i = 0; i < list->size() - 1; i++)
	{
		if (list->at(i + 1) > list->at(i) - 1 || list->at(i + 1) < list->at(i) - 3)
			return false;
	}

	//All elements passed the test. List is safe
	return true;
}