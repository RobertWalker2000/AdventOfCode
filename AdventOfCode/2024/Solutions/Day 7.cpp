#include <iostream>
#include <fstream>
#include <string>
#include <vector>

uint64_t Evaluate(std::string equation);
bool TrySolve(std::vector<uint64_t> values, const uint64_t target);
bool TryAdd(std::vector<uint64_t> values, const uint64_t target);
bool TryMultiply(std::vector<uint64_t> values, const uint64_t target);

int main()
{
	//Create an input stream and open the file
	const int day = 7;

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

	std::vector<std::string> lines;
	while (!input.eof())
	{
		char temp[256];
		input.getline(temp, 256);
		std::string line(temp);

		if (line.empty())
			break;

		lines.push_back(line);
	}

	uint64_t total = 0;
	for (int i = 0; i < lines.size(); i++)
		total += Evaluate(lines[i]);

	std::cout << "Sum of all valid equations: " << total << std::endl;
	exit(0);
}

uint64_t Evaluate(std::string equation)
{
	//Find the first number, which is the calibration value. Store it as an int and remove it form the string
	int colon = equation.find(':');
	if (colon == std::string::npos)
		return 0;
	
	uint64_t target = std::stoll(equation.substr(0, colon));
	equation.erase(0, colon + 2);	//Remove the calibration value, the colon, and the space

	//Add a space to the end of the string to normalise the formatting (all numbers are followed by a space)
	equation.append(" ");

	//Read in all numbers from the string and store them ass a vector of ints
	std::vector<uint64_t> values;
	while (!equation.empty())
	{
		int space = equation.find(' ');
		if (space == std::string::npos)
			break;

		int temp = std::stoi(equation.substr(0, space));
		values.push_back(temp);
		equation.erase(0, space + 1);
	}

	if (TrySolve(values, target))
		return target;

	return 0;
}

bool TrySolve(std::vector<uint64_t> values, const uint64_t target)
{
	//If we only have 1 value left, and it is equal to the target, we have solved the equation
	//If that one value is not euqal to the target, the equation is unsolved
	if (values.size() == 1)
	{
		if (values[0] == target)
			return true;
		else
			return false;
	}

	//If we have more than 1 value, try the 2 ways to combine the next 2 numbers
	//We only need there to be at least 1 possible solution, so if we find it on the first method, no need to perform the second method
	if (TryAdd(values, target))
		return true;

	return TryMultiply(values, target);
}

bool TryAdd(std::vector<uint64_t> values, const uint64_t target)
{
	//Check that we have multiple values to work with
	if (values.size() < 2)
	{
		std::cout << "ERROR: TryAdd was called with insufficient values to operate on" << std::endl;
		return false;
	}

	//Add the values together, then replace the first 2 values in the vector with the combined value
	uint64_t sum = values[0] + values[1];
	values.erase(values.begin());
	values[0] = sum;
	
	//Take the new list of values, and check if the equation is solveable
	return TrySolve(values, target);
}

bool TryMultiply(std::vector<uint64_t> values, const uint64_t target)
{
	//Check that we have multiple values to work with
	if (values.size() < 2)
	{
		std::cout << "ERROR: TryMultiply was called with insufficient values to operate on" << std::endl;
		return false;
	}

	//Multiply the values together, then replace the first 2 values in the vector with the combined value
	uint64_t product = values[0] * values[1];
	values.erase(values.begin());
	values[0] = product;

	//Take the new list of values, and check if the equation is solveable
	return TrySolve(values, target);
}