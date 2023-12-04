#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <vector>

typedef std::array<std::string, 3> StrArr;
int ScanMiddleLine(StrArr lines);
int CalculateGearRatio(StrArr lines, int symbolPos);
int EvaluateAndReplaceNumber(std::string *line, int i);

int main()
{
	//Create an input stream and open the file
	const int day = 3;

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

	std::array<std::string, 3> lines;

	//Set up the array to have the first line at the bottom, and 2 'empty' rows above it
	//This willl allow us to feed it into the loop and have the first line moved into the middle to be read
	char firstLine[256];
	input.getline(firstLine, 256);
	lines[2] = firstLine;
	lines[0].append(lines[2].size(), '.');
	lines[1].append(lines[2].size(), '.');

	int result = 0;
	while (!input.eof())
	{
		//Read in the next line
		char temp[256];
		input.getline(temp, 256);

		//Shift the lines up and feed the new line onto the bottom
		lines[0] = lines[1];
		lines[1] = lines[2];
		lines[2].clear();
		lines[2] = temp;

		//If the bottom line is empty, fill it with periods
		if (lines[2].empty())
			lines[2].append(lines[1].size(), '.');

		result += ScanMiddleLine(lines);
	}

	std::cout << "Total sum of all valid part numbers: " << result << std::endl;
	exit(0);
}

const std::string nums = "0123456789";
//Scans the middle row of the array for symbols. When a symbol is found, the value of all adjacent numbers is calculated. This is repeated for all symbols on the line, and the result is then summed and returned.
int ScanMiddleLine(StrArr lines)
{
	int result = 0;
	for (int i = 0; i < lines.at(1).length(); i++)
	{
		//If the current char is not a symbol, move to the next char
		if(nums.find(lines.at(1)[i]) != std::string::npos || lines.at(1)[i] == '.')
			continue;

		//If we found a symbol, get the value of all adjacent numbers and add them to the total
		result += CalculateGearRatio(lines, i);
	}

	return result;
}

//Loops through all spaces adjacent to the given index on the middle row and sums the values of all numbers found
int CalculateGearRatio(StrArr lines, int symbolPos)
{
	std::vector<int> adjacentNums;

	if (symbolPos > 0)
	{
		adjacentNums.push_back(EvaluateAndReplaceNumber(&lines.at(0), symbolPos - 1));
		adjacentNums.push_back(EvaluateAndReplaceNumber(&lines.at(1), symbolPos - 1));
		adjacentNums.push_back(EvaluateAndReplaceNumber(&lines.at(2), symbolPos - 1));
	}

	adjacentNums.push_back(EvaluateAndReplaceNumber(&lines.at(0), symbolPos));
	adjacentNums.push_back(EvaluateAndReplaceNumber(&lines.at(2), symbolPos));

	if (symbolPos < lines.at(1).size() - 1)
	{
		adjacentNums.push_back(EvaluateAndReplaceNumber(&lines.at(0), symbolPos + 1));
		adjacentNums.push_back(EvaluateAndReplaceNumber(&lines.at(1), symbolPos + 1));
		adjacentNums.push_back(EvaluateAndReplaceNumber(&lines.at(2), symbolPos + 1));
	}

	int count = 0;
	int ratio = 1;
	while (!adjacentNums.empty())
	{
		if (adjacentNums.back() != 0)
		{
			count++;
			ratio *= adjacentNums.back();
		}

		adjacentNums.pop_back();
	}

	if (count == 2)
		return ratio;
	else
		return 0;
}

//First checks if the given char is a digit. If so, the full number is found and removed.
int EvaluateAndReplaceNumber(std::string *line, int i)
{
	if(nums.find(line->at(i)) == std::string::npos)
		return 0;

	//Find the bounds of the number within the string
	int numStart = line->find_last_not_of(nums, i);
	int numEnd = line->find_first_not_of(nums, i);	//Note that numEnd represents the index AFTER the last digit of the number

	//Adjust the indexes to sit on the first/last digits of the string, while also checking for invalid values
	if (numStart == std::string::npos)
		numStart = 0;
	else
		numStart++;

	if (numEnd == std::string::npos)
		numEnd = line->size();

	int numLen = numEnd - numStart;

	//Read and store the int value of the number
	int result = std::stoi(line->substr(numStart, numLen));

	//Replace the number with an equal length string of periods
	line->replace(numStart, numLen, numLen, '.');

	return result;
}