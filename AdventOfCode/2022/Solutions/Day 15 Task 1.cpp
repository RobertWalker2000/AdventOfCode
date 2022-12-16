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

//class Sensor
//{
//	Sensor();
//
//	Coords position;
//	Coords beaconPos;
//	int distance = 0;
//
//public:
//	void SetPosition(Coords inPos) { position = inPos; }
//	void SetBeacon(Coords inBeacon) { beaconPos = inBeacon; }
//	bool CalculateDistance()
//	{
//		if (position.first != NULL && position.second != NULL && beaconPos.first != NULL && beaconPos.second != NULL)
//		{
//			int x = abs(beaconPos.first - position.first);
//			int y = abs(beaconPos.second - position.second);
//			distance = x + y;
//			return true;
//		}
//		return false;
//	}
//};

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

		ranges.push_back(GetImpossibleRange(sensorPos, beaconPos, targetRow));

		//Track all unique beacons on the target row, as these must be removed from the total of impossible spaces
		if (beaconPos.second == targetRow)
			beaconsAtTarget.insert(beaconPos);

		input.ignore(256, '\n');
	}

	std::cout << "Number of spaces where a beacon is impossible: " << (CountImpossibleSpaces(ranges) - beaconsAtTarget.size()) << std::endl;
	exit(0);
}

Coords GetImpossibleRange(const Coords sensor, const Coords beacon, const int target)
{
	int distToBeacon = 0;
	distToBeacon += abs(beacon.first - sensor.first);
	distToBeacon += abs(beacon.second - sensor.second);

	int distToTarget = abs(target - sensor.second);
	int offSet = abs(distToBeacon - distToTarget);

	Coords result;
	result.first = sensor.first - offSet;
	result.second = sensor.first + offSet;

	return result;
}

int CountImpossibleSpaces(std::list<Coords> ranges)
{
	int result = 0;

	while (!ranges.empty())
	{
		Coords current = ranges.front();
		ranges.pop_front();
		std::list<Coords> redundant;

		for (Coords other : ranges)
		{
			if (current == other)
			{
				current.first = INT_MAX;
				current.second = INT_MIN;
				break;
			}

			if (current.first >= other.first)
			{
				if (current.first <= other.second)	//Current first is inside other, we have overlap
				{
					if (current.second <= other.second)	//Current is completely inside other, current is all overlap and should be ignored completely
					{
						current.first = INT_MAX;
						current.second = INT_MIN;
						break;
					}
					else	//Only current first is inside other, adjust current to remove overlap
					{
						current.first = other.second + 1;
					}
				}
			}
			else if (current.second <= other.second && current.second >= other.first)	//Current second is inside other, we have overlap. Adjust current to remove it
			{
				current.second = other.first - 1;
			}
			else if (other.first > current.first && other.first < current.second && other.second > current.first && other.second < current.second)
			{
				//Other is completely inside current. It should be removed from the list as it is redundant
				//Can't remove now as that would break the logic for the current loop, so mark for deletion afterwards
				//It doesn't matter if current is resized after marking other for redundancy, as it will still be covered by other ranges in that case
				redundant.push_back(other);
			}
		}

		//Remove any redundant ranges from the list
		while (!redundant.empty())
		{
			ranges.remove(redundant.front());
			redundant.pop_front();
		}

		//Check we have a valid range
		if (current.first > current.second)
			continue;

		int score = abs(current.second - current.first) + 1;
		result += score;
	}

	return result;
}