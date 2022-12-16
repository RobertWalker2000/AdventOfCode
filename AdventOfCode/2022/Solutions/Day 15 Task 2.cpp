#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <set>
#include <algorithm>

typedef std::pair<int, int> Coords;

struct Line
{
	Line() {};
	Line(Coords inStart, Coords inEnd)
	{
		start = inStart;
		end = inEnd;

		if (start.second > end.second)	//Line slopes down to the right
			xIntercept = end.first + end.second;
		else
		{
			//Line slopes down to the left
			xIntercept = start.first - start.second;
		}
	}

	Line(int minY, int maxY, int xInt, int gradient)
	{
		xIntercept = xInt;

		if (gradient > 0)
		{
			start.first = (minY + xInt) / gradient;	//Using y = mx + b
			end.first = (maxY + xInt) / gradient;

			start.second = minY;
			end.second = maxY;
		}
		else if (gradient < 0)
		{
			start.first = (maxY - xInt) / gradient;
			end.first = (minY - xInt) / gradient;

			start.second = maxY;
			end.second = minY;
		}
	}

	Coords start;
	Coords end;
	int xIntercept;
};

struct Box
{
	Box() {};
	Box(Coords top, Coords right, Coords bottom, Coords left)
	{
		topLeft = Line(left, top);
		topRight = Line(top, right);
		botLeft = Line(left, bottom);
		botRight = Line(bottom, right);
	}

	Line topLeft;
	Line topRight;
	Line botLeft;
	Line botRight;

};

struct Sensor
{
	Sensor(Coords pos, int r) { position = pos; range = r; }
	Coords position;
	int range;
};

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

Box CalculateImpossibleBox(Coords sensor, Coords beacon);
void FindGaps(std::list<Line> *posGaps, std::list<Line> *negGaps, std::list<Box> boxes);
void FindLineIntercept(std::list<Coords> *spots, std::list<Line> posGaps, std::list<Line> negGaps);
void CheckGapsAgainstSensors(std::list<Coords> *gaps, std::list<Sensor> sensors);

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

	std::list<Box> boxes;
	std::list<Sensor> sensors;
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

		Box temp;
		//Create a box to represent the zone which this sensor eliminates as a possible beacon location
		temp = CalculateImpossibleBox(sensorPos, beaconPos);
		boxes.push_back(temp);

		//Remember this sensors position and range
		int xDist = abs(beaconPos.first - sensorPos.first);
		int yDist = abs(beaconPos.second - sensorPos.second);
		int range = xDist + yDist;
		sensors.push_back(Sensor(sensorPos, range));
	}

	std::list<Line> posGaps;
	std::list<Line> negGaps;
	std::list<Coords> gapSpots;

	//Find all the lines which represent gaps between two zones, storing pos and neg gradients seperately to allow for easy intercept checks
	FindGaps(&posGaps, &negGaps, boxes);

	//Find the points of intersection between the different gaps
	FindLineIntercept(&gapSpots, posGaps, negGaps);

	//Check the points of intersection against the sensors, eliminating any which are enclosed inside a sensors range
	CheckGapsAgainstSensors(&gapSpots, sensors);

	//According to the rules, we should now have exactly one gap remaining
	if (gapSpots.size() != 1)
	{
		std::cout << "ERROR: Invalid number of gaps" << std::endl;
		exit(2);
	}

	//Calculate the answer
	Coords beacon = gapSpots.front();
	int x = beacon.first;
	int y = beacon.second;
	uint64_t answer = beacon.first;
	answer *= 4000000;
	answer += beacon.second;

	std::cout << "Tuning frequency: " << answer << "Hz" << std::endl;

	exit(0);
}

//Construct the 4 lines which will make up a box representing the area excluded by the current sensor
Box CalculateImpossibleBox(Coords sensor, Coords beacon)
{
	int xDist = abs(sensor.first - beacon.first);
	int yDist = abs(sensor.second - beacon.second);
	int offset = xDist + yDist;

	Coords right = sensor;
	Coords left = sensor;
	Coords top = sensor;
	Coords bottom = sensor;

	right.first += offset;
	left.first -= offset;
	top.second += offset;
	bottom.second -= offset;

	return Box(top, right, bottom, left);
}

