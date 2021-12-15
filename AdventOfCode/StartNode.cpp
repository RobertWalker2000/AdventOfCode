#include "StartNode.h"
#include <iostream>

StartNode::StartNode()
{
	name = "start";
}

StartNode::~StartNode()
{
}

int StartNode::CheckRoutes(bool doubleUsed)
{
	isValid = false;

	int numRoutes = 0;
	for (int i = 0; i < numConnections; i++)
	{
		if (connections[i]->IsValid(doubleUsed))
			numRoutes += connections[i]->CheckRoutes(doubleUsed);
	}

	isValid = true;
	return numRoutes;
}