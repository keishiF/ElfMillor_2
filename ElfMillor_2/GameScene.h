#pragma once
#include "SceneBase.h"
#include <memory>

class Player;
class Bullet;

class GameScene :
    public SceneBase
{
private:
    int m_frame = 60;
	int m_backHandle;
	std::shared_ptr<Player> m_player;

	void FadeInUpdate(Input& input);
	void FadeOutUpdate(Input& input);
	void NormalUpdate(Input& input);

	void FadeDraw();
	void NormalDraw();

	using UpdateFunc_t = void(GameScene::*)(Input& input);
	using DrawFunc_t = void(GameScene::*)();

	UpdateFunc_t m_update;
	DrawFunc_t m_draw;

public:
	GameScene(SceneController& controller);

	virtual void Update(Input& input)override;
	virtual void Draw()override;
};