//Find any instance where the edges of 2 zones leave a gap exactly 1 width between them
void FindGaps(std::list<Line> *posGaps, std::list<Line> *negGaps, std::list<Box> boxes)
{
	while (!boxes.empty())
	{
		Box currentBox = boxes.front();
		boxes.pop_front();

		for (Box otherBox : boxes)
		{
			Line currentLine = currentBox.topRight;
			Line otherLine = otherBox.botLeft;

			//Difference of 2 means there is exactly one empty space between the lines. Don't use abs as using this order ensures the boxes are not overlapping
			if (otherLine.xIntercept - currentLine.xIntercept == 2)	
			{
				int middleIntercept = otherLine.xIntercept - 1;
				int maxY = std::min(currentLine.start.second, otherLine.start.second);
				int minY = std::max(currentLine.end.second, otherLine.end.second);

				if (minY <= maxY)
				{
					Line gap = Line(minY, maxY, middleIntercept, -1);
					negGaps->push_back(gap);
				}
			}

			//As above, but for the bottom left of our box
			currentLine = currentBox.botLeft;
			otherLine = otherBox.topRight;
			if (currentLine.xIntercept - otherLine.xIntercept == 2)
			{
				int middleIntercept = otherLine.xIntercept + 1;
				int maxY = std::min(currentLine.start.second, otherLine.start.second);
				int minY = std::max(currentLine.end.second, otherLine.end.second);

				if (minY <= maxY)
				{
					Line gap = Line(minY, maxY, middleIntercept, -1);
					negGaps->push_back(gap);
				}
			}

			//As above, but for our top left edge
			currentLine = currentBox.topLeft;
			otherLine = otherBox.botRight;
			if (currentLine.xIntercept - otherLine.xIntercept == 2)
			{
				int middleIntercept = otherLine.xIntercept + 1;
				int maxY = std::min(currentLine.start.second, otherLine.start.second);
				int minY = std::max(currentLine.end.second, otherLine.end.second);

				if (minY <= maxY)
				{
					Line gap = Line(minY, maxY, middleIntercept, 1);
					posGaps->push_back(gap);
				}
			}

			//As above, but for our bottom right edge
			currentLine = currentBox.botRight;
			otherLine = otherBox.topLeft;
			if (otherLine.xIntercept - currentLine.xIntercept == 2)
			{
				int middleIntercept = otherLine.xIntercept - 1;
				int maxY = std::min(currentLine.end.second, otherLine.end.second);
				int minY = std::max(currentLine.start.second, otherLine.start.second);

				if (minY <= maxY)
				{
					Line gap = Line(minY, maxY, middleIntercept, 1);
					posGaps->push_back(gap);
				}
			}
		}
	}
}

//Find the points at which the lines intersect. This will give us a point which is not covered by either of the sensors used to find this gap line
void FindLineIntercept(std::list<Coords> *spots, std::list<Line> posGaps, std::list<Line> negGaps)
{
	//Find the point of itersection between each set of perpendicular lines
	//As they are perpendicular, this X-Value will always be halfway between the lines x-intercepts
	for (Line posLine : posGaps)
	{
		int posYInt = -posLine.xIntercept;
		for (Line negLine : negGaps)
		{
			int negYInt = negLine.xIntercept;

			//as the result has to be an integer coordinate, ensure the intercept happens at an integer value
			int difference = negLine.xIntercept - posLine.xIntercept;
			if (difference % 2 == 1)
				continue;

			difference = difference / 2;
			int xAtCross = negLine.xIntercept - difference;
			int yAtCross = xAtCross + posYInt;

			//Check this is in fact the correct intercept point
			if (yAtCross != (negLine.xIntercept - xAtCross))
			{
				std::cout << "ERROR: Incorrect intercept calculations" << std::endl;
				continue;
			}

			//Check if the intersect point is within the ranges of both lines, and if so, we found an empty space
			if (yAtCross >= posLine.start.second && yAtCross <= posLine.end.second && yAtCross <= negLine.start.second && yAtCross >= negLine.end.second)
			{
				spots->push_back(Coords(xAtCross, yAtCross));
			}
		}
	}
}

//As a final check, remove any positions which are inside a sensors range.
void CheckGapsAgainstSensors(std::list<Coords> *gaps, std::list<Sensor> sensors)
{
	std::set<Coords, CoordsCompare> blocked;

	for (Coords gap : *gaps)
	{
		for (Sensor sensor : sensors)
		{
			int xDist = abs(sensor.position.first - gap.first);
			int yDist = abs(sensor.position.second - gap.second);
			int distance = yDist + xDist;

			if (distance <= sensor.range)
			{
				blocked.insert(gap);
				break;
			}
		}
	}

	for (Coords gap : blocked)
	{
		gaps->remove(gap);
	}
}