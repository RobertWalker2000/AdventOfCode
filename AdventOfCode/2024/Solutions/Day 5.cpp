#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <set>

bool IsValid(std::vector<int>* update, std::multimap<int, int>* rules);
int GetMiddleValue(std::vector<int>* update);
std::vector<int> GetRulesAsVector(std::multimap<int, int>* rules, int key);

int main()
{
	//Create an input stream and open the file
	const int day = 5;

	std::string fileName;
	fileName.append("2024/Inputs/D");
	fileName.append(std::to_string(day));
	fileName.append(".txt");
	std::ifstream input;
	input.open(fileName);

	//Check the input stream is valid
	if (!input)
	{
		std::cout << "Unable to open file" << std::endl;
		exit(1);
	}

	std::multimap<int, int> rules;
	//Read in all the ordering rules and store them
	while (!input.eof())
	{
		char temp[256];
		input.getline(temp, 256);
		std::string line(temp);

		if (line.empty())
			break;

		//Read in the 2 values as integers
		int delim = line.find('|');
		int before = std::stoi(line.substr(0, delim));
		int after = std::stoi(line.substr(delim + 1, line.size() - (delim + 1)));
		
		//Assign the values into the rules map
		rules.insert(std::pair<int, int>(before, after));
	}

	//Read in all the updates and process them into a vector
	std::vector<std::vector<int>> updates;
	while (!input.eof())
	{
		char temp[256];
		input.getline(temp, 256);
		std::string line(temp);

		if (line.empty())
			break;

		//Read through the line, pulling out the numbers as ints and storing them
		line.append(",");	//Add a comma to the end of the line to normalise the parsing
		std::vector<int> thisUpdate;
		while (!line.empty())
		{
			int comma = line.find(',');
			int num = std::stoi(line.substr(0, comma));
			line.erase(0, comma + 1);
			thisUpdate.push_back(num);
		}

		updates.push_back(thisUpdate);
	}

	//Check all the values and sum up the appropriate middle values
	int midValues = 0;
	for (int i = 0; i < updates.size(); i++)
	{
		if (IsValid(&updates[i], &rules))
			midValues += GetMiddleValue(&updates[i]);
	}

	std::cout << "Sum of valid middle values: " << midValues << std::endl;
	exit(0);
}

//Check if all the pages are in an acceptable order
bool IsValid(std::vector<int>* update, std::multimap<int, int>* rules)
{
	//Create a set, storing all the values that have already appeared in the given update
	std::set<int> prevPages;

	//Loop through all the elements in the vector
	for (int i = 0; i < update->size(); i++)
	{
		//Create a vector representing all the pages that can't come before the current one
		//This becomes a list of "Forbidden Pages" and checked against anything that has already appeared
		//If a "Forbidden Page" appears after the current page, that is fine
		std::vector<int> forbiddenPages = GetRulesAsVector(rules, update->at(i));

		//Loop through all the forbidden pages and check if they hae appeared earlier in the update or not
		for (int i = 0; i < forbiddenPages.size(); i++)
		{
			//If the page has appeared earlier in the update, it is invalid
			if (prevPages.find(forbiddenPages.at(i)) != prevPages.end())
				return false;
		}

		//Update is still valid for now. Add current page to previous pages and check the next page
		prevPages.insert(update->at(i));
	}

	//We checked all pages and they all passed the rules checks. Update is valid
	return true;
}

//Returns a vector containing all elements in the set which match to the given key
std::vector<int> GetRulesAsVector(std::multimap<int, int>* rules, int key)
{
	std::vector<int> vec;
	std::pair<std::multimap<int, int>::iterator, std::multimap<int, int>::iterator> range;	//A pair of iterators to the "rules" multimap
	range = rules->equal_range(key);	//Get the iterators representing the first and laste elements matching the key

	for (std::multimap<int, int>::iterator it = range.first; it != range.second; it++)	//Loop through all valid entries for the given key
		vec.push_back(it->second);	//Push the value for the current rule into the vector

	return vec;
}

//Return the value in the middle of the vector
int GetMiddleValue(std::vector<int>* update)
{
	int size = update->size();
	if (size % 2 == 0)
	{
		std::cout << "ERROR: Even sized update vector. Can not calculate middle value" << std::endl;
		return 0;
	}

	int midPoint = (size - 1) / 2;
	return update->at(midPoint);
}