#include "stdafx.h"
#include "GameCamera.h"
#include "Player.h"
#include "Bullet.h"

GameCamera::GameCamera()
{
	
}

GameCamera::~GameCamera()
{
	DeleteGO(this);
}

bool GameCamera::Start()
{
	//�����_���王�_�܂ł̃x�N�g����ݒ�B
	m_toCameraPos.Set(1.0f, 125.0f, -60.0f);
	//�J�����̍��W���擾����
	//�v���C���[�̃C���X�^���X��T���B
	m_player = FindGO<Player>("player");

	//�J�����̃j�A�N���b�v�ƃt�@�[�N���b�v��ݒ肷��B
	g_camera3D->SetNear(1.0f);
	g_camera3D->SetFar(20000.0f);



	return true;
}

void GameCamera::Update()
{
	//�J�������X�V�B
    //�����_���v�Z����B
	Vector3 target = m_player->m_position;
	//�v���C��
	target.y += 115.0f;

	Vector3 toCameraPosOld = m_toCameraPos;
	//�p�b�h�̓��͂��g���ăJ�������񂷁B
	float x = g_pad[0]->GetRStickXF();
	float y = g_pad[0]->GetRStickYF();
	//Y������̉�]
	Quaternion qRot;
	qRot.SetRotationDeg(Vector3::AxisY, 1.8f * x);
	qRot.Apply(m_toCameraPos);
	//X������̉�]�B
	Vector3 axisX;
	axisX.Cross(Vector3::AxisY, m_toCameraPos);
	axisX.Normalize();
	qRot.SetRotationDeg(axisX, 1.45f * y);
	qRot.Apply(m_toCameraPos);
	//�J�����̉�]�̏�����`�F�b�N����B
	//�����_���王�_�܂ł̃x�N�g���𐳋K������B
	//���K������ƁA�x�N�g���̑傫�����P�ɂȂ�B
	//�傫�����P�ɂȂ�Ƃ������Ƃ́A�x�N�g�����狭�����Ȃ��Ȃ�A�����݂̂̏��ƂȂ�Ƃ������ƁB
	Vector3 toPosDir = m_toCameraPos;
	toPosDir.Normalize();
	/*if (toPosDir.y < -0.1f) {
		//�J����������������B
		m_toCameraPos = toCameraPosOld;
	}
	else if (toPosDir.y > 0.1f) {
		//�J�����������������B
		m_toCameraPos = toCameraPosOld;
	}*/

	//���_���v�Z����B
	Vector3 pos = target + m_toCameraPos;
	//���C���J�����ɒ����_�Ǝ��_��ݒ肷��B
	g_camera3D->SetTarget(target);
	g_camera3D->SetPosition(pos);

	//�J�����̍X�V�B
	g_camera3D->Update();
}

void GameCamera::CameraVector()
{
	//�J�����̃��[���h���W���擾
	Vector3 cameraPosition = g_camera3D->GetPosition();
	Vector3 targetPosition = cameraPosition + m_toCameraPos;//�����_���W

	//�J�����̕����x�N�g�����v�Z�i�����_-�J�������W�j
	Vector3 cameraDirection = targetPosition - cameraPosition;

	//�J�����̌������K���i���K���x�N�g���j
	cameraDirection.Normalize();

	//�e�̏����ʒu�i�J�����̏����O�ɐ����j
	Vector3 bulletStartPosition = cameraPosition + cameraDirection * 2.0f;

	//�e�̑��x�x�N�g���i�J�����̌���*���x�j
	float bulletSpeed = 2000.0f;
	Vector3 bulletVelocity = cameraDirection * bulletSpeed;

	////�e�̃C���X�^���X���쐬
	//Bullet* bullet = NewGO<bullet>(0);

	////�����ʒu���Z�b�g
	//bullet->SetPosition(bulletStartPosition);
	//bullet->SetVelocity(bulletVelocity);
}


