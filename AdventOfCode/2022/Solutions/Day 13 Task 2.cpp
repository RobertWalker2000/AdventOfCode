#include <iostream>
#include <fstream>
#include <string>
#include <set>

int CompareLists(std::string left, std::string right);
int FindListEnd(std::string list);

struct ListCompare
{
	bool operator()(const std::string lhs, const std::string rhs) const { return (CompareLists(lhs, rhs) == 1); }
};

int main()
{
	//Create an input stream and open the file
	const int day = 13;
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

	std::multiset<std::string, ListCompare> lists;

	while (!input.eof())
	{
		char temp[250];
		std::string leftSide;
		std::string rightSide;

		input.getline(temp, 256);
		leftSide = temp;
		input.getline(temp, 256);
		rightSide = temp;
		input.ignore(256, '\n');

		lists.insert(leftSide);
		lists.insert(rightSide);
	}

	//Create and insert the divider packets
	std::string div1 = "[[2]]";
	std::string div2 = "[[6]]";
	lists.insert(div1);
	lists.insert(div2);

	auto it1 = lists.find(div1);
	auto it2 = lists.find(div2);

	int index1 = std::distance(lists.begin(), it1) + 1;
	int index2 = std::distance(lists.begin(), it2) + 1;

	int total = index1 * index2;
	std::cout << "Product of divider packet indices: " << total << std::endl;

	exit(0);
}

int CompareLists(std::string left, std::string right)
{
	//Strip the outer brackets from the list that we are working on
	left.erase(0, 1);
	left.erase(left.size() - 1, 1);
	right.erase(0, 1);
	right.erase(right.size() - 1, 1);

	while (!(left.empty() && right.empty()))
	{
		//Check if either string hs reached its end
		if (left.empty())
			return 1;
		else if (right.empty())
			return -1;

		bool hasList = false;
		std::string subLeft;
		std::string subRight;

		if (left[0] == '[')	//This side is checking a list
		{
			hasList = true;
			int endList = FindListEnd(left);
			subLeft = left.substr(0, endList + 1);
			left.erase(0, endList + 1);	//Erase the sub list
		}
		else if (left[0] >= '0' && left[0] <= '9')	//This side is checking a number
		{
			int commaIndex = left.find(',');
			if (commaIndex == std::string::npos)
			{
				subLeft = left;
				left.clear();
			}
			else
			{
				subLeft = left.substr(0, commaIndex);
				left.erase(0, commaIndex + 1);
			}
		}

		if (right[0] == '[')	//This side is checking a list
		{
			hasList = true;
			int endList = FindListEnd(right);
			subRight = right.substr(0, endList + 1);
			right.erase(0, endList + 1);
		}
		else if (right[0] >= '0' && right[0] <= '9')	//This side is checking a number
		{
			int commaIndex = right.find(',');
			if (commaIndex == std::string::npos)
			{
				subRight = right;
				right.clear();
			}
			else
			{
				subRight = right.substr(0, commaIndex);
				right.erase(0, commaIndex + 1);
			}
		}

		//If we have a list to deal with, ensure both substrings qualify as lists then evaluate them
		if (hasList)
		{
			if (subLeft[0] != '[')
			{
				subLeft.insert(subLeft.begin(), '[');
				subLeft.insert(subLeft.end(), ']');
			}

			if (subRight[0] != '[')
			{
				subRight.insert(subRight.begin(), '[');
				subRight.insert(subRight.end(), ']');
			}

			int result = CompareLists(subLeft, subRight);
			if (result != 0)
				return result;
		}
		else	//Compare the two numbers to check if they are in order
		{
			int numLeft = stoi(subLeft);
			int numRight = stoi(subRight);
			if (numLeft < numRight)
				return 1;
			else if (numLeft > numRight)
				return -1;
		}

		//Ensure we don't have a comma at the front of our list
		if (right[0] == ',')
			right.erase(0, 1);

		if (left[0] == ',')
			left.erase(0, 1);
	}

	return 0;
}

int FindListEnd(std::string list)
{
	int i = 0;
	int layers = 0;

	while(i < list.size())
	{
		if (list[i] == '[')
		{
			layers++;
		}
		else if (list[i] == ']')
		{
			layers--;
			if (layers == 0)
				break;
		}
		i++;
	}
	return i;
}