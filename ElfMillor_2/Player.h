#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Shot.h"
#include "Input.h"
#include "GroundEnemy.h"
#include "FlyingEnemy.h"
#include "Map.h"
#include "ScoreManager.h"
#include <memory>
#include <vector>

namespace
{
    // �V���b�g�̐�
	constexpr int kShot = 5;
}

class Player : public GameObject
{
public:
    Player(std::weak_ptr<Camera> camera);
    ~Player();

    // �������A�X�V�A�`��
    void Init();
    void Update(Input& input, std::vector<std::shared_ptr<GroundEnemy>> groundEnemy, 
        std::vector<std::shared_ptr<FlyingEnemy>> flyingEnemy, Map& map);
    void Draw();

    // �_���[�W���󂯂����̏���
    void OnDamage();

	// �v���C���[��HP���擾
	int GetHp() { return m_hp; }

    // �v���C���[�̋�`�����蔻����擾
    float GetLeft();
    float GetRight();
    float GetTop();
    float GetBottom();
    Rect  GetRect();

    // �v���C���[�̍��W���擾
    Vec3 GetPos() { return m_pos; }

    ScoreManager& GetScoreManager() { return m_scoreManager; }

    // �N���A�t���O
    bool m_isClear;

    // ���S�A�j���[�V����
    Animation m_deadAnim;

private:
    // ���G���Ԃ̍X�V
    void UpdateBlinkFrame();

    // �ړ�����
    void HandleMovement(Input& input, Map& map);
    void HandleJump(Map& map);
    void HandleGroundMovement(Input& input, Map& map);

    // �U������
    void HandleAttack(Input& input);

    // �v���C���[�̃O���t�B�b�N�n���h��
    int m_handleIdle;
    int m_handleRun;
    int m_handleDead;

    // SE�n���h��
    int m_shotSEHandle;
    int m_loopSEHandle;

    // �A�j���[�V�����t���O
    bool m_isRun;
    bool m_isJump;
    bool m_isDead;

    // �W�����v�֘A
    float m_jumpSpeed;
    int   m_jumpCount;

    // �v���C���[�̌���
    bool m_isDirLeft;

    // �v���C���[������͂����Ă��邩�ǂ���
    bool m_isUp;

    // ���G���Ԋ֘A
    int m_blinkFrameCount;
    int m_deadFrameCount;

    // �v���C���[��HP
    int m_hp;

    // �W�����v�{�^���̏��
    bool m_isLastJumpButton;

    ScoreManager m_scoreManager;

    // �V���b�g
    Shot m_shot[kShot];

    // �A�j���[�V����
    Animation m_idleAnim;
    Animation m_runAnim;
};