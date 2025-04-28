#pragma once

#include "../ScreenBase.h"

class TitleScreen : public ScreenBase
{
public:
	TitleScreen();
	virtual ~TitleScreen() override;

	virtual void Init() override;
	virtual void PreRender() override;
	virtual void PostRender() override;
};