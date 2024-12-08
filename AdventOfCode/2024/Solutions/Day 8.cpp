#include <numeric>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <set>

using Coords = std::pair<int, int>;
using Antenna = std::pair<char, Coords>;

std::set<Coords> CalculateShortAntinodes(std::multimap<char, Coords> antennas, std::set<char> frequencies, const Coords bounds);
std::set<Coords> CalculateLongAntinodes(std::multimap<char, Coords> antennas, std::set<char> frequencies, const Coords bounds);
Coords CalculateDifference(Coords antA, Coords antB);
Coords Simplify2DVector(Coords vec);
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

	std::set<Coords> antinodes = CalculateShortAntinodes(antennas, frequencies, bounds);
	std::set<Coords> resonateAntinodes = CalculateLongAntinodes(antennas, frequencies, bounds);

	int numAntinodes = antinodes.size();
	int numResAntinodes = resonateAntinodes.size();

	std::cout << "Positions containing simple antinodes: : " << numAntinodes << std::endl;
	std::cout << "Positions containing resonate antinodes: : " << numResAntinodes << std::endl;
	exit(0);
}

std::set<Coords> CalculateShortAntinodes(std::multimap<char, Coords> antennas, std::set<char> frequencies, const Coords bounds)
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

std::set<Coords> CalculateLongAntinodes(std::multimap<char, Coords> antennas, std::set<char> frequencies, const Coords bounds)
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
				Coords diff = CalculateDifference(antA->second, antB->second);
				diff = Simplify2DVector(diff);	//Simplify the vector down to its shortest form

				//Start at antA, and move backwards along the vector until we go out of bounds
				Coords node = antA->second;
				while (IsInBounds(node, bounds))
				{
					node.first -= diff.first;
					node.second -= diff.second;
				}

				//Node is now out of bounds, so move it once to come back in bounds
				node.first += diff.first;
				node.second += diff.second;

				//Node is now on the first valid space for this pair of antennas
				//Note its position while continuously moving along the vector until we go out of bounds on the other side
				while (IsInBounds(node, bounds))
				{
					antinodes.insert(node);

					node.first += diff.first;
					node.second += diff.second;
				}
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

Coords Simplify2DVector(Coords vec)
{
	int x = vec.first;
	int y = vec.second;
	int denom = std::gcd(abs(x), abs(y));

	x = x / denom;
	y = y / denom;

	return Coords(x, y);
}

bool IsInBounds(const Coords pos, const Coords bounds)
{
	if (pos.first < 0 || pos.second < 0)
		return false;

	if (pos.first > bounds.first || pos.second > bounds.second)
		return false;

	return true;
}