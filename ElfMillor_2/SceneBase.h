#pragma once
class SceneBase
{
public:
	SceneBase();

	virtual void Update() abstract;
	virtual void Draw() abstract;
};