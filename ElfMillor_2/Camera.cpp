#include "Camera.h"
#include "Player.h"
#include "game.h"

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::Init(std::weak_ptr<Player> player)
{
	m_player = player;
}

void Camera::End()
{
}

void Camera::Update()
{
	// ’Ç”ö
	m_pos = m_player.lock()->m_pos;
	
	// ƒJƒƒ‰‚ÌˆÊ’u‚ğ’†S‚É
	m_offset = m_pos;
	m_offset.x *= -1;
	m_offset.y *= -1;
	m_offset.x += Game::kScreenWidth * 0.5f;
	m_offset.y += Game::kScreenHeight * 0.5f;

	//DrawFormatString(0, 60, 0xffffff, "CameraPosX=%f, Y=%f", m_pos.x, m_pos.y);
	//DrawFormatString(0, 75, 0xffffff, "OffsetX=%f, Y=%f", m_offset.x, m_offset.y);
}