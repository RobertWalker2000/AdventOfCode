#include <iostream>
#include <fstream>
#include <string>
#include <vector>

uint64_t Evaluate(std::string equation, bool canConcatenate);
bool TrySolve(std::vector<uint64_t> values, const uint64_t target, bool canConcatenate);
bool TryAdd(std::vector<uint64_t> values, const uint64_t target, bool canConcatenate);
bool TryMultiply(std::vector<uint64_t> values, const uint64_t target, bool canConcatenate);
bool TryConcatenate(std::vector<uint64_t> values, const uint64_t target, bool canConcatenate);

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

	uint64_t nonConcatTotal = 0;
	uint64_t concatTotal = 0;
	for (int i = 0; i < lines.size(); i++)
	{
		uint64_t value = Evaluate(lines[i], false);

		//If we got a value without concatenating, no need to check with concats
		if (value != 0)
		{
			nonConcatTotal += value;
			concatTotal += value;
			continue;
		}

		//If we didn't find a value without concatenating, try again with concats
		concatTotal += Evaluate(lines[i], true);
	}

	std::cout << "Sum of all valid equations (No concats): " << nonConcatTotal << std::endl;
	std::cout << "Sum of all valid equations (With concats): " << concatTotal << std::endl;
	exit(0);
}

uint64_t Evaluate(std::string equation, bool canConcatenate)
{
	//Find the first number, which is the calibration value. Store it as an int and remove it form the string
	int colon = equation.find(':');
	if (colon == std::string::npos)
		return 0;
	
	uint64_t target = std::stoull(equation.substr(0, colon));
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

	//The max value is greater than the target, equation may still be solvable in some way
	if (TrySolve(values, target, canConcatenate))
		return target;

	//Equation was unsolvable
	return 0;
}

bool TrySolve(std::vector<uint64_t> values, const uint64_t target, bool canConcatenate)
{
	//If we only have 1 value left, and it is equal to the target, we have solved the equation
	//If that one value is not equal to the target, the equation is unsolved
	if (values.size() == 1)
	{
		if (values[0] == target)
			return true;
		else
			return false;
	}

	//If we have multiple values, but the first value is greater than the target, the equation is impossible
	if (values[0] > target)
		return false;

	//As we only need at least 1 possible way to solve the equation, we can return on the first one that returns true, no need to check others afterwards
	//If we can concatenate, try it first. Allows us to keep the same control flow for the other 2 methods afterwards
	if (canConcatenate)
	{
		if (TryConcatenate(values, target, canConcatenate))
			return true;
	}

	//Test the equation by adding the first 2 values
	if (TryAdd(values, target, canConcatenate))
		return true;

	return TryMultiply(values, target, canConcatenate);
}

bool TryAdd(std::vector<uint64_t> values, const uint64_t target, bool canConcatenate)
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
	return TrySolve(values, target, canConcatenate);
}

bool TryMultiply(std::vector<uint64_t> values, const uint64_t target, bool canConcatenate)
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
	return TrySolve(values, target, canConcatenate);
}

bool TryConcatenate(std::vector<uint64_t> values, const uint64_t target, bool canConcatenate)
{
	//Check that this value was allowed to be called, in case it was done by mistake
	if (!canConcatenate)
	{
		std::cout << "ERROR: TryConcatenate was called with value \"canConcatenate = false\"" << std::endl;
		return false;
	}

	//Check that we have multiple values to work with
	if (values.size() < 2)
	{
		std::cout << "ERROR: TryConcatenate was called with insufficient values to operate on" << std::endl;
		return false;
	}

	//Convert the numbers to strings, which makes them easy to concatenate
	std::string v1 = std::to_string(values[0]);
	std::string v2 = std::to_string(values[1]);

	//Concatenate the values, and convert the result back to a number
	std::string combined(v1);
	combined.append(v2);
	uint64_t result = std::stoull(combined);

	//Replace the first 2 values with the concatenated value
	values.erase(values.begin());
	values[0] = result;

	//Take the new list of values, and check if the equation is solveable
	return TrySolve(values, target, canConcatenate);
}
