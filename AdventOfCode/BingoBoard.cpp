#include "BingoBoard.h"

BingoBoard::BingoBoard() {};

BingoBoard::~BingoBoard() {};

void BingoBoard::PopulateBoard(std::fstream* input)
{
	for (int col = 0; col < columns; col++)
	{
		for (int row = 0; row < rows; row++)
		{
			*input >> Numbers[col][row];
		}
	}
}

void BingoBoard::MarkNumber(int num)
{
	if (hasWon)
		return;

	for (int col = 0; col < columns; col++)
	{
		for (int row = 0; row < rows; row++)
		{
			if (Numbers[col][row] == num)
			{
				Marks[col][row] = true;

				if (CheckBingo(col, row))
					CalculateScore(num);
			}
		}
	}
}

bool BingoBoard::CheckBingo(int x, int y)
{
	bool bingo = true;
	for (int i = 0; i < columns; i++)
	{
		if (Marks[i][y] == false)
		{
			bingo = false;
			break;
		}
	}

	if (bingo)
	{
		hasWon = true;
		return true;
	}

	bingo = true;
	for (int i = 0; i < rows; i++)
	{
		if (Marks[x][i] == false)
		{
			bingo = false;
			break;
		}
	}

	if (bingo)
	{
		hasWon = true;
		return true;
	}
}

bool BingoBoard::HasWon()
{
	return hasWon;
}

void BingoBoard::PrintBoard()
{
	std::cout << "Bingo Board----" << std::endl;

	for (int col = 0; col < columns; col++)
	{
		for (int row = 0; row < rows; row++)
		{
			std::cout << Numbers[col][row] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << "---------------" << std::endl;
}

void BingoBoard::CalculateScore(int num)
{
	score = 0;
	for (int col = 0; col < columns; col++)
	{
		for (int row = 0; row < rows; row++)
		{
			if (!Marks[col][row])
				score += Numbers[col][row];
		}
	}

	score *= num;
}

int BingoBoard::GetScore()
{
	return score;
}

bool BingoBoard::AssignWinOrder(int num)
{
	if (hasWon && winOrder == -1)
	{
		winOrder = num;
		return true;
	}
	
	return false;
}