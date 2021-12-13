#include <iostream>
#include <fstream>

int GetNumSegments(char number[10]);
bool ShouldBeCounted(int num);

int main()
{
	std::fstream input;
	input.open("Day 8 Input.txt");
	//input.open("Test Input.txt");

	if (!input)
	{
		std::cout << "ERROR: Couln't open input file" << std::endl;
		return 1;
	}

	char display[10];
	int count = 0;
	while (!input.eof())
	{
		input.ignore(256, '|');
		input.get();

		for (int i = 0; i < 3; i++)
		{
			input.getline(display, 256, ' ');
			if (ShouldBeCounted(GetNumSegments(display)))
				count++;
		}
		input.getline(display, 256);
		if (ShouldBeCounted(GetNumSegments(display)))
			count++;
	}

	std::cout << "Total count: " << count << std::endl;

	return 0;
}

int GetNumSegments(char number[10])
{
	int segments = 0;
	for (segments = 0; segments < 10; segments++)
	{
		if (number[segments] == 0)
			break;
	}

	return segments;
}

bool ShouldBeCounted(int num)
{
	//1 requires 2 segments
	//4 requires 4 segments
	//7 requires 3 segments
	//8 requires 7 segments
	if (num == 2 || num == 4 || num == 3 || num == 7)
		return true;
	else
		return false;
}