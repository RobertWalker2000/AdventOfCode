#include <iostream>
#include <fstream>
#include <string>
#include <vector>

bool IsReportSafe(char input[256]);
bool IsReportTolerable(char input[256]);
bool CheckAscending(std::vector<int>* list);
bool CheckDescending(std::vector<int>* list);
bool CheckAscendingWithTolerance(std::vector<int>* list);
bool CheckDescendingWithTolerance(std::vector<int>* list);
int FindIssueAscending(std::vector<int>* list);
int FindIssueDescending(std::vector<int>* list);

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
	int tolCount = 0;
	while (!input.eof())
	{
		//Read the line into a string
		char line[256];
		input.getline(line, 256);

		//Still want to check for safe values to cover Task 1
		//All safe values are automatically tolerable, so don't need to re-check them
		if (IsReportSafe(line))
		{
			safeCount++;
			tolCount++;
			continue;
		}

		//Reading was unsafe, now check if it is tolerable
		if (IsReportTolerable(line))
			tolCount++;
	}

	std::cout << "Total number of safe readings: " << safeCount << std::endl;
	std::cout << "Total number of tolerable readings: " << tolCount << std::endl;
	exit(0);
}

//Returns true if the report is safe with no modifications required
bool IsReportSafe(char input[256])
{
	std::string report(input);

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

//Returns true if the report can be made safe by removing only a single element
bool IsReportTolerable(char input[256])
{
	std::string report(input);

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

	//Check if the list is tolerable and in ascending order
	if (CheckAscendingWithTolerance(&nums))
		return true;

	//If the list is not tolerable and ascending, report if it is tolerable and descending
	return(CheckDescendingWithTolerance(&nums));
}

//Returns true if the report is safe AND in ascending order
bool CheckAscending(std::vector<int>* list)
{
	return (FindIssueAscending(list) == -1);
}

//Returns true if the list can be made safe *in ascending order* by removing one element
bool CheckAscendingWithTolerance(std::vector<int>* list)
{
	//Find the first issue with the list in an ascending check
	int issue = FindIssueAscending(list);

	//Check if the list is already safe
	if (issue == -1)
		return true;

	//If unsafe, check again after removing one of the problem elements
	std::vector<int> numsTol = *list;
	numsTol.erase(numsTol.begin() + issue);

	//Check if removing the element fixed the report
	if (CheckAscending(&numsTol))
		return true;

	//If the report still isn't safe, try removing the other problem element
	issue++;
	numsTol = *list;
	numsTol.erase(numsTol.cbegin() + issue);

	//Report whether this fixed the list or not
	return (CheckAscending(&numsTol));
}

//Returns the index of the first element which makes the report unsafe when checked in ascending order. If report is safe, returns -1.
int FindIssueAscending(std::vector<int>* list)
{
	int test1 = list->at(0);
	int test2 = list->at(1);
	//Loop through all elements in the list, except the last one. (Last element has nothing to compare against)
	for (int i = 0; i < list->size() - 1; i++)
	{
		if (list->at(i + 1) < list->at(i) + 1 || list->at(i + 1) > list->at(i) + 3)
			return i;
	}

	//All elements passed the test. List is safe
	return -1;
}

//Returns true if the report is safe AND in descending order
bool CheckDescending(std::vector<int>* list)
{
	return (FindIssueDescending(list) == -1);
}

//Returns true if the list can be made safe *in descending order* by removing one element
bool CheckDescendingWithTolerance(std::vector<int>* list)
{
	//Find the first issue with the list in an ascending check
	int issue = FindIssueDescending(list);

	//Check if the list is already safe
	if (issue == -1)
		return true;

	//If unsafe, check again after removing one of the problem elements
	std::vector<int> numsTol = *list;
	numsTol.erase(numsTol.begin() + issue);

	//Check if removing the element fixed the report
	if (CheckDescending(&numsTol))
		return true;

	//If the report still isn't safe, try removing the other problem element
	issue++;
	numsTol = *list;
	numsTol.erase(numsTol.cbegin() + issue);

	//Report whether this fixed the list or not
	return (CheckDescending(&numsTol));
}

//Returns the index of the first element which makes the report unsafe when checked in descending order. If report is safe, returns -1.
int FindIssueDescending(std::vector<int>* list)
{
	for (int i = 0; i < list->size() - 1; i++)
	{
		if (list->at(i + 1) > list->at(i) - 1 || list->at(i + 1) < list->at(i) - 3)
			return i;
	}

	//All elements passed the test. List is safe
	return -1;
}