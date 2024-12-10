#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int SumAllPeaks(std::vector<std::string>* map);
int CalculatePeaksFromPoint(std::vector<std::string>* map, int x, int y);

int SumAllTrails(std::vector<std::string>* map);
int CalculateTrailsFromPoint(std::vector<std::string>* map, int x, int y);

int main()
{
	//Create an input stream and open the file
	const int day = 10;

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

	std::vector<std::string> map;
	while (!input.eof())
	{
		char line[256];
		input.getline(line, 256);
		std::string strLine(line);

		if (strLine.empty())
			break;

		//Add a border around the map for easy edge detection
		strLine.append(".");
		strLine.insert(0, ".");
		map.push_back(strLine);
	}

	//Add a border to the top and bottom
	std::string borderLine = "";
	borderLine.append(map.at(0).size(), '.');
	map.insert(map.begin(), borderLine);
	map.push_back(borderLine);

	int possiblePeaks = SumAllPeaks(&map);
	int possibleTrails = SumAllTrails(&map);

	std::cout << "Total number of possible peaks: " << possiblePeaks << std::endl;
	std::cout << "Total number of possible trails: " << possibleTrails << std::endl;

	exit(0);
}

int SumAllPeaks(std::vector<std::string>* map)
{
	int trailCount = 0;

	for (int y = 0; y < map->size(); y++)
	{
		int x = 0;

		while (x < map->at(y).size())
		{
			//Find the next 0 on the line
			x = map->at(y).find('0', x);

			//Ifwe didn't find any 0s, move on to the next line
			if (x == std::string::npos)
				break;

			//We found a 0, so now calculate how many peaks it leads to
			//Create a duplicate of the map so that we can mark off peaks as we go
			std::vector<std::string> dupeMap = *map;
			trailCount += CalculatePeaksFromPoint(&dupeMap, x, y);

			//Increment x so we don't find the same 0 again
			x++;
		}
	}

	return trailCount;
}

int CalculatePeaksFromPoint(std::vector<std::string>* map, int x, int y)
{
	//Calculate the height of the space we are currently on
	char target = map->at(y).at(x);

	//If we are currently on a 9, we are at the top of the trail
	//Mark off the peak and count it
	if (target == '9')
	{
		map->at(y)[x] = '.';
		return 1;
	}

	//If we are not on top of the trail, look for the next step up and check all possibilites
	target++;


	int trails = 0;

	//Check the space above
	int targetY = y - 1;
	int targetX = x;
	if (map->at(targetY).at(targetX) == target)	//The "minus '0'" part converts a char between 0 and 9 to its int equivalent 
		trails += CalculatePeaksFromPoint(map, targetX, targetY);

	//Check the space to the right
	targetY = y;
	targetX = x + 1;
	if (map->at(targetY).at(targetX) == target)
		trails += CalculatePeaksFromPoint(map, targetX, targetY);

	//Check the space below
	targetY = y + 1;
	targetX = x;
	if (map->at(targetY).at(targetX) == target)
		trails += CalculatePeaksFromPoint(map, targetX, targetY);

	//Check the space to the left
	targetY = y;
	targetX = x - 1;
	if (map->at(targetY).at(targetX) == target)
		trails += CalculatePeaksFromPoint(map, targetX, targetY);

	return trails;
}

int SumAllTrails(std::vector<std::string>* map)
{
	int trailCount = 0;

	for (int y = 0; y < map->size(); y++)
	{
		int x = 0;

		while (x < map->at(y).size())
		{
			//Find the next 0 on the line
			x = map->at(y).find('0', x);

			//Ifwe didn't find any 0s, move on to the next line
			if (x == std::string::npos)
				break;

			//If we found a 0, calculate the number of hiking trails it leads to
			trailCount += CalculateTrailsFromPoint(map, x, y);

			//Increment x so we don't find the same 0 again
			x++;
		}
	}

	return trailCount;
}

int CalculateTrailsFromPoint(std::vector<std::string>* map, int x, int y)
{
	//Calculate the height of the space we are currently on
	char target = map->at(y).at(x);

	//If we are currently on a 9, we are at the top of the trail
	if (target == '9')
		return 1;

	//If we are not on top of the trail, look for the next step up and check all possibilites
	target++;


	int trails = 0;

	//Check the space above
	int targetY = y - 1;
	int targetX = x;
	if (map->at(targetY).at(targetX) == target)	//The "minus '0'" part converts a char between 0 and 9 to its int equivalent 
		trails += CalculateTrailsFromPoint(map, targetX, targetY);

	//Check the space to the right
	targetY = y;
	targetX = x + 1;
	if (map->at(targetY).at(targetX) == target)
		trails += CalculateTrailsFromPoint(map, targetX, targetY);

	//Check the space below
	targetY = y + 1;
	targetX = x;
	if (map->at(targetY).at(targetX) == target)
		trails += CalculateTrailsFromPoint(map, targetX, targetY);

	//Check the space to the left
	targetY = y;
	targetX = x - 1;
	if (map->at(targetY).at(targetX) == target)
		trails += CalculateTrailsFromPoint(map, targetX, targetY);

	return trails;
}