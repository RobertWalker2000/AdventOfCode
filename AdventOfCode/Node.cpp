#include "Node.h"
#include <ctype.h>
#include <iostream>

Node::Node()
{
}

Node::~Node()
{
}

bool Node::operator==(Node other)
{
	if (name == other.name)
		return true;

	return false;
}

void Node::SetName(std::string newName)
{
	name = newName;

	isLowercase = islower(name[0]);
}

void Node::InsertConnection(Node* other)
{
	for (int i = 0; i < numConnections; i++)
	{
		if (*other == *connections[i])
		{
			return;
		}
	}

	connections.push_back(other);
	numConnections++;
}

bool Node::IsValid(bool doubleUsed)
{
	if (!isValid)
		return false;

	if (doubleUsed)
		return smallVisits < 1;
	else
		return smallVisits < 2;
}

int Node::CheckRoutes(bool doubleUsed)
{
	//If this is a small cave, mark it so we can't backtrack through it
	if (isLowercase)
	{
		smallVisits++;
		if (smallVisits == 2)
			doubleUsed = true;
	}

	int numRoutes = 0;
	for (int i = 0; i < numConnections; i++)
	{
		if(connections[i]->IsValid(doubleUsed))
			numRoutes += connections[i]->CheckRoutes(doubleUsed);
	}

	if (isLowercase)
		smallVisits--;

	return numRoutes;
}