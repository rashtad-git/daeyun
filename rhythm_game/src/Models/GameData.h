#pragma once

#include <list>

class Node;

enum class GameState
{
	None,
	Title,
	Game,
	Result,
};

enum class Difficulty
{
	None,
	Beginner,
	Easy,
	Normal,
	Hard,
	VeryHard,
	Expert,
	Master,
};

struct TimeSignature
{
	int BPM = 120;
	int Top = 4; // 3, 4, 6, 9, 12
	int Bottom = 4; // 4, 8 

	int GetMaxIndex() const { return Top * Bottom; }
	double GetDuration() const { return (60 / (double)BPM) * (double)Top; }
	double GetTick() const { return GetDuration() / GetMaxIndex(); }
};

struct GameData {
	GameState GameState = GameState::Title;

	bool IsPlaySound = true;
	Difficulty Difficulty = Difficulty::Easy;
	int NoteSpeed = 2;
	TimeSignature TimeSig;
	int TimeSigIndex = 0; // 0 ~ TimeSig.Top * TimeSig.Bottom
	double TimeSigFrame = 0;

	int StageNodeCount;
	std::list<Node*> StageNodes;

	double debugJudge = 0;
};
