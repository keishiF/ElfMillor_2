#pragma once
#include "SceneBase.h"

class ResultScene :
    public SceneBase
{
public:
    ResultScene();

    virtual void Update() override;
    virtual void Draw() override;
};