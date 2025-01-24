#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Shot.h"
#include "Input.h"
#include "Boss.h"
#include "GroundEnemy.h"
#include "FlyingEnemy.h"
#include "Map.h"
#include <memory>
#include <vector>

namespace
{
	constexpr int kShot = 5;
}

class Player : public GameObject
{
public:
    Player(std::weak_ptr<Camera> camera);
    ~Player();

    // �v���C���[�̏������A�I������
    void Init();
    void End();

    // �v���C���[�̍X�V�A�`�揈��
    void Update(Input& input, Boss& boss, std::vector<std::shared_ptr<GroundEnemy>> groundEnemy, 
        std::vector<std::shared_ptr<FlyingEnemy>> flyingEnemy, Map& map);
    void Draw();

    // ���S���̍X�V����
    void DeadUpdate();

    // �_���[�W���󂯂����̏���
    void OnDamage();

	// �v���C���[��HP���擾
	int GetHp() { return m_hp; }

    // �v���C���[�̋�`�����蔻����擾
    float GetLeft();
    float GetRight();
    float GetTop();
    float GetBottom();
    Rect GetRect();

    // �v���C���[�̍��W���擾
    Vec3 GetPos() { return m_pos; }

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
    int m_handleAtk;
    int m_handleDead;

    // �A�j���[�V�����t���O
    bool m_isRun;
    bool m_isJump;
    bool m_isAtk;
    bool m_isDead;

    // �W�����v�֘A
    float m_jumpSpeed;
    int m_jumpCount;

    // �v���C���[�̌���
    bool m_isDirLeft;

    // ���G���Ԋ֘A
    int m_blinkFrameCount;
    int m_deadFrameCount;

    // �v���C���[��HP
    int m_hp;

    // �n�ʁA�V��Ƃ̓����蔻��t���O
    bool m_isGroundHit;
    bool m_isCeilingHit;

    // �W�����v�{�^���̏��
    bool m_isLastJump;
    bool m_isLastJumpButton;

    // �V���b�g
    Shot m_shot[kShot];

    // �A�j���[�V����
    Animation m_idleAnim;
    Animation m_runAnim;
    Animation m_atkAnim;
    Animation m_deadAnim;
};