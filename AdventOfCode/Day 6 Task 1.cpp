#include <iostream>
#include <fstream>
#include <vector>

//This solution is very inefficient for large data sats/timespans
//Task 2 was going to take way too long to finish, so a more efficient solution was implemented in the task 2 file

int main()
{
	std::fstream file;
	file.open("Day 6 Input.txt");
	//file.open("Test Input.txt");

	if (!file)
	{
		std::cout << "ERROR: Couldn't open file" << std::endl;
		return 1;
	}

	std::vector<int> fish;


	while (!file.eof())
	{
		int tempFish;
		char fishChar[3];

		file.getline(fishChar, 256, ',');
		tempFish = atoi(fishChar);

		fish.push_back(tempFish);
	}

	const int numDays = 80;
	const int resetTimer = 6;
	const int newTimer = 8;
	for (int day = 0; day < numDays; day++)
	{
		const int fishToCheck = fish.size();
		for (int i = 0; i < fishToCheck; i++)
		{
			if (fish[i] < 0)	//Eroor checking
			{
				std::cout << "ERROR: Fish timer reached below 0" << std::endl;
				return 2;
			}

			if (fish[i] > 0)	//Reduce timer on non-zero fish
			{
				fish[i] = fish[i] - 1;
				continue;
			}

			//Any fish that reach herehave a timer of 0, reset and spawn new fish
			fish[i] = resetTimer;
			fish.push_back(newTimer);
		}
	}

	std::cout << "After " << numDays << " there will be " << fish.size() << " lantern fish" << std::endl;

	return 0;
}