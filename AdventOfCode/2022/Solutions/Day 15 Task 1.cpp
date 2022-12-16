#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <set>

typedef std::pair<int, int> Coords;

struct CoordsCompare
{
	bool operator()(const Coords lhs, const Coords rhs)
	{
		if (lhs.first == rhs.first)
			return lhs.second < rhs.second;
		else
			return lhs.first < rhs.first;
	}
};

Coords GetImpossibleRange(const Coords sensor, const Coords beacon, const int target);
int CountImpossibleSpaces(std::list<Coords> ranges);

int main()
{
	//Create an input stream and open the file
	const int day = 15;
	const int task = 1;

	std::string fileName;
	fileName.append("2022/Inputs/D");
	fileName.append(std::to_string(day));
	fileName.append("T");
	fileName.append(std::to_string(task));
	fileName.append(".txt");
	std::ifstream input;
	input.open(fileName);

	//Check the input stream is valid
	if (!input)
	{
		std::cout << "Unable to open file" << std::endl;
		exit(1);
	}

	std::list<Coords> ranges;
	std::set<Coords, CoordsCompare> beaconsAtTarget;
	std::set<Coords> orderedRanges;
	//const int targetRow = 10;
	const int targetRow = 2000000;
	while (!input.eof())
	{
		if (input.peek() != 'S')	//Make sure we have a valid line
			break;

		Coords sensorPos;
		Coords beaconPos;
		input.ignore(256, '=');
		input >> sensorPos.first;
		input.ignore(256, '=');
		input >> sensorPos.second;

		input.ignore(256, '=');
		input >> beaconPos.first;
		input.ignore(256, '=');
		input >> beaconPos.second;
		input.ignore(256, '\n');

		Coords temp;
		temp = GetImpossibleRange(sensorPos, beaconPos, targetRow);

		if(temp == Coords(INT_MAX, INT_MIN))
			continue;

		ranges.push_back(temp);
		orderedRanges.insert(temp);

		//Track all unique beacons on the target row, as these must be removed from the total of impossible spaces
		if (beaconPos.second == targetRow)
			beaconsAtTarget.insert(beaconPos);
	}

	int spaces = CountImpossibleSpaces(ranges);
	std::cout << "Number of spaces where a beacon is impossible: " << spaces - (int)beaconsAtTarget.size() << std::endl;
	exit(0);
}

Coords GetImpossibleRange(const Coords sensor, const Coords beacon, const int target)
{
	Coords result;
	int xToBeacon = abs(beacon.first - sensor.first);
	int yToBeacon = abs(beacon.second - sensor.second);

	int distToBeacon = xToBeacon + yToBeacon;
	int distToRow = abs(target - sensor.second);

	if (distToBeacon < distToRow)
		return Coords(INT_MAX, INT_MIN);

	int offset = abs(distToBeacon - distToRow);
	result.first = sensor.first - offset;
	result.second = sensor.first + offset;

	if (result.first < -638849 || result.second > 4401794)
		std::cout << "ERROR" << std::endl;

	return result;
}

int CountImpossibleSpaces(std::list<Coords> ranges)
{
	int result = 0;
	std::list<Coords> superRanges;
	bool foundOverlap = true;

	while (foundOverlap == true)
	{
		foundOverlap = false;
		while (!ranges.empty())
		{
			Coords current = ranges.front();
			ranges.pop_front();
			std::list<Coords> redundant;

			for (Coords other : ranges)
			{
				//If the other is fully enclosed in current, mark it as redundant
				if (other.first >= current.first && other.first <= current.second && other.second >= current.first && other.second <= current.second)
				{
					redundant.push_back(other);
					foundOverlap = true;
					continue;
				}

				//If the current first is inside the other range, expand current to enclose other and mark other as redundant
				if (current.first >= other.first && current.first <= other.second)
				{
					current.first = other.first;
					foundOverlap = true;
					redundant.push_back(other);
				}

				//As above, but for the current second
				if (current.second <= other.second && current.second >= other.first)
				{
					current.second = other.second;
					foundOverlap = true;
					redundant.push_back(other);
				}
			}

			//Remove any redundant ranges from the list
			while (!redundant.empty())
			{
				ranges.remove(redundant.front());
				redundant.pop_front();
			}

			superRanges.push_back(current);
		}
		ranges = superRanges;
		superRanges.clear();
	}

	for (Coords current : ranges)
	{
		int score = abs(current.second - current.first) + 1;
		result += score;
	}

	return result;
}