#pragma once

#include "../ScreenBase.h"

class IndicatorScreen : public ScreenBase
{
public:
	IndicatorScreen();
	virtual ~IndicatorScreen() override;

	virtual void Init() override;
	virtual void PreRender() override;
	virtual void PostRender() override;
};
