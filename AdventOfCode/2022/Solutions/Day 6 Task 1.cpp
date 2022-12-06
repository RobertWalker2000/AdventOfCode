#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <queue>

int main()
{
	//Create an input stream and open the file
	const int day = 6;
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

	const int markerLength = 4;
	std::set<char> markerSet;	//Ensures the queue only stores unique characters
	std::queue<char> marker;	//Remembers the order of the characters
	int i = 0;					//Tracks how many characters we go through before reaching the marker

	while (!input.eof())
	{
		i++;
		char nextChar = input.get();
		bool unique = markerSet.insert(nextChar).second;

		//If the character is a duplicate, remove elements from the front of the queue until we remove the copy of the next char
		while(!unique)
		{
			char temp = marker.front();
			marker.pop();
			markerSet.erase(temp);
			unique = markerSet.insert(nextChar).second;
		}

		marker.push(nextChar);

		if (markerSet.size() == markerLength)
			break;
	}

	std::cout << "Num characters processed before start of marker: " << i << std::endl;
	exit(0);
}
