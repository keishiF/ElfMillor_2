#pragma once
#include "SceneBase.h"

class TitleScene :
    public SceneBase
{
public:
    TitleScene();

    virtual void Update() override;
    virtual void Draw() override;
};