#include <iostream>
#include <fstream>
#include <vector>

int FindFuelCost(std::vector<int>* crabs, int position);

int main()
{
	std::fstream input;
	input.open("Day 7 Input.txt");
	//input.open("Test Input.txt");

	if (!input)
	{
		std::cout << "ERROR: Couldn't open file" << std::endl;
		return 1;
	}

	std::vector<int> positions;
	while (!input.eof())
	{
		char tempChar[10];
		int tempPos;

		input.getline(tempChar, 256, ',');
		tempPos = atoi(tempChar);
		positions.push_back(tempPos);
	}

	int rightPos = 0;
	int leftPos = positions[0];

	for (int i = 0; i < positions.size(); i++)
	{
		if (positions[i] > rightPos)
			rightPos = positions[i];

		if (positions[i] < leftPos)
			leftPos = positions[i];
	}

	int midPos = (rightPos - leftPos) / 2;
	int leftCost, rightCost, bestCost;
	leftCost = FindFuelCost(&positions, leftPos);
	rightCost = FindFuelCost(&positions, rightPos);

	while (true)
	{
		if (leftCost < rightCost)
		{
			rightPos = midPos;
			int gap = rightPos - leftPos;
			midPos = leftPos + (gap / 2);
		}
		else if (leftCost > rightCost)
		{
			leftPos = midPos;
			int gap = rightPos - leftPos;
			midPos = leftPos + (gap / 2);
		}

		leftCost = FindFuelCost(&positions, leftPos);
		rightCost = FindFuelCost(&positions, rightPos);

		if (leftCost == rightCost)
		{
			if (leftPos == rightPos)
			{
				std::cout << "Found best position" << std::endl;
				bestCost = leftCost;
				break;
			}
			else
			{
				std::cout << "ERROR: Found equal cost on different points" << std::endl;
				return 2;
			}
		}
		else if ((rightPos - leftPos) == 1)
		{
			std::cout << "Found best position" << std::endl;
			if (leftCost < rightCost)
				bestCost = leftCost;
			else
				bestCost = rightCost;

			break;
		}
	}

	std::cout << "The least possible amoutnof fuel used is " << bestCost << " units" << std::endl;

	return 0;
}

//Fueel calculation for task 2
int FindFuelCost(std::vector<int>* crabs, int position)
{
	int fuelCost = 0;
	for (int i = 0; i < crabs->size(); i++)
	{
		//Fuel cost increases for each position you move
		//First space costs 1, 2nd space costs 2, 3rd space costs 3 etc.
		//Should be a formula for this but the loop works
		int distance = abs(crabs->at(i) - position);
		fuelCost += (distance * (distance + 1)) / 2;
	}

	return fuelCost;
}

//Fuel calculation for task 1
//int FindFuelCost(std::vector<int>* crabs, int position)
//{
//	int fuelCost = 0;
//	for (int i = 0; i < crabs->size(); i++)
//	{
//		int distance = abs(crabs->at(i) - position);
//		fuelCost += distance;
//	}
//
//	return fuelCost;
//}