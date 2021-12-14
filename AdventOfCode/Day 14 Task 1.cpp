#include <iostream>
#include <fstream>
#include <vector>

struct Pair
{
	char left;
	char right;
	char insertion;

	bool IsMatch(char atoms[3])
	{
		if (left == atoms[0] && right == atoms[1])
			return true;

		return false;
	}

	void Print()
	{
		std::cout << left << right << " -> " << insertion << std::endl;
	}
};

int CalculateOccurences(std::vector<char> chain, char letter);

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

	//Read in chain
	std::vector<char> chain;
	while (true)
	{
		char currentChar = input.get();

		if (currentChar == '\n')
			break;

		chain.push_back(currentChar);
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

	//Perform chain insertions
	const int numLoops = 10;
	for (int step = 0; step < numLoops; step++)
	{
		std::vector<char> tempChain;
		for (int i = 0; i < chain.size() - 1; i++)
		{
			//Push the current atom onto the new chain
			tempChain.push_back(chain[i]);

			//Check the current and next atom to form a pair
			char currentPair[3];
			currentPair[0] = chain[i];
			currentPair[1] = chain[i + 1];

			//Check all possible pairs to see if we need to insert something into the chain
			for (int j = 0; j < pairs.size(); j++)
			{
				if (pairs[j].IsMatch(currentPair))
				{
					//When we find a pair, push the insertion onto the chain
					tempChain.push_back(pairs[j].insertion);
					break;
				}
			}
		}
		//Push the last atom onto the chain, as this is never checked in the above loop
		tempChain.push_back(chain[chain.size() - 1]);

		//Copy the tempChain onto the main chain, leaving us with the new chain after insertions
		chain = tempChain;
	}

	//Find most common and least common atoms
	int occurences[26];

	int biggestIndex = 0, smallestIndex = 0;
	for (int i = 0; i < 26; i++)
	{
		char currentLetter = 'A' + i;
		occurences[i] = CalculateOccurences(chain, currentLetter);

		if (occurences[i] > occurences[biggestIndex])	//Track the most common occurence
			biggestIndex = i;

		if (occurences[smallestIndex] == 0)	//Always take the newest value if smallest is still 0, to ensure we get an actual value to measure against
			smallestIndex = i;
		else if (occurences[i] < occurences[smallestIndex] && occurences[i] != 0)	//Track the rarest non-zero occurence
			smallestIndex = i;
	}

	std::cout << "Rarest occurence: " << (char)('A' + smallestIndex) << " = " << occurences[smallestIndex] << std::endl;
	std::cout << "Most common occurence: " << (char)('A' + biggestIndex) << " = " << occurences[biggestIndex] << std::endl;

	int result = occurences[biggestIndex] - occurences[smallestIndex];
	std::cout << "Final Result: " << result << std::endl;
	return 0;
}

int CalculateOccurences(std::vector<char> chain, char letter)
{
	int frequency = 0;
	for (int i = 0; i < chain.size(); i++)
	{
		if (chain[i] == letter)
			frequency++;
	}
	return frequency;
}