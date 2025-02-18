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
	// �t�F�[�h�p�̃t���[��
	int m_fadeFrameCount;

	// HP(�n�[�g)�̃n���h��
	int m_lifeHandle;

	// �w�i�̃n���h��
	int m_bgHandle;

	// BGM�n���h��
	int m_bgmHandle;

	// ���S����SE�n���h��
	int m_deadSEHandle;

	// �N���A����HP�ɂ���ĕϓ�����X�R�A
	float m_clearHpScore;

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

	Life m_life[10];
	
	// �ʏ�X�V
	void NormalUpdate(Input& input);
	// �t�F�[�h�C��
	void FadeInUpdate(Input& input);
	// �t�F�[�h�A�E�g
	void FadeOutUpdate(Input& input);

	// �ʏ�`��
	void NormalDraw();
	// �t�F�[�h���̕`��
	void FadeDraw();

	// �n��̓G�̐����֐�
	void CreateGroundEnemy(float posX, float posY);
	// ��s����G�̐����֐�
	void CreateFlyingEnemy(float posX, float posY);

	// �N���A����HP�ɉ����ăX�R�A�ɔ{����������
	void ClearHpScore();
};