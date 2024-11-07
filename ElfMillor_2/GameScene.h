#pragma once
#include "SceneBase.h"

class GameScene :
    public SceneBase
{
public:
    GameScene();

    virtual void Update() override;
    virtual void Draw() override;
};