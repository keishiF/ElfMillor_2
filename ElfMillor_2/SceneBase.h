#pragma once

class Input;
class SceneController;

class SceneBase
{
protected:
	SceneController& m_controller;

public:

	SceneBase(SceneController& controller);

	virtual void Update(Input& input) = 0;

	virtual void Draw() = 0;
};