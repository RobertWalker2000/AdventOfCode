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

	//Array representing a table which shows the scores given for each matchup (Structured in form of Array[oppChoice][roundResult])
	//For oppChoice: 0 = rock, 1 = paper, 3 = scissors
	//For roundResult: 0 = lose, 1 = draw, 3 = win
	//0 points for a loss, 3 for a draw, 6 for a win - Bonus points per player choice: 1 for rock, 2 for paper, 3 for scissors
	int scoreTable[3][3];
	scoreTable[0][0] = 3;	scoreTable[0][1] = 4;	scoreTable[0][2] = 8;
	scoreTable[1][0] = 1;	scoreTable[1][1] = 5;	scoreTable[1][2] = 9;
	scoreTable[2][0] = 2;	scoreTable[2][1] = 6;	scoreTable[2][2] = 7;

	int score = 0;
	int prevScore = 0;
	while (!input.eof())
	{
		char oppChoice;
		char roundResult;

		oppChoice = input.get();
		input.get();
		roundResult = input.get();
		input.get();

		//Convert the letter inputs to a value between 0 and 3
		oppChoice -= 65;
		roundResult -= 88;

		if (roundResult >= 0 && roundResult <= 2 && oppChoice >= 0 && oppChoice <= 2)
		{
			score += scoreTable[oppChoice][roundResult];
		}
		else
			std::cout << "ERROR: Invalid choices" << std::endl;
	}

	std::cout << "Total Score: " << score << std::endl;
	exit(0);
}
