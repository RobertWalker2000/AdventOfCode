#include <iostream>
#include <fstream>
#include <string>

int EvaluateLine(std::string line);
int EvaluateLineConditional(std::string line, bool* condition);
int EvaluateInstruction(std::string instruction);

int main()
{
	//Create an input stream and open the file
	const int day = 3;

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

	bool doMul = true;
	int raw = 0;
	int conditionalResult = 0;
	while (!input.eof())
	{
		static const int lineSize = 4096;
		char line[lineSize];
		input.getline(line, lineSize);
		std::string strLine(line);
		raw += EvaluateLine(strLine);
		conditionalResult += EvaluateLineConditional(strLine, &doMul);
	}

	std::cout << "Raw result: " << raw << std::endl;
	std::cout << "Conditional result: " << conditionalResult << std::endl;
	exit(0);
}

int EvaluateLine(std::string line)
{
	int value = 0;
	while (!line.empty())
	{
		//Find the first instance of an "m", marking the start of a potential instruction
		int mul = line.find("mul(");

		//If there is no M, no instructions remain
		if (mul == std::string::npos)
			return value;

		line.erase(0, mul);	//Erase all characters before the M
		
		//Find the first ')', marking the end of a potential instruction
		int instrEnd = line.find(')');

		//If there is no ')', no instructions remain
		if (instrEnd == std::string::npos)
			return value;

		//Evaluate the potential instruction
		value += EvaluateInstruction(line.substr(0, instrEnd + 1));

		//Remove the M, so we can start looking for the next instruction
		line.erase(0, 1);
	}

	return value;
}

int EvaluateLineConditional(std::string line, bool* condition)
{
	int value = 0;
	while (!line.empty())
	{
		//If we are currently set to not do instrucitons, we can ignore everything until the next "Do()"
		if (*condition == false)
		{
			int nextDo = line.find("do()");

			//If there is no Do() instruciton, there is nothing else to evaluate
			if (nextDo == std::string::npos)
				break;

			//If we found a Do(), remove everything before it and set our condition to true
			line.erase(0, nextDo);
			*condition = true;
		}

		//Find the first instance of a "mul(", marking the start of a potential instruction
		int mul = line.find("mul(");

		int nextDont = line.find("don't()");

		//If we didn't find a mul or a dont instruction, there is nothing left to evaluate
		if (mul == std::string::npos && nextDont == std::string::npos)
			break;

		//If we only found a don't instruction, erase everyhting before it, set condition to false, and go to the next loop
		if (mul == std::string::npos)
		{
			line.erase(0, nextDont);
			*condition = false;
			continue;
		}

		//If we found both instructions, but the Dont was first, do as above
		if (nextDont != std::string::npos)
		{
			if (nextDont < mul)
			{
				line.erase(0, nextDont);
				*condition = false;
				continue;
			}
		}

		//If we have reached here, our next instruction is a mul. Begin evaluating
		line.erase(0, mul);	//Erase all characters before the "mul("

		//Find the first ')', marking the end of a potential instruction
		int instrEnd = line.find(')');

		//If there is no ')', no instructions remain
		if (instrEnd == std::string::npos)
			break;

		//Evaluate the potential instruction
		value += EvaluateInstruction(line.substr(0, instrEnd + 1));

		//Remove the M, so we can start looking for the next instruction
		line.erase(0, 1);
	}

	return value;
}

int EvaluateInstruction(std::string instruction)
{
	//Check that the instruction begins with "mul(". If not, instruction is invalid.
	if (instruction.substr(0, 4) != "mul(")
		return 0;

	instruction.erase(0, 4);	//Strip the "mul( from the start of the instruction
	instruction.pop_back();		//Remove the ")" from the end of the instruction

	//We should now simply have 2 numbers seperated by a comma. If not, instruction is invalid
	if (instruction.find_first_not_of("0123456789,") != std::string::npos)	//Check we only have valid characters
		return 0;

	//We should now have 2 numbers seperated by a singular comma. Check that is true
	int comma = instruction.find(',');

	if (comma == std::string::npos || comma == 0 || comma == instruction.size() - 1)	//Check that the comma: A) Exists, B) Is not the first element, and C) Is not the last element
		return 0;

	if (comma != instruction.rfind(','))	//Check that we do not have multiple commas
		return 0;

	//We passed all checks, instruction should be valid. Now evaluate it and return the result
	int num1 = stoi(instruction.substr(0, comma));	//Read the number before the comma
	int num2 = stoi(instruction.substr(comma + 1, instruction.size() - comma + 1));	//Read the number after the comma

	return num1 * num2;
}