#include <iostream>
#include <fstream>
#include <string>

int main()
{
	//Create an input stream and open the file
	const int day = 2;
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

	//Array representing a table which shows the scores given for each matchup (Structured in form of Array[playerChoice][opponentChoice])
	//0 = rock, 1 = paper, 3 = scissors
	//0 points for a loss, 3 for a draw, 6 for a win - Bonus points per choice: 1 for rock, 2 for paper, 3 for scissors
	int scoreTable[3][3];
	scoreTable[0][0] = 4;	scoreTable[0][1] = 1;	scoreTable[0][2] = 7;
	scoreTable[1][0] = 8;	scoreTable[1][1] = 5;	scoreTable[1][2] = 2;
	scoreTable[2][0] = 3;	scoreTable[2][1] = 9;	scoreTable[2][2] = 6;

	int score = 0;
	int prevScore = 0;
	while (!input.eof())
	{
		char playerChoice;
		char oppChoice;

		oppChoice = input.get();
		input.get();
		playerChoice = input.get();
		input.get();

		//Convert the letter inputs to a value between 0 and 3
		playerChoice -= 88;
		oppChoice -= 65;

		if (playerChoice >= 0 && playerChoice <= 2 && oppChoice >= 0 && oppChoice <= 2)
		{
			score += scoreTable[playerChoice][oppChoice];
		}
		else
			std::cout << "ERROR: Invalid choices" << std::endl;
	}

	std::cout << "Total Score: " << score << std::endl;
	exit(0);
}
