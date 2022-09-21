#pragma once
#include <iostream>
#include <fstream>

class BingoBoard
{
public:
	BingoBoard();
	~BingoBoard();

	void PopulateBoard(std::fstream* input);
	void MarkNumber(int num);
	bool HasWon();
	void PrintBoard();
	int GetScore();
	bool AssignWinOrder(int num);

private:
	static const int columns = 5;
	static const int rows = 5;

	int Numbers[columns][rows];
	bool Marks[columns][rows] = { false };
	bool hasWon = false;
	int score = 0;
	int winOrder = -1;

	bool CheckBingo(int x, int y);
	void CalculateScore(int num);
};

