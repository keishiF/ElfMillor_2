#pragma once
#include "SceneBase.h"
#include "Life.h"
#include <memory>
#include <vector>

class Player;
class Camera;
class Map;
class GroundEnemy;
class FlyingEnemy;
class EnemyBase;

class GameScene : public SceneBase
{
public:
	GameScene(SceneController& controller);
	~GameScene();

	virtual void Update(Input& input)override;
	virtual void Draw()override;

private:
	// フェード用のフレーム
	int m_fadeFrameCount;

	// HP(ハート)のハンドル
	int m_lifeHandle;

	// 背景のハンドル
	int m_bgHandle;

	// BGMハンドル
	int m_bgmHandle;

	// 死亡時のSEハンドル
	int m_deadSEHandle;

	// クリア時のHPによって変動するスコア
	float m_clearHpScore;

	// ゲームオブジェクト
	std::shared_ptr<Player> m_player;
	std::shared_ptr<Map>    m_map;
	std::shared_ptr<Camera> m_camera;
	std::vector<std::shared_ptr<GroundEnemy>> m_groundEnemyArray;
	std::vector<std::shared_ptr<FlyingEnemy>> m_flyingEnemyArray;

	using UpdateFunc_t = void(GameScene::*)(Input& input);
	using DrawFunc_t   = void(GameScene::*)();

	UpdateFunc_t m_update;
	DrawFunc_t   m_draw;

	Life m_life[10];
	
	// 通常更新
	void NormalUpdate(Input& input);
	// フェードイン
	void FadeInUpdate(Input& input);
	// フェードアウト
	void FadeOutUpdate(Input& input);

	// 通常描画
	void NormalDraw();
	// フェード時の描画
	void FadeDraw();

	// 地上の敵の生成関数
	void CreateGroundEnemy(float posX, float posY);
	// 飛行する敵の生成関数
	void CreateFlyingEnemy(float posX, float posY);

	// クリア時のHPに応じてスコアに倍率をかける
	void ClearHpScore();
};