#include <iostream>
#include <fstream>
#include <string>
#include <forward_list>

bool IsClosingBracket(char bracket);
bool IsCloseValid(std::string openingBrackets, char closingBracket);
uint64_t CalculateCompletionScore(std::string chunks);

int main()
{
	std::fstream input;
	//input.open("Test Input.txt");
	input.open("Day 10 Input.txt");
	if (!input)
	{
		std::cout << "ERROR: Couldn't open file" << std::endl;
		return 1;
	}

	std::forward_list<uint64_t> scores;
	while (!input.eof())
	{
		std::string line;
		input >> line;

		std::string chunk;
		for (int i = 0; i < line.size(); i++)
		{
			char currentChar = line[i];
			//Check if the bracket is opening or closing a chunk
			if (IsClosingBracket(currentChar))
			{
				//Check if we are closing a valid chunk
				if (IsCloseValid(chunk, currentChar))
				{
					//Remove last element of string, closing the chunk
					chunk.pop_back();
				}
				else
				{
					//We have an invalid chunk. Discard the line and move on
					line.clear();
					chunk.clear();
					break;
				}
			}
			else
			{
				//Open a new chunk
				chunk.push_back(currentChar);
			}
		}

		//If chunk is empty, the line was either corrupt or complete. Either way, ignore it and go to next line
		if (chunk.empty())
			continue;

		//If we reach here, we have a non-corrupt, incomplete line. Calculate the auto-complete score and record it
		uint64_t currentScore = CalculateCompletionScore(chunk);
		scores.push_front(currentScore);
	}

	//We have all our socres. Sort the list and find the middle element to get our final score
	scores.sort();
	auto start = scores.begin();
	auto end = scores.end();
	int size = std::distance(start, end);
	int middle = size / 2;
	std::advance(start, middle);

	std::cout << "Completion Score: " << *start << std::endl;
	return 0;
}

bool IsClosingBracket(char bracket)
{
	//If the current char is one of 4 closing brackets, return true. Any other char returns false (should only be opening brackets)
	if (bracket == ')' || bracket == ']' || bracket == '}' || bracket == '>')
		return true;

	return false;
}

//Attempt to close the currently open chunk. Returns the success state of the closing operation. A false result means the chunk was corrupt.
bool IsCloseValid(std::string openingBrackets, char closingBracket)
{
	//Get the index of the last char in the string and ensure it is valid. Otherwise, the string is empty and the chunk is corrupt.
	int lastIndex = openingBrackets.size() - 1;
	if (lastIndex < 0)
		return false;

	//Determine the opening brakcet that matches the current closing bracket
	char matchingBracket;
	if (closingBracket == ')')
		matchingBracket = '(';
	else if (closingBracket == ']')
		matchingBracket = '[';
	else if (closingBracket == '}')
		matchingBracket = '{';
	else if (closingBracket == '>')
		matchingBracket = '<';
	else
		abort();

	if (openingBrackets[lastIndex] == matchingBracket)
		return true;

	return false;
}

uint64_t CalculateCompletionScore(std::string chunks)
{
	const int roundScore = 1;
	const int squareScore = 2;
	const int curlyScore = 3;
	const int pointyScore = 4;

	uint64_t score = 0;

	while (!chunks.empty())
	{
		//Multiply score by 5 for each new character
		score *= 5;

		//Add the score for each character, pulling from the back of the string
		char currentBracket = chunks.back();
		if (currentBracket == '(')
			score += roundScore;
		else if (currentBracket == '[')
			score += squareScore;
		else if (currentBracket == '{')
			score += curlyScore;
		else if (currentBracket == '<')
			score += pointyScore;
		else
			abort();

		//We have the score for the current chunk, now close it
		chunks.pop_back();
	}

	return score;
}