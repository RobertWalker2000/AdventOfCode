#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <set>

using Coords = std::pair<int, int>;
using Antenna = std::pair<char, Coords>;

std::set<Coords> CalculateAntinodes(std::multimap<char, Coords> antennas, std::set<char> frequencies, const Coords bounds);
Coords CalculateDifference(Coords antA, Coords antB);
bool IsInBounds(Coords pos, Coords bounds);

int main()
{
	//Create an input stream and open the file
	const int day = 8;

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

	std::multimap<char, Coords> antennas;
	std::set<char> frequencies;
	int y = 0;
	int lineLength = 0;
	while (!input.eof())
	{
		char temp[256];
		input.getline(temp, 256);
		std::string line(temp);
		if (line.empty())
			break;

		lineLength = line.size();
		for (int x = 0; x < lineLength; x++)
		{
			if (line[x] != '.')	//Anything that is not a '.' is an antenna. Note its frequency and position
			{
				antennas.insert(Antenna(line[x], Coords(x, y)));
				frequencies.insert(line[x]);
			}
		}

		//Increment Y before we loop again, as we are going to the next line
		y++;
	}

	//Create a coordinate representing the bottom-right-most valid coordinate
	Coords bounds(lineLength - 1, y - 1);

	std::set<Coords> antinodes = CalculateAntinodes(antennas, frequencies, bounds);

	int numAntinodes = antinodes.size();

	std::cout << "Positions containing antinodes: : " << numAntinodes << std::endl;
	exit(0);
}

std::set<Coords> CalculateAntinodes(std::multimap<char, Coords> antennas, std::set<char> frequencies, const Coords bounds)
{
	//Create a setto store all the unique antinodes
	std::set<Coords> antinodes;

	//Loop through all the frequencies, checking for all antennas of that type
	for (std::set<char>::iterator freq = frequencies.begin(); freq != frequencies.end(); freq++)
	{
		std::pair<std::multimap<char, Coords>::iterator, std::multimap<char, Coords>::iterator>  freqRange = antennas.equal_range(*freq);
		//Loop through all the antennas, checking for antinodes with each subsequent antenna of the same type
		for (std::multimap<char, Coords>::iterator antA = freqRange.first; antA != freqRange.second; antA++)
		{
			//Initialise antB to equal antA, and then increment it at the entry to the loop so we are starting with them offset by 1
			std::multimap<char, Coords>::iterator antB = antA;
			for (antB++; antB != freqRange.second; antB++)
			{
				//Calculate the offset between the antennas and a potential antinode
				Coords halfDiff = CalculateDifference(antA->second, antB->second);

				//Calculate the first antinode, and store it if valid
				Coords node = Coords(antB->second.first + halfDiff.first, antB->second.second + halfDiff.second);
				if (IsInBounds(node, bounds))
					antinodes.insert(node);

				//Now check the opposite antinode position
				node = Coords(antA->second.first - halfDiff.first, antA->second.second - halfDiff.second);
				if (IsInBounds(node, bounds))
					antinodes.insert(node);
			}
		}
	}

	return antinodes;
}

Coords CalculateDifference(const Coords antA, const Coords antB)
{
	Coords result(0, 0);

	//Calculate the vector representing the difference from antenna A to antenna B
	result.first = antB.first - antA.first;
	result.second = antB.second - antA.second;

	return result;
}

bool IsInBounds(const Coords pos, const Coords bounds)
{
	if (pos.first < 0 || pos.second < 0)
		return false;

	if (pos.first > bounds.first || pos.second > bounds.second)
		return false;

	return true;
}