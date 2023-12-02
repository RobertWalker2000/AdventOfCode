#include <iostream>
#include <fstream>
#include <string>

int CheckGame(std::string game);
bool CheckColour(int num, char colour);

int main()
{
	//Create an input stream and open the file
	const int day = 0;

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

	//Strip the substring "Game " from the start of the string to get to the ID
	game.erase(0, 5);
	
	//Get the game ID from the start of the line then strip it out to reach the next number
	int gameID = std::stoi(game);
	game.erase(0, game.find(' ') + 1);
	
	//Loop through the line, finding each number/colour pairing and checking their validity
	while (!game.empty())
	{
		//Read in the number of cubes then skip ahead to the colour
		int num = std::stoi(game);
		game.erase(0, game.find(' ') + 1);

		//Check the validity of the cube selection using the number and colour code
		if (!CheckColour(num, game[0]))
			return 0;	//If the colour is invalid, do not add this ID to the result

		//If there are no spaces left, we have checked the last set of cubes
		int space = game.find(' ');
		if (space == std::string::npos)
			break;

		//If there are still cubes left, skip to the next one
		game.erase(0, space + 1);
	}

	//If we reached here, we have checked every handful of dice and they were all valid, therefore we count this game's ID
	return gameID;
}

bool CheckColour(int num, char colour)
{
	const int red = 12;
	const int green = 13;
	const int blue = 14;

	if (colour == 'r')
		return num <= red;

	if (colour == 'g')
		return num <= green;

	if (colour == 'b')
		return num <= blue;

	std::cout << "ERROR: Unexpected colour code given to CheckColour. Code: " << colour << std::endl;
	abort();
}
