#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int main()
{
	//Create an input stream and open the file
	const int day = 5;
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

	const int numStacks = 9;
	const int tallestStack = 8;
	std::vector<char> stacks[numStacks];

	for (int i = 0; i < tallestStack; i++)
	{
		char temp;
		for (int j = 0; j < numStacks; j++)
		{
			input.get();					//Skip the first character (either space or '[')
			temp = input.get();				//Read in the crate value (or space if empty)
			if (temp >= 'A' && temp <= 'Z')	//Check that we have a valid crate
				stacks[j].insert(stacks[j].begin(), temp);	//Push the crate onto the front of the vector (the bottom of the stack) as we are working from top down

			input.get();	//Skip the last value (either space or ']')
			input.get();	//Skip the space between crates
		}
	}

	//Skip the stack numbers and empty line so we get to the start of the instructions
	input.ignore(256, 'm');
	char temp = input.get();

	//Loop through all instructions, carrying them out as we go
	while (!input.eof())
	{
		int numCrates;
		int startingStack;
		int destinationStack;
		char temp[5];

		//Skip to the value for number of crates to move, and read it in
		input.ignore(256, ' ');
		input.getline(temp, 256, ' ');
		numCrates = atoi(temp);

		//Check we are reading from a valid line
		if (numCrates <= 0)
			break;

		//Skip to the starting stack and read it in
		input.ignore(256, ' ');
		input.getline(temp, 256, ' ');
		startingStack = atoi(temp) - 1;	//Reduce the stack number by 1 to make compatable with vector index

		//Skip to the destination stack and read it in
		input.ignore(256, ' ');
		input.getline(temp, 256);
		destinationStack = atoi(temp) - 1;

		//Check that there are enough crates in the stack to complete the move
		if (stacks[startingStack].size() < numCrates)
		{
			std::cout << "ERROR: Not enough crates in stack" << std::endl;
			return -1;
		}

		//Move the appropriate number of crates from start to finish, always working with the top of each stack
		std::vector<char> movingStack;	//Temporary stack used to easily keep the crates in order
		for (int i = 0; i < numCrates; i++)	//Copy the crates into a temporary stack
		{
			movingStack.push_back(stacks[startingStack].back());	//Copy the element from top of start stack to top of moving stack
			stacks[startingStack].pop_back();	//Remove the crate from the top of the starting stack
		}

		for (int i = 0; i < numCrates; i++)	//Copy the crates from the temporary stack to the destination
		{
			stacks[destinationStack].push_back(movingStack.back());	//Copy the last element from the moving stack to the end of the destination stack
			movingStack.pop_back();	//Remove the last crate from the end of the moving stack
		}
	}

	std::string topCrates = "Top cartes: ";
	for (int i = 0; i < numStacks; i++)
		if(stacks[i].size() > 0)
			topCrates.append(1u, stacks[i].back());

	std::cout << topCrates << std::endl;

	exit(0);
}
