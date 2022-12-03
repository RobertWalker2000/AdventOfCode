#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>

int main()
{
	//Create an input stream and open the file
	const int day = 3;
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

	int score = 0;
	while (!input.eof())
	{
		//Use a set for the rucksacks now as we only care about which items are duplicates between separate bags
		//We don't need to retain the total size of the bags for comparing compartments any more
		std::set<char> rucksacks[3];
		char currentItem;

		//Read in 3 rucksacks at a time (1 full group)
		for (int i = 0; i < 3; i++)
		{
			//Read in all items in 1 rucksack
			while (true)
			{
				currentItem = input.get();	//Read in the next character

				if (currentItem < 'A' || currentItem > 'z')	//Check that the character is a letter. If not, exit the loop and move on to the next operation
					break;

				//Push valid letters into the vector
				rucksacks[i].insert(currentItem);
			}
		}

		//If the final rucksack is empty, we have likely reached the end
		if (rucksacks[2].size() <= 0)
			continue;

		//Find a list of all items that are shared between rucksacks 0 and 1
		std::set<char> commonPair;
		std::set<char>::iterator it;
		for (it = rucksacks[0].begin(); it != rucksacks[0].end(); it++)
		{
			if (!rucksacks[1].insert(*it).second)	//insert().second returns false if the item already exists in the set
				commonPair.insert(*it);	//Make note of all items which are duplicates between bags 0 and 1
		}

		//Perform the same comparison as above, but between bag 2 and the list of common items, noting the item found in all 3 bags
		char badge;
		for (it = commonPair.begin(); it != commonPair.end(); it++)
		{
			if (!rucksacks[2].insert(*it).second)
			{
				badge = *it;
				break;
			}
		}

		//Calculate the priority of the item
		if (badge < 'a')
			badge -= 38;	//Convert uppercase letters(65-90) to values between 27 and 52
		else
			badge -= 96;	//Convert lowercase letters(97-122) to values between 1 and 26

		//Add the priority of the duplicate to the total score
		score += badge;
	}

	std::cout << "Total score: " << score << std::endl;
	exit(0);
}
