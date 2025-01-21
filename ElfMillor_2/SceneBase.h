#pragma once

class Input;
class SceneController;

class SceneBase
{
public:

	SceneBase(SceneController& controller);

	virtual void Update(Input& input) = 0;

	virtual void Draw() = 0;

protected:
	SceneController& m_controller;
};