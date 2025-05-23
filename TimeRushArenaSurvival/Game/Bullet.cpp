#include "stdafx.h"
#include "Bullet.h"
#include "Player.h"
#include "Enemy.h"
#include "collision/CollisionObject.h"
#include "GameCamera.h"

#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"


Bullet::Bullet()
{
}

Bullet::~Bullet()
{
	DeleteGO(m_collisionObject);
	DeleteGO(m_effectEmitter);
}


bool Bullet::Start()
{
	//�C���X�^���X�A�h���X����������
	m_gameCamera = FindGO<GameCamera>("gamecamera");
	m_enemy = FindGO<Enemy>("skeleton");

	//�G�t�F�N�g�����[�h����
	EffectEngine::GetInstance()->ResistEffect(0,u"Assets/effect/black.efkefc");	
	

	//����ǂݍ���
	g_soundEngine->ResistWaveFileBank(6, "Assets/sound/BulletSound.wav");

	Matrix cameraMatrix = g_camera3D->GetCameraRotation();
	m_direction = Vector3(cameraMatrix.m[2][0], cameraMatrix.m[2][1], cameraMatrix.m[2][2]);
	m_direction.Normalize();

	////�ړ����x���v�Z����////
	m_moveSpeed = m_direction * bulletSpeed;


	CreateCollision();	

	CreateEffect();

	return true;
}

void Bullet::Update()
{
	//�Q�[���N���A�[�����Ƃ��ɒe�𔭎˂��Ȃ��悤�ɂ���
	if (m_enemy->m_DefeatedEnemy <= 0)
	{
		m_moveSpeed *= 0.0f;
	}

	//���W���ړ�������
	m_position += m_moveSpeed * g_gameTime->GetFrameDeltaTime() * 1.5f;
	if (m_effectEmitter->GetEffect() != nullptr) {
		m_effectEmitter->SetPosition(m_position);
	}

    //�R���W�����I�u�W�F�N�g�ɍ��W��ݒ肷��
	m_collisionObject->SetPosition(m_position);
	//effectEmitter->SetPosition(m_position);

	//�^�C�}�[�����Z����
	m_timer += g_gameTime->GetFrameDeltaTime();

	//�^�C�}�[��0.48�ȏゾ������
	if (m_timer >= 0.48f)
	{
		//���g���폜����
		DeleteGO(this);
		m_effectEmitter->Stop();
	}
}

void Bullet::CreateCollision()
{

	//BGM
	SoundSource* se = NewGO<SoundSource>(0);
	se->Init(6);
	se->Play(false);
	se->SetVolume(0.8f);

	//�J�����̌��݈ʒu���擾�i���_�ʒu�j
	Vector3 cameraPosition = g_camera3D->GetPosition();
	
	//�R���W�����I�u�W�F�N�g���쐬
	m_collisionObject = NewGO<CollisionObject>(0);

	//����̃R���W�������쐬����
	m_collisionObject->CreateBox(m_position, Quaternion::Identity,{45.0f,45.0f,45.0f});

	//�v���C���[�̒e�̖��O
	m_collisionObject->SetName("bullet");

	//�I�u�W�F�N�g�������ō폜����Ȃ��悤�ɂ���
	m_collisionObject->SetIsEnableAutoDelete(false);
}

void Bullet::CreateEffect()
{
	m_effectEmitter = NewGO<EffectEmitter>(0);
	m_effectEmitter->Init(0);
	m_effectEmitter->SetScale({ 55.0f,55.0f,55.0f });
	//�G�t�F�N�g�̍��W���Z�b�g����
	m_effectEmitter->SetPosition(m_position);
	m_effectEmitter->Play();
}



void Bullet::Render(RenderContext& rc)
{
	//m_modelRender.Draw(rc);
}