#include <iostream>
#include <fstream>
#include <string>

bool IsClosingBracket(char bracket);
bool IsCloseValid(std::string openingBrackets, char closingBracket);
int CalculateErrorScore(char error);

int main()
{
	std::fstream input;
	input.open("Day 10 Input.txt");
	if (!input)
	{
		std::cout << "ERROR: Couldn't open file" << std::endl;
		return 1;
	}

	int runningTotal = 0;
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
					//We have an invalid chunk. Track the error score and go to the next line
					runningTotal += CalculateErrorScore(currentChar);
					break;
				}
			}
			else
			{
				//Open a new chunk
				chunk.push_back(currentChar);
			}
		}
	}

	std::cout << "Total error score: " << runningTotal << std::endl;
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

int CalculateErrorScore(char error)
{
	const int roundScore = 3;
	const int squareScore = 57;
	const int curlyScore = 1197;
	const int pointyScore = 25137;

	if (error == ')')
		return roundScore;
	else if (error == ']')
		return squareScore;
	else if (error == '}')
		return curlyScore;
	else if (error == '>')
		return pointyScore;
	else
		abort();
}