#pragma once
#include "SceneBase.h"
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
	// �t�F�[�h�p�̃J�E���g�ϐ�
    int m_frame = 60;

	// BGM�n���h��
	int m_bgmHandle;

	// �Q�[���I�u�W�F�N�g
	std::shared_ptr<Player> m_player;
	std::shared_ptr<Map>    m_map;
	std::shared_ptr<Camera> m_camera;
	std::vector<std::shared_ptr<GroundEnemy>> m_groundEnemyArray;
	std::vector<std::shared_ptr<FlyingEnemy>> m_flyingEnemyArray;

	using UpdateFunc_t = void(GameScene::*)(Input& input);
	using DrawFunc_t   = void(GameScene::*)();

	UpdateFunc_t m_update;
	DrawFunc_t   m_draw;
	
	void NormalUpdate(Input& input);
	void FadeInUpdate(Input& input);
	void FadeOutUpdate(Input& input);

	void NormalDraw();
	void FadeDraw();

	void CreateGroundEnemy(float posX, float posY);
	void CreateFlyingEnemy(float posX, float posY);
};