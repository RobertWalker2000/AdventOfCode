#include <iostream>
#include <fstream>
#include <string>
#include <queue>

class Monkey
{
	std::queue<int> items;
	int currentItem;
	int condition;	//Value to check against during tests
	int opAdd = 0;	//Value to add upon inspection
	int opMul = 1;	//Value to multiply by on inspection
	int opPow = 1;
	int trueMonk;	//Monkey to throw items that pass tests
	int falseMonk;	//Monkey to throw items that fail tests
	int inspections = 0;
	Monkey* monkeys;

	bool InspectItem()
	{
		inspections++;

		//Take the first item from the queue
		currentItem = items.front();
		items.pop();

		//Perform this monkeys operation
		currentItem += opAdd;
		currentItem *= opMul;
		currentItem = pow(currentItem, opPow);

		//Apply relief multiplier
		currentItem /= 3;

		//Check the worry level of the item against the monkey's condition
		return ((currentItem) % condition) == 0;
	}

	void TossItem(const int receivingMonkey)
	{
		monkeys[receivingMonkey].GrabItem(currentItem);
		currentItem = NULL;
	}

public:
	void InitialiseMonkey(Monkey* inMonkeys, const int inCondition, const int inTrue, const int inFalse, const int inAdd, const int inMul, const int inPow)
	{
		monkeys = inMonkeys;
		condition = inCondition;
		trueMonk = inTrue;
		falseMonk = inFalse;
		opAdd = inAdd;
		opMul = inMul;
		opPow = inPow;
	}

	void TakeTurn()
	{
		while (items.size() > 0)
		{
			if (InspectItem())
				TossItem(trueMonk);
			else
				TossItem(falseMonk);
		}
	}

	void GrabItem(const int item)
	{
		items.push(item);
	}

	int GetInspections() { return inspections; }
};

int main()
{
	//Create an input stream and open the file
	const int day = 11;
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

	//Initialise each monkey with their unique conditions/operations
	const int numMonkeys = 8;
	Monkey monkeys[numMonkeys];

	for(int i = 0; i < numMonkeys; i++)
	{
		input.ignore(256, ':');
		input.ignore(256, ':');
		
		while (true)	//Read in starting numbers and add them to the monkey
		{
			input.ignore(256, ' ');

			char nextChar = input.peek();
			if (nextChar < '0' || nextChar > '9')
				break;

			int item;
			input >> item;
			monkeys[i].GrabItem(item);
		}

		int add = 0;
		int multiply = 1;
		int power = 1;
		input.ignore(256, 'd');	//Skip to end of 'old'
		input.get();	//Skip blank space
		char symbol = input.get();	//Read in the operation symbol
		input.get();	//Skip blank space

		if (symbol == '+')
			input >> add;
		else
			if (input.peek() == 'o') //Check if we are squaring or multiplying. Squaring if old * old, thereofre check for 'o' as next char
				power = 2;
			else
				input >> multiply;

		int testCondition;
		input.ignore(256, 'y');
		input.get();
		input >> testCondition;

		int trueMonkey;
		input.ignore(256, 'y');
		input.get();
		input >> trueMonkey;

		int falseMonkey;
		input.ignore(256, 'y');
		input.get();
		input >> falseMonkey;

		monkeys[i].InitialiseMonkey(monkeys, testCondition, trueMonkey, falseMonkey, add, multiply, power);
	}

	//Play a given number of rounds with the monkeys
	const int numRounds = 20;
	for (int round = 0; round < numRounds; round++)
	{
		for (int monk = 0; monk < numMonkeys; monk++)
		{
			monkeys[monk].TakeTurn();
		}
	}

	int highNum = 0;
	int lowNum = 0;
	for (int i = 0; i < numMonkeys; i++)
	{
		int inspections = monkeys[i].GetInspections();

		if (inspections > lowNum)
		{
			if (inspections > highNum)
			{
				lowNum = highNum;
				highNum = inspections;
			}
			else
				lowNum = inspections;
		}
	}

	std::cout << "Level of monkey business: " << (lowNum * highNum) << std::endl;
	exit(0);
}
