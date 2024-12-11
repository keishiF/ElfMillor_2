#pragma once
#include "Vec3.h"
#include <memory>

class Player;

/// <summary>
/// �J����
/// ���W
/// �I�u�W�F�N�g�����̍��W�Ƃ̑��΋����ŕ`�悵����
/// </summary>
class Camera
{
public:

	Camera();
	~Camera();

	void Init(std::weak_ptr<Player> player);
	void End();
	void Update();

	Vec3 Capture(const Vec3& pos) const
	{
		return m_offset + pos;
	}

	Vec3 m_pos;
	Vec3 m_offset;

	std::weak_ptr<Player> m_player;
};