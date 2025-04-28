#include "ScreenBase.h"

const char ScreenBase::GetBoard(const Point& p) const
{
	int i = GetIndex(p);
	if (i < 0 || i >= screen.size.width * screen.size.height)
		return ' ';

	return board[i];
}

bool ScreenBase::SetBoard(const Point& p, char c)
{
	int i = GetIndex(p);
	if (i < 0 || i >= screen.size.width * screen.size.height)
		return false;

	board[i] = c;
	return true;
}

void ScreenBase::SetSize(const Size& size)
{
	if (board != nullptr)
	{
		delete[] board;
	}

	screen.size = size;
	int length = size.width * size.height;
	board = new char[length];
	for (int i = 0; i < length; i++)
	{
		board[i] = ' ';
	}
}


void ScreenBase::DrawNumber(int x, int y, int number, int space)
{
	const char numbers[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };

	//SetBoard(Point(13, y), numbers[score % 10]);
	//if (score >= 10) SetBoard(Point(12, y), numbers[(score / 10) % 10]);
	//if (score >= 100) SetBoard(Point(11, y), numbers[(score / 100) % 10]);
	//if (score >= 1000) SetBoard(Point(10, y), numbers[(score / 1000) % 10]);
}
