#pragma once
#include "SceneBase.h"
#include "Map.h"
#include <memory>

class Player;
class Boss;
class Enemy1;
class Camera;
class Map;

class GameScene :
    public SceneBase
{
private:
    int m_frame = 60;
	std::shared_ptr<Player> m_player;
	std::shared_ptr<Boss> m_boss;
	std::shared_ptr<Enemy1> m_enemy1;
	std::shared_ptr<Map> m_map;
	std::shared_ptr<Camera> m_camera;
	
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