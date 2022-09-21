#include <iostream>
#include <fstream>
#include <vector>

#define CharToIndex(x) x - 65

struct Pair
{
	char left;
	char right;
	char insertion;
	uint64_t occurences = 0;

	bool IsMatch(char atoms[3])
	{
		if (left == atoms[0] && right == atoms[1])
			return true;

		return false;
	}

	void Update()
	{
		occurences = newOccurences;
		newOccurences = 0;
	}

	void AddOccurencesToQueue(uint64_t num)
	{
		newOccurences += num;
	}

private:
	uint64_t newOccurences = 0;
};

int main()
{
	std::fstream input;
	//input.open("Test Input.txt");
	input.open("Day 14 Input.txt");
	if (!input)
	{
		std::cout << "ERROR: Couldn't open file" << std::endl;
		return 1;
	}

	//Read in chain and record frequency of each letter
	uint64_t frequency[26] = { 0 };
	std::vector<char> chain;
	while (true)
	{
		char currentChar = input.get();

		if (currentChar == '\n')
			break;

		chain.push_back(currentChar);

		//Add 1 to the count of the appropriate letter
		frequency[CharToIndex(currentChar)]++;
	}

	//Skip the empty line
	input.ignore(256, '\n');

	//Read in pairings
	std::vector<Pair> pairs;
	while (!input.eof())
	{
		Pair newPair;
		newPair.left = input.get();
		newPair.right = input.get();
		
		//Skip the 2 spaces and the arrow
		input.ignore(256, ' ');
		input.ignore(256, ' ');

		newPair.insertion = input.get();
		pairs.push_back(newPair);

		//Go to next line
		input.ignore(256, '\n');
	}

	//Record the frequency of each pair from the starting chain
	for (int i = 0; i < chain.size() - 1; i++)
	{
		char currentPair[3];
		currentPair[0] = chain[i];
		currentPair[1] = chain[i + 1];

		for (int j = 0; j < pairs.size(); j++)
		{
			if (pairs[j].IsMatch(currentPair))
				pairs[j].occurences++;
		}
	}


	//Loop through all existing pairs, calculating the new pairs it will create after insertion, and updating the occurences of these new pairs
	const int numSteps = 40;
	for (int loops = 0; loops < numSteps; loops++)
	{
		for (int i = 0; i < pairs.size(); i++)	//Perform the simultaneous insertions
		{
			//Update the count for each letter as it is inserted into the chain
			uint64_t numOccurences = pairs[i].occurences;
			int charIndex = CharToIndex(pairs[i].insertion);
			frequency[charIndex] += numOccurences;

			//Construct the 2 new pairs that will be created after insertion
			char leftPair[3];
			leftPair[0] = pairs[i].left;
			leftPair[1] = pairs[i].insertion;

			char rightPair[3];
			rightPair[0] = pairs[i].insertion;
			rightPair[1] = pairs[i].right;

			//Find the new pairs and add to their occurences
			for (int j = 0; j < pairs.size(); j++)
			{
				if (pairs[j].IsMatch(leftPair) || pairs[j].IsMatch(rightPair))
				{
					pairs[j].AddOccurencesToQueue(numOccurences);
				}
			}
		}

		//Update the old pair counts to the new pair counts, simulating an insertion
		for (int i = 0; i < pairs.size(); i++)
		{
			pairs[i].Update();
		}
	}

	//Find the most and least common letters
	uint64_t mostCommon = 0;
	uint64_t leastCommon = UINT64_MAX;

	for (int i = 0; i < 26; i++)
	{
		uint64_t occurences = frequency[i];

		//If there are 0 occurences, ignore this letter
		if (occurences == 0)
			continue;

		if (occurences > mostCommon)
			mostCommon = occurences;

		if (occurences < leastCommon)
			leastCommon = occurences;
	}

	uint64_t result = mostCommon - leastCommon;
	std::cout << "Most common: " << mostCommon << std::endl;
	std::cout << "Least common " << leastCommon << std::endl;
	std::cout << "Final result: " << result << std::endl;
	return 0;
}