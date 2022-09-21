#pragma once
#include "Node.h"
class EndNode :
	public Node
{
public:
	EndNode();
	~EndNode();

	virtual int CheckRoutes(bool doubleUsed) override;
};

