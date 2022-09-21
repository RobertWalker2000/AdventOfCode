#pragma once
#include <vector>
#include <string>

class Node
{
public:
	Node();
	~Node();

	std::string name;

	void SetName(std::string newName);
	void InsertConnection(Node* other);
	virtual int CheckRoutes(bool doubleUsed);
	bool IsValid(bool doubleUsed);

	bool operator==(Node other);

protected:
	std::vector<Node*> connections;

	bool isValid = true;
	bool isLowercase = false;
	int numConnections = 0;
	int smallVisits = 0;
};

