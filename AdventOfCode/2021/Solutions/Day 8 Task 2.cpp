#include <iostream>
#include <fstream>
#include <vector>

#include <chrono>
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

//Converts a to 0, b to 1 etc to match the below key
#define LetterToNumber(x)(x - 97)

/* 7 segment display key
 aaaa		a = 0		 0000
b	 c		b = 1		1	 2
b	 c		c = 2		1	 2
 dddd		d = 3		 3333
e	 f		e = 4		4	 5
e	 f		f = 5		4	 5
 gggg		g = 6		 6666
*/
int GetNumSegments(char number[10]);
bool IsSignalRegisteredAtSegment(std::vector<int> *signals, int signal, int segment);
bool IsSignalRegistered(std::vector<int> *signals, int signal);
void CreateMasks(int masks[10], std::vector<int> *signals, int loops);
int FindSegmentFromMasks(int masks[10], int signal);

int main()
{
	auto startTime = high_resolution_clock::now();
	std::fstream input;
	input.open("Day 8 Input.txt");
	//input.open("Test Input.txt");

	if (!input)
	{
		std::cout << "ERROR: Couln't open input file" << std::endl;
		return 1;
	}

	int runningTotal = 0;
	int loops = 0;
	while (!input.eof())
	{
		//An array of the signal listings
		//X-value represents the order the signal was output
		//Y-value represents the specific digit within that output
		char display[10][10];

		//Represents the signal-segment relation
		//X-value represents the segment, Y-value represents the signal that turns it on
		//Values start out false and are flipped to true if they are a possible match based on the 4 unique numbers
		//Values are then cross checked against other numbers and turned back to false if they don't match
		//This should leave us with 1 true value for each segment, representing its corresponding signal
		//bool segment[7][7] = { false };

		//Track each value as we figure it out
		int values[10] = { -1 };

		//Holds all the signal-segment relations
		//Index is the display segment, values are possible signals for that segment
		std::vector<int> signals[7];

		//Read in the 10 values
		for (int i = 0; i < 10; i++)
			input.getline(display[i], 256, ' ');


		//Handle the 1-------------------------------------------------------------
		for (int i = 0; i < 10; i++)
		{
			if (GetNumSegments(display[i]) == 2)
			{
				values[i] = 1;
				int val1 = LetterToNumber(display[i][0]);
				int val2 = LetterToNumber(display[i][1]);

				//Track the 2 possible signals for each segment
				signals[2].push_back(val1);
				signals[2].push_back(val2);
				signals[5].push_back(val1);
				signals[5].push_back(val2);
			}
		}

		//Handle the 7-------------------------------------------------------------
		for (int i = 0; i < 10; i++)
		{
			if (GetNumSegments(display[i]) == 3)
			{
				values[i] = 7;
				int vals[3];
				vals[0] = LetterToNumber(display[i][0]);
				vals[1] = LetterToNumber(display[i][1]);
				vals[2] = LetterToNumber(display[i][2]);

				for (int val = 0; val < 3; val++)
				{
					//Check if the current signal is already registers in segment 2
					//If so, ignore it. If not, its the signal for segment 0
					int currentVal = vals[val];
					if (!IsSignalRegistered(signals, currentVal))
					{
						signals[0].push_back(currentVal);
						break;
					}
				}
			}
		}

		//Handle the 4---------------------------------------------------------------
		for (int i = 0; i < 10; i++)
		{
			if (GetNumSegments(display[i]) == 4)
			{
				values[i] = 4;
				int vals[4];
				vals[0] = LetterToNumber(display[i][0]);
				vals[1] = LetterToNumber(display[i][1]);
				vals[2] = LetterToNumber(display[i][2]);
				vals[3] = LetterToNumber(display[i][3]);

				for (int val = 0; val < 4; val++)
				{
					//Check if the current signal is already registers in segment 2
					//If so, ignore it. If not, its the signal for segment 1 or 3
					int currentVal = vals[val];
					if (!IsSignalRegisteredAtSegment(signals, currentVal, 2))
					{
						signals[1].push_back(currentVal);
						signals[3].push_back(currentVal);
					}
				}
			}
		}

		//Handle the 6-----------------------------------------------------------------
		for (int i = 0; i < 10; i++)
		{
			if (GetNumSegments(display[i]) == 6)
			{
				//Loop through all signals in this number
				//If we find a number with only one signal matching segment 2, its 6
				int matches = 0;
				int matchingSignal = -1;
				for (int j = 0; j < 6; j++)
				{
					int currentSignal = LetterToNumber(display[i][j]);
					if (IsSignalRegisteredAtSegment(signals, currentSignal, 2))
					{
						matches++;
						matchingSignal = currentSignal;
					}
				}

				if (matches == 1)	//Exactly 1 match means 6
				{
					values[i] = 6;

					//The only matching segment between 6 and 1 is segment 5
					//So set the matching signal to be the one for seg 5, and remove it from 2
					//This leaves us with guaranteed values for segments 2 and 5
					signals[5].clear();
					signals[5].push_back(matchingSignal);

					if (signals[2].at(0) == matchingSignal)
						signals[2].erase(signals[2].begin());
					else
						signals[2].erase(signals[2].begin() + 1);

					//Find the 2 new signals
					bool firstSignal = true;
					int newSignal1 = 0, newSignal2 = 0;
					for (int j = 0; j < 6; j++)
					{
						int currentSignal = LetterToNumber(display[i][j]);
						if (!IsSignalRegistered(signals, currentSignal))
						{
							if (firstSignal)
							{
								newSignal1 = currentSignal;
								firstSignal = false;
							}
							else
							{
								newSignal2 = currentSignal;
								break;
							}
						}
					}

					//Register the new signals in segments 4 and 6, as all other segments for displaying 6 already have values
					signals[4].push_back(newSignal1);
					signals[4].push_back(newSignal2);
					signals[6].push_back(newSignal1);
					signals[6].push_back(newSignal2);

					break;
				}
			}
		}

		//Handle the 9-----------------------------------------------
		for (int i = 0; i < 10; i++)
		{
			//Find the numbers with 6 segments, not including 6
			//This should give us 0 and 9
			if (GetNumSegments(display[i]) == 6 & values[i] != 6)
			{
				//Check how many matches we get out of segment 6's possibilities
				//If it's 1 match, we found 9. 2 matches means it's 0
				int matches = 0;
				int matchingSignal = -1;
				for (int j = 0; j < 6; j++)
				{
					int currentSignal = LetterToNumber(display[i][j]);
					if (IsSignalRegisteredAtSegment(signals, currentSignal, 6))
					{
						matches++;
						matchingSignal = currentSignal;
					}
				}

				//If we don't have exactly 1 match then it's not number 9, try again
				if (matches != 1)
					continue;

				//If we reach here, we found number 9 and the matching signal is the correct one for segmnent 6
				//Register the position of 9
				values[i] = 9;

				//Set segment 6 to use the matching signal
				signals[6].clear();
				signals[6].push_back(matchingSignal);
				//Set segment 4 to use the non-matching signal
				if (signals[4].at(0) == matchingSignal)
					signals[4].erase(signals[4].begin());
				else
					signals[4].erase(signals[4].begin() + 1);

				break;
			}
		}

		//Handle the 0---------------------------------------------------------
		for (int i = 0; i < 10; i++)
		{
			//Find the last 6 segment number that we have, which will be 0
			if (GetNumSegments(display[i]) == 6 & values[i] != 6 && values[i] != 9)
			{
				int matchingSignal = -1;
				for (int j = 0; j < 6; j++)
				{
					int currentSignal = LetterToNumber(display[i][j]);
					if (IsSignalRegisteredAtSegment(signals, currentSignal, 1))
					{
						matchingSignal = currentSignal;
						break;
					}
				}

				//We have the matching signal, i.e. the signal for segment 1
				//Use this to update segments 1 and 3, completing our translation set
				signals[1].clear();
				signals[1].push_back(matchingSignal);

				if (signals[3].at(0) == matchingSignal)
					signals[3].erase(signals[3].begin());
				else
					signals[3].erase(signals[3].begin() + 1);
			}
		}

		//Compile a set of masks which can be used to calculate the output values
		int mask[10] = { 0 };
		CreateMasks(mask, signals, loops);

		//Skip ahead to the lines output
		input.ignore(256, ' ');

		char fullOutput[5];
		for (int value = 0; value < 3; value++)
		{
			char currentOutput[10];
			input.getline(currentOutput, 256, ' ');
			int currentValue = 0;
			int numSignals = GetNumSegments(currentOutput);
			for (int i = 0; i < numSignals; i++)
			{
				int currentSignal = LetterToNumber(currentOutput[i]);
				currentValue += 1 << currentSignal;
			}
			fullOutput[value] = FindSegmentFromMasks(mask, currentValue) + 48;	//Adding 48 converts from the number 0 to the character 0, same for other numbers
		}

		//Last run doesn't use a delimiter as it finds the end of the line instead
		char output[10];
		input.getline(output, 256);
		int currentValue = 0;
		int numSignals = GetNumSegments(output);
		for (int i = 0; i < numSignals; i++)
		{
			int currentSignal = LetterToNumber(output[i]);
			currentValue += 1 << currentSignal;
		}
		fullOutput[3] = FindSegmentFromMasks(mask, currentValue) + 48;

		runningTotal += atoi(fullOutput);
		loops++;
	}
	auto endTime = high_resolution_clock::now();
	auto timeTaken = duration_cast<milliseconds>(endTime - startTime);

	std::cout << "Final output: " << runningTotal << std::endl;
	std::cout << "Time taken to compute answer: " << timeTaken.count() << "ms" << std::endl;
	return 0;
}

