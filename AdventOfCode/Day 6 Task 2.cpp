#include <iostream>
#include <fstream>
#include <vector>

//This solution is a more efficient implementation than the one used in task 1
//An array is used to track how many fish have a given timer length, treating them as a group rather than individuals
//This allows the calculatiuon to be performed in O(n) time rather than 0(n^2) in regards to number of days
//The calculation is also O(1) in relation to number of fish, while the reading from file remains O(n)

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
	const int numDays = 256;
	const int resetTimer = 6;
	const int newTimer = 8;
	int64_t fish[newTimer + 1] = { 0 };

	while (!file.eof())
	{
		int tempFish;
		char fishChar[3];

		file.getline(fishChar, 256, ',');
		tempFish = atoi(fishChar);

		fish[tempFish] = fish[tempFish] + 1;
	}
	file.close();

	for (int day = 0; day < numDays; day++)
	{
		//Used to track how many fish give birth
		//Used to update timers of 8 and 6 after all else is handled
		int64_t fishToReset;

		fishToReset = fish[0];

		//Shift all values down the array by 1, effectiveley reducing each fish's timer
		for (int i = 0; i < newTimer; i++)
			fish[i] = fish[i + 1];

		//Set the number of new fish based on the fishToReset, and also add it on to the fish at the reset timer
		//Must add instead of replace as some new-ish fish will already be there that also need ot be counted
		fish[newTimer] = fishToReset;
		fish[resetTimer] = fish[resetTimer] + fishToReset;
	}
	
	//Count up the total number of fish
	int64_t totalFish = 0;
	for (int i = 0; i < newTimer + 1; i++)
		totalFish = totalFish + fish[i];

	std::cout << "After " << numDays << " days there will be " << totalFish << " lantern fish" << std::endl;

	return 0;
}