#pragma once
#include "Node.h"
class StartNode :
	public Node
{
public:
	StartNode();
	~StartNode();

	virtual int CheckRoutes(bool doubleUsed) override;
};

