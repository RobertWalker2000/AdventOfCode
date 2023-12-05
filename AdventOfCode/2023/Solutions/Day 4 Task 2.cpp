#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <algorithm>
#include <vector>

std::set<int> CreateWinningSet(std::string nums);
int CalculateScore(std::set<int> winners, std::string nums);

int main()
{
	//Create an input stream and open the file
	const int day = 4;

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

	int score = 0;
	std::vector<int> bonuses;
	while (!input.eof())
	{
		char line[256];
		input.getline(line, 256);
		if (line[0] == NULL)
			break;

		//Store the line and remove the ticket ID
		std::string winners = line;
		winners.erase(0, winners.find(':') + 2);	//Erase the seperator and the space
		int mid = winners.find('|');

		//Create a new string to store the given numbers, then erase them from the winners
		std::string myNums = winners.substr(mid + 1);	//Skips the seperator
		while (myNums[0] == ' ')
			myNums.erase(0, 1);	//Erase all spaces until we reach the first number
		winners.erase(mid - 1);	//Erase everything from the space before the seperator and later


		int matches = CalculateScore(CreateWinningSet(winners), myNums);

		//Now find out how many copies of this card we had
		int copies = 1;
		if (!bonuses.empty())
		{
			copies += bonuses[0];
			bonuses.erase(bonuses.begin());
		}

		//Track the current cards for the final total
		score += copies;

		//Update the record of bonus cards
		for (int i = 0; i < matches; i++)
		{
			if (i >= bonuses.size())
				bonuses.push_back(0);

			bonuses[i] += copies;
		}
	}

	std::cout << "Total score: " << score << std::endl;
	exit(0);
}

std::set<int> CreateWinningSet(std::string nums)
{
	std::set<int> result = std::set<int>();

	while (!nums.empty())
	{
		result.insert(std::stoi(nums));	//Read the current number into the set

		//Remove the current number to prepare the next one
		int space = nums.find(' ');
		if (space == std::string::npos)
			break;	//If there is no space, we have done the last number

		nums.erase(0, space + 1);

		//Keep erasing while our next char is a space, in case of any extra spacing
		while (nums[0] == ' ')
			nums.erase(0, 1);
	}

	return result;
}

int CalculateScore(std::set<int> winners, std::string nums)
{
	int result = 0;
	while (!nums.empty())
	{

		if (winners.find(std::stoi(nums)) != winners.end())	//Check if the number is in the winning set
			result++;	//Update the score

		//Remove the current number to prepare the next one
		int space = nums.find(' ');
		if (space == std::string::npos)
			break;	//If there is no space, we have done the last number

		nums.erase(0, space + 1);

		//Keep erasing while our next char is a space, in case of any extra spacing
		while (nums[0] == ' ')
			nums.erase(0, 1);
	}

	return result;
}