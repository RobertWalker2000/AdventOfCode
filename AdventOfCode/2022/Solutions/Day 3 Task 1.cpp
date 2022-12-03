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
		std::vector<char> rucksack;
		char currentItem;

		//Read in all items in 1 rucksack
		while (true)
		{
			currentItem = input.get();	//Read in the next character

			if (currentItem < 'A' || currentItem > 'z')	//Check that the character is a letter. If not, exit the loop and move on to the next operation
				break;

			//Push valid letters into the vector
			rucksack.push_back(currentItem);
		}

		//If the rucksack is empty, skip it
		if (rucksack.size() <= 0)
			continue;

		//Find the first item in the 2nd compartment as a midpoint
		int midpoint = (rucksack.size() / 2);

		//Split the rucksack into 2 compartments, eliminating any duplicates
		std::set<char> comp1;
		std::set<char> comp2;
		for (int i = 0; i < midpoint; i++)
		{
			comp1.insert(rucksack[i]);
			comp2.insert(rucksack[midpoint + i]);
		}


		//Now that we have the list of unique item types in each compartment, lets compare them to find the duplicate
		//Inserting the elements from comp1 into comp2 and checking the value returned by the set, we can find the item present in both
		std::set<char>::iterator it;
		char duplicateItem;
		for (it = comp1.begin(); it != comp1.end(); it++)
		{
			if (!comp2.insert(*it).second)	//insert().second returns true if a new element was created, or false if the item was a duplicate
			{
				duplicateItem = *it;
				break;
			}
		}

		//Calculate the priority of the item
		if (duplicateItem < 'a')
			duplicateItem -= 38;	//Convert uppercase letters(65-90) to values between 27 and 52
		else
			duplicateItem -= 96;	//Convert lowercase letters(97-122) to values between 1 and 26

		//Add the priority of the duplicate to the total score
		score += duplicateItem;
	}

	std::cout << "Total score: " << score << std::endl;
	exit(0);
}
