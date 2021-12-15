#include <iostream>
#include <fstream>
#include "Node.h"
#include "StartNode.h"
#include "EndNode.h"

int main()
{
	std::fstream input;
	input.open("Day 12 Input.txt");
	//input.open("Test Input.txt");
	if (!input)
	{
		std::cout << "ERROR: Couldn't open file" << std::endl;
		return 1;
	}

	StartNode* start = new StartNode;
	EndNode* end = new EndNode;

	std::vector<Node*> caves;
	caves.push_back(start);
	caves.push_back(end);

	while (!input.eof())
	{
		char firstName[10];
		char secondName[10];

		input.getline(firstName, 256, '-');
		input.getline(secondName, 256, '\n');

		Node* firstNode = new Node;
		Node* secondNode = new Node;

		firstNode->SetName(firstName);
		secondNode->SetName(secondName);

		//Check if either of the nodes are already tracked
		int firstIndex = -1;
		int secondIndex = -1;
		for (int i = 0; i < caves.size(); i++)
		{
			if (*caves[i] == *firstNode)
				firstIndex = i;

			if (*caves[i] == *secondNode)
				secondIndex = i;
		}

		//If we didn't find a match for node 1, add it to the vector
		if (firstIndex == -1)
		{
			caves.push_back(firstNode);
			firstIndex = caves.size() - 1;
		}
		else
		{
			//If this node is a duplicate, delete the copy not in the vector
			delete firstNode;
			firstNode = nullptr;
		}

		//If we didn't find a match for node 2, add it to the vector
		if (secondIndex == -1)
		{
			caves.push_back(secondNode);
			secondIndex = caves.size() - 1;
		}
		else
		{
			delete secondNode;
			secondNode = nullptr;
		}

		//Push each node onto its pairs connections node
		caves[firstIndex]->InsertConnection(caves[secondIndex]);
		caves[secondIndex]->InsertConnection(caves[firstIndex]);
	}

	int routes = caves[0]->CheckRoutes(false);
	std::cout << "Routes Found: " << routes << std::endl;

	system("pause");
	return 0;
}