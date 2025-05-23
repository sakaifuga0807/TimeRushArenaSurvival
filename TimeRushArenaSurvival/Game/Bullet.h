#pragma once

//class Bullet;
class Player;
class Enemy;
class GameCamera;
#include "graphics/effect/EffectEmitter.h"

class Bullet :public IGameObject
{
public:
	Bullet();
	~Bullet();
	void Update();
	//�R���W�����쐬
	void CreateCollision();
	//�G�t�F�N�g�쐬
	void CreateEffect();
	void Render(RenderContext& rc);
	bool Start();
	//���W���Z�b�g
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}

	//��]���Z�b�g
	void SetRotaiton(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}

	//�ړ����x
	void SetVelocity(const Vector3& velocity)
	{
		m_velocity = velocity;
	}

	//���W���擾
	const Vector3& GetPosition() const
	{
		return m_position;
	}

	//�����o�ϐ�
	Enemy* m_enemy;
	const float bulletSpeed = 2000.0f;
	Vector3 m_toCameraPos;//�����_���王�_�Ɍ������x�N�g��
	Vector3 m_velocity;
	SoundSource* m_bgm = nullptr;
	GameCamera*m_gameCamera;
	CollisionObject* m_collisionObject;//�R���W�����I�u�W�F�N�g
	int Damage = 1;
	ModelRender m_modelRender;//���f�������_�[
	float m_timer = 0.0f;//�^�C�}�[
	Player* m_player;//�v���C���[
	Vector3 m_position;//���W
	Vector3 m_direction; //�e�̈ړ�����
	Vector3 m_moveSpeed;//�ړ����x
	Vector3 m_scale = Vector3::One;//�傫��
	Quaternion m_rotation;//��]

	EffectEmitter* m_effectEmitter;
};