#include "MetronomeScreen.h"

#include "../../System/DataManager.h"

const int OFFSET_TOP = 2;
const int OFFSET_BOTTOM = 3;

MetronomeScreen::MetronomeScreen()
	: prevTimeSigIndex(0)
{
	SetPosition(Point(40, 2));

	// SetSize(Size(38,  12));
	auto& game = DataManager::GetInstance().game;
	SetSize(Size(38, OFFSET_TOP + OFFSET_BOTTOM + game.TimeSig.Top));
}

MetronomeScreen::~MetronomeScreen()
{
}

void MetronomeScreen::Init()
{
	// guide line
	auto& size = GetSize();
	for (int x = 0; x < size.width; x++)
	{
		for (int y = 0; y < size.height; y++)
		{
			Point p(x, y);
			if (x == 0 || x == size.width - 1)
			{
				SetBoard(p, '|');
			}

			if (y == 0 || y == size.height - 1)
			{
				SetBoard(p, '-');
			}
		}
	}

	// guide line
	SetBoard(Point(0, 0), '+');
	SetBoard(Point(size.width - 1, 0), '+');
	SetBoard(Point(0, size.height - 1), '+');
	SetBoard(Point(size.width - 1, size.height - 1), '+');
	SetBoard(Point(2, 0), ' ');
	SetBoard(Point(3, 0), 'M');
	SetBoard(Point(4, 0), 'e');
	SetBoard(Point(5, 0), 't');
	SetBoard(Point(6, 0), 'r');
	SetBoard(Point(7, 0), 'o');
	SetBoard(Point(8, 0), 'n');
	SetBoard(Point(9, 0), 'o');
	SetBoard(Point(10, 0), 'm');
	SetBoard(Point(11, 0), 'e');
	SetBoard(Point(12, 0), ' ');

	auto& game = DataManager::GetInstance().game;
	for (int i = 0; i < game.TimeSig.Top; i++)
	{
		auto p = GetSigIndex(i * game.TimeSig.Bottom);
		SetBoard(Point(p.x, OFFSET_TOP), 'o');
		SetBoard(Point(p.x, OFFSET_BOTTOM + game.TimeSig.Top), 'o');
	}
}

void MetronomeScreen::PreRender()
{
	auto& game = DataManager::GetInstance().game;

	// remove prev
	if (prevTimeSigIndex != game.TimeSigIndex)
	{
		auto prevPoint = GetSigIndex(prevTimeSigIndex);
		SetBoard(prevPoint, ' ');
		SetBoard(Point(prevPoint.x, OFFSET_TOP), 'o');
		SetBoard(Point(prevPoint.x, OFFSET_BOTTOM + game.TimeSig.Top), 'o');
	}

	auto currentPoint = GetSigIndex(game.TimeSigIndex);
	SetBoard(currentPoint, '+');
	SetBoard(Point(currentPoint.x, OFFSET_TOP), 'O');
	SetBoard(Point(currentPoint.x, OFFSET_BOTTOM + game.TimeSig.Top), 'O');

	prevTimeSigIndex = game.TimeSigIndex;
}

void MetronomeScreen::PostRender()
{
}

Point MetronomeScreen::GetSigIndex(int sig) const
{
	auto& game = DataManager::GetInstance().game;

	int y = sig % game.TimeSig.Bottom;

	int interval = (36 / game.TimeSig.Top);
	int offset = interval / 2 - 1;
	int x = 2 + offset + (sig / game.TimeSig.Bottom) * interval;

	return Point(x, OFFSET_BOTTOM + y);
}
