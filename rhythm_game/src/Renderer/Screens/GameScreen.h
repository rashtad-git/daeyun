#pragma once

#include "../../Models/Math.h"
#include "../ScreenBase.h"

class GameScreen : public ScreenBase
{
public:
	GameScreen();
	virtual ~GameScreen() override;

	virtual void Init() override;
	virtual void PreRender() override;
	virtual void PostRender() override;

private:
	void DrawTapped();
	void DrawNode();
};
