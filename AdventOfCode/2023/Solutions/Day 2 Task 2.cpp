#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

int CheckGame(std::string game);
void CheckColour(int num, char colour, int dice[3]);

int main()
{
	//Create an input stream and open the file
	const int day = 2;

	std::string fileName;
	fileName.append("2023/Inputs/D");
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

	int result = 0;
	while (!input.eof())
	{
		char temp[256];
		input.getline(temp, 256);
		result += CheckGame(std::string(temp));
	}

	std::cout << "Sum of IDs for all possible games: " << result << std::endl;
	exit(0);
}

int CheckGame(std::string game)
{
	if (game.empty())
		return 0;

	//Strip the start of the spring to reach the first dice
	game.erase(0, 5);
	game.erase(0, game.find(' ') + 1);
	
	//Tracks the necessary number of dice (max values found) [r, g, b]
	int diceNeeded[3] = { 0, 0, 0 };
	//Loop through the line, finding each number/colour pairing and checking their validity
	while (!game.empty())
	{
		//Read in the number of cubes then skip ahead to the colour
		int num = std::stoi(game);
		game.erase(0, game.find(' ') + 1);

		//Check the validity of the cube selection using the number and colour code
		CheckColour(num, game[0], diceNeeded);

		//If there are no spaces left, we have checked the last set of cubes
		int space = game.find(' ');
		if (space == std::string::npos)
			break;

		//If there are still cubes left, skip to the next one
		game.erase(0, space + 1);
	}

	//If we reached here, we have checked every handful of dice and they were all valid, therefore we count this game's ID
	return diceNeeded[0] * diceNeeded[1] * diceNeeded[2];
}

void CheckColour(int num, char colour, int dice[3])
{
	int die = -1;

	if (colour == 'r')
		die = 0;

	if (colour == 'g')
		die = 1;

	if (colour == 'b')
		die = 2;

	dice[die] = std::max(dice[die], num);
}
