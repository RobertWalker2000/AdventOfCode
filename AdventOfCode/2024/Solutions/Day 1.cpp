#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

int main()
{
	//Create an input stream and open the file
	const int day = 1;

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

	std::vector<int> listA;
	std::vector<int> listB;

	while (!input.eof())
	{
		int num = 0;

		input >> num;
		listA.push_back(num);

		input >> num;
		listB.push_back(num);
	}

	std::sort(listA.begin(), listA.end());
	std::sort(listB.begin(), listB.end());

	int diff = 0;
	for (int i = 0; i < listA.size(); i++)
	{
		diff += std::abs(listA.at(i) - listB.at(i));
	}

	int sim = 0;
	while (!listA.empty())
	{
		//Get the last element in list A, and remove it
		int numA = listA.back();

		int reps = 0;
		while (!listA.empty())
		{
			if (listA.back() == numA)
			{
				reps++;
				listA.pop_back();
				continue;
			}

			break;
		}

		//Loop through listB, removing elements as we go. This prevents excessive checking of redundant data.
		//Track how many times we match numA, and stop when we get below it
		int count = 0;
		while (!listB.empty())
		{
			int numB = listB.back();

			//If we have gone past numA, leave the loop and get the next item from listA
			if (numB < numA)
				break;

			//If we match numA, add it to the count
			if (numB == numA)
				count++;

			//Whether it is a match or not, remove it form the list
			listB.pop_back();
		}

		//We have found all matches for this value of listA. Add the value to the total
		sim += numA * count * reps;
	}

	std::cout << "Total distance: " << diff << std::endl;
	std::cout << "Total similarity: " << sim << std::endl;
	exit(0);
}