int GetNumSegments(char number[10])
{
	int segments = 0;
	for (segments = 0; segments < 10; segments++)
	{
		if (number[segments] == 0)
			break;
	}

	return segments;
}

//Check if a given signal is an existing possibility for a given segment
bool IsSignalRegisteredAtSegment(std::vector<int> *signals, int signal, int segment)
{
	for (int i = 0; i < signals[segment].size(); i++)
	{
		if (signals[segment].at(i) == signal)
			return true;
	}

	return false;
}

//Check if a given signal has been registered at all
bool IsSignalRegistered(std::vector<int> *signals, int signal)
{
	for (int i = 0; i < 7; i++)
	{
		if (IsSignalRegisteredAtSegment(signals, signal, i))
			return true;
	}

	return false;
}

int FindSegmentFromMasks(int masks[10], int signal)
{
	for (int i = 0; i < 10; i++)
	{
		if (signal == masks[i])
			return i;
	}

	return -1;
}

void CreateMasks(int mask[10], std::vector<int> *signals, int loops)
{
	mask[0] += 1 << signals[0].at(0);
	mask[0] += 1 << signals[1].at(0);
	mask[0] += 1 << signals[2].at(0);
	mask[0] += 1 << signals[4].at(0);
	mask[0] += 1 << signals[5].at(0);
	mask[0] += 1 << signals[6].at(0);

	mask[1] += 1 << signals[2].at(0);
	mask[1] += 1 << signals[5].at(0);

	mask[2] += 1 << signals[0].at(0);
	mask[2] += 1 << signals[2].at(0);
	mask[2] += 1 << signals[3].at(0);
	mask[2] += 1 << signals[4].at(0);
	mask[2] += 1 << signals[6].at(0);

	mask[3] += 1 << signals[0].at(0);
	mask[3] += 1 << signals[2].at(0);
	mask[3] += 1 << signals[3].at(0);
	mask[3] += 1 << signals[5].at(0);
	mask[3] += 1 << signals[6].at(0);

	mask[4] += 1 << signals[1].at(0);
	mask[4] += 1 << signals[2].at(0);
	mask[4] += 1 << signals[3].at(0);
	mask[4] += 1 << signals[5].at(0);

	mask[5] += 1 << signals[0].at(0);
	mask[5] += 1 << signals[1].at(0);
	mask[5] += 1 << signals[3].at(0);
	mask[5] += 1 << signals[5].at(0);
	mask[5] += 1 << signals[6].at(0);

	mask[6] += 1 << signals[0].at(0);
	mask[6] += 1 << signals[1].at(0);
	mask[6] += 1 << signals[3].at(0);
	mask[6] += 1 << signals[4].at(0);
	mask[6] += 1 << signals[5].at(0);
	mask[6] += 1 << signals[6].at(0);

	mask[7] += 1 << signals[0].at(0);
	mask[7] += 1 << signals[2].at(0);
	mask[7] += 1 << signals[5].at(0);

	mask[8] += 1 << signals[0].at(0);
	mask[8] += 1 << signals[1].at(0);
	mask[8] += 1 << signals[2].at(0);
	mask[8] += 1 << signals[3].at(0);
	mask[8] += 1 << signals[4].at(0);
	mask[8] += 1 << signals[5].at(0);
	mask[8] += 1 << signals[6].at(0);

	mask[9] += 1 << signals[0].at(0);
	mask[9] += 1 << signals[1].at(0);
	mask[9] += 1 << signals[2].at(0);
	mask[9] += 1 << signals[3].at(0);
	mask[9] += 1 << signals[5].at(0);
	mask[9] += 1 << signals[6].at(0);
}