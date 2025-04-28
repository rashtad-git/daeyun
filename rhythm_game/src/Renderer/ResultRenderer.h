#pragma once
#include "IRenderer.h"

class ResultRenderer : public IRenderer
{
public:
    virtual void PreRender() override;
    virtual char OnRender(const Point& p) override;
    virtual void PostRender() override;

public:
    ResultRenderer() = default;
    virtual ~ResultRenderer() = default;
};
