#include "ScoreScreen.h"

#include "../../System/DataManager.h"

const int SCORE_LINE_PERFECT = 0;
const int SCORE_LINE_GREAT = 1;
const int SCORE_LINE_GOOD = 2;
const int SCORE_LINE_BAD = 3;
const int SCORE_LINE_MISS = 4;

const int SCORE_LINE_FAST = 7;
const int SCORE_LINE_LATE = 8;

int GetScoreHeight(ScoreTypes score)
{
	switch (score)
	{
	case ScoreTypes::Miss:
		return SCORE_LINE_MISS;
	case ScoreTypes::Bad:
		return SCORE_LINE_BAD;
	case ScoreTypes::Good:
		return SCORE_LINE_GOOD;
	case ScoreTypes::Great:
		return SCORE_LINE_GREAT;
	case ScoreTypes::Perfect:
		return SCORE_LINE_PERFECT;
	}

	return -1;
}

ScoreScreen::ScoreScreen()
{
	SetPosition(Point(23, 2));

	//// size width
	// max width: 14 = Perfect(7) + '   '(3) + 0000(4)
	// height: 9 = Perfect, Great, Good, Bad, Miss, Space 2, FAST, LATE

	SetSize(Size(14, 9));
}

ScoreScreen::~ScoreScreen()
{
}

void ScoreScreen::Init()
{
	SetBoard(Point(0, SCORE_LINE_PERFECT), 'P');
	SetBoard(Point(1, SCORE_LINE_PERFECT), 'E');
	SetBoard(Point(2, SCORE_LINE_PERFECT), 'R');
	SetBoard(Point(3, SCORE_LINE_PERFECT), 'F');
	SetBoard(Point(4, SCORE_LINE_PERFECT), 'E');
	SetBoard(Point(5, SCORE_LINE_PERFECT), 'C');
	SetBoard(Point(6, SCORE_LINE_PERFECT), 'T');

	SetBoard(Point(0, SCORE_LINE_GREAT), 'G');
	SetBoard(Point(1, SCORE_LINE_GREAT), 'R');
	SetBoard(Point(2, SCORE_LINE_GREAT), 'E');
	SetBoard(Point(3, SCORE_LINE_GREAT), 'A');
	SetBoard(Point(4, SCORE_LINE_GREAT), 'T');

	SetBoard(Point(0, SCORE_LINE_GOOD), 'G');
	SetBoard(Point(1, SCORE_LINE_GOOD), 'O');
	SetBoard(Point(2, SCORE_LINE_GOOD), 'O');
	SetBoard(Point(3, SCORE_LINE_GOOD), 'D');

	SetBoard(Point(0, SCORE_LINE_BAD), 'B');
	SetBoard(Point(1, SCORE_LINE_BAD), 'A');
	SetBoard(Point(2, SCORE_LINE_BAD), 'D');

	SetBoard(Point(0, SCORE_LINE_MISS), 'M');
	SetBoard(Point(1, SCORE_LINE_MISS), 'I');
	SetBoard(Point(2, SCORE_LINE_MISS), 'S');
	SetBoard(Point(3, SCORE_LINE_MISS), 'S');

	SetBoard(Point(0, SCORE_LINE_FAST), 'F');
	SetBoard(Point(1, SCORE_LINE_FAST), 'A');
	SetBoard(Point(2, SCORE_LINE_FAST), 'S');
	SetBoard(Point(3, SCORE_LINE_FAST), 'T');

	SetBoard(Point(0, SCORE_LINE_LATE), 'L');
	SetBoard(Point(1, SCORE_LINE_LATE), 'A');
	SetBoard(Point(2, SCORE_LINE_LATE), 'T');
	SetBoard(Point(3, SCORE_LINE_LATE), 'E');

	SetBoard(Point(13, SCORE_LINE_PERFECT), '0');
	SetBoard(Point(13, SCORE_LINE_GREAT), '0');
	SetBoard(Point(13, SCORE_LINE_GOOD), '0');
	SetBoard(Point(13, SCORE_LINE_BAD), '0');
	SetBoard(Point(13, SCORE_LINE_MISS), '0');
	SetBoard(Point(13, SCORE_LINE_LATE), '0');
	SetBoard(Point(13, SCORE_LINE_FAST), '0');
}

void ScoreScreen::PreRender()
{

	const auto& user = DataManager::GetInstance().user;
	for (int i = (int)ScoreTypes::Miss; i <= (int)ScoreTypes::Perfect; i++)
	{
		auto type = (ScoreTypes)i;

		int y = GetScoreHeight(type);

		auto iter = user.scores.find(type);
		int score = 0;
		if (iter != user.scores.end())
		{
			score = iter->second;
		}

		DrawScore(score, y);
	}

	DrawScore((int)user.fastIndicator.size(), SCORE_LINE_FAST);
	DrawScore((int)user.lateIndicator.size(), SCORE_LINE_LATE);
}

void ScoreScreen::PostRender()
{
	for (int i = (int)ScoreTypes::Miss; i <= (int)ScoreTypes::Perfect; i++)
	{
		// 1의 자리는 유지
		for (int y = 10; y < 13; y++)
		{
			SetBoard(Point(y, i), ' ');
		}
	}
}

void ScoreScreen::DrawScore(int score, int y)
{
	const char numbers[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };

	SetBoard(Point(13, y), numbers[score % 10]);
	if (score >= 10) SetBoard(Point(12, y), numbers[(score / 10) % 10]);
	if (score >= 100) SetBoard(Point(11, y), numbers[(score / 100) % 10]);
	if (score >= 1000) SetBoard(Point(10, y), numbers[(score / 1000) % 10]);
}
