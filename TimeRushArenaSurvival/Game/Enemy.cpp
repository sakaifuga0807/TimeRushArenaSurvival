#include "stdafx.h"
#include "Enemy.h"

#include "GameClear.h"
#include "GameOver.h"
#include "Player.h"
#include "Game.h"
#include "collision/CollisionObject.h"

#include <time.h>
#include <stdlib.h>

Enemy::Enemy()
{
	//���W�Ƒ傫�����Z�b�g����
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetScale(m_scale);
}

Enemy::~Enemy()
{
	DeleteGO(this);
}

void Enemy::Init(Vector3 initPos)
{
	m_position = initPos;
	m_rotation.SetRotationY(Math::PI);
}

bool Enemy::Start()
{
	////�A�j���[�V������ǂݍ���////
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/enemy/SkeletonIdle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Walk].Load("Assets/animData/enemy/SkeletonWalk.tka");
	m_animationClips[enAnimationClip_Walk].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Run].Load("Assets/animData/enemy/SkeletonRun.tka");
	m_animationClips[enAnimationClip_Run].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Attack].Load("Assets/animData/enemy/SkeletonAttack.tka");
	m_animationClips[enAnimationClip_Attack].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Damage].Load("Assets/animData/enemy/SkeletonDamage.tka");
	m_animationClips[enAnimationClip_Damage].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Down].Load("Assets/animData/enemy/SkeletonDeath.tka");
	m_animationClips[enAnimationClip_Down].SetLoopFlag(false);

	//����ǂݍ���
	g_soundEngine->ResistWaveFileBank(9, "Assets/sound/DamageSound.wav");
	g_soundEngine->ResistWaveFileBank(10, "Assets/sound/EnemySound.wav");
	g_soundEngine->ResistWaveFileBank(11, "Assets/sound/EnemyDeadSoud.wav");

	//�X�P���g���̃��f����ǂݍ���
	m_modelRender.Init("Assets/modelData/Skeleton.tkm");//,m_animationClips,enAnimationClip_Num);
	
	//�L�����R��������������
	m_charaCon.Init(57.0f, 150.0f, m_position);
	m_characonR = 57.0f;

	//���̃{�[����ID���擾����
	m_swordBoneId = m_modelRender.FindBoneID(L"Sword");

	//�v���C���[�̃C���X�^���X�A�h���X����������
	m_player = FindGO<Player>("player");
	m_game = FindGO<Game>("game");
	
	
	//����������������
	srand((unsigned)time(NULL));
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);

	return true;
}

void Enemy::Update()
{
	//�ǐՏ���
	Chase();
	//��]����
	Rotation();
	//�����蔻��
	Collision();
	//�U������
	//Attack();
	//�A�j���[�V�����̍Đ�
	PlayAnimation();
	//�X�e�[�g�Ǘ�
	ManageState();

	IsHit();


	m_modelRender.SetPosition(m_position);
	//���f�����X�V
	m_modelRender.Update();
}

void Enemy::IsHit()
{
	//�G�l�~�[����v���C���[�Ɍ������ĐL�т�x�N�g�����v�Z����
	Vector3 toPlayer = m_player->m_position - m_position;
	//�v���C���[�Ƃ̋������v�Z����
	float distToPlayer = toPlayer.Length();
	//�v���C���[�Ƃ̋�����������x�߂������画������
	if (distToPlayer < m_characonR + m_player->m_characonR+1)
	//�t���O�����ĂȂ�
	if (m_flag == false)
	{
		m_player->m_playerHP--;//�v���C���[��HP�����炷
		//��
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(9);
		se->Play(false);
		se->SetVolume(0.2f);

		m_flag = true;//�t���O�����Ă�
		m_hitCoolTime = 3.0f;//�N�[���^�C��
	}

	//�t���O�𗧂Ă�
	if (m_flag == true)
	{
		m_hitCoolTime -= g_gameTime->GetFrameDeltaTime();//�t���[������
		if (m_hitCoolTime <= 0.0f)
		{
			m_flag = false;//�t���O�����ĂȂ�
		}
	}
}
void Enemy::Rotation()
{
	if (fabsf(m_moveSpeed.x) < 0.001f && fabsf(m_moveSpeed.z) < 0.001f)
	{
		//m_moveSpeed.x��m_moveSpeed.z�̐�Βl���Ƃ���0.001�ȉ��Ƃ������Ƃ�
		//���̃t���[���ł̓L�����͈ړ����Ă��Ȃ��̂Ő��񂷂�K�v�͂Ȃ��B
		return;
	}
	//atan2��tan�Ƃ̒l���p�x(���W�A���P��)�ɕϊ����Ă����֐��B
	//m_moveSpeed.x / m_moveSpeed.z�̌��ʂ�tan�ƂɂȂ�B
	//atan2���g�p���āA�p�x�����߂Ă���B
	//���ꂪ��]�p�x�ɂȂ�B
	float angle = atan2(-m_moveSpeed.x, m_moveSpeed.z);
	//atan���Ԃ��Ă���p�x�̓��W�A���P�ʂȂ̂�
	//SetRotationDeg�ł͂Ȃ�SetRotation���g�p����B
	m_rotation.SetRotationY(-angle);

	//��]��ݒ肷��B
	m_modelRender.SetRotation(m_rotation);

	//�v���C���[�̑O�x�N�g�����v�Z����B
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
}

void Enemy::Chase()
{
	//�ǐՃX�e�[�g�łȂ��Ȃ�ǐՂ��Ȃ�
	if (m_enemyState != enEnemyState_Chase)
	{
		return;
	}

	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	if (m_charaCon.IsOnGround())
	{
		//�n�ʂɂ���
		m_moveSpeed.y - 0.0f;
	}
	Vector3 modelPosition = m_position;
	//������Ƃ������f���̍��W��������
	//modelPosition.y += 2.5f;
	m_modelRender.SetPosition(modelPosition);
}


//�����蔻��̎���
void Enemy::Collision()
{
		//�v���C���[�̍U���p�R���W�������擾����
		const auto& collisions = g_collisionObjectManager->FindCollisionObjects("bullet");
	    //�R���W�����̔z���for���ŉ�
		for (auto collision : collisions)
		{
			//�R���W�����ƃL�����R�����Փ˂�����
			if (collision->IsHit(m_charaCon))
			{
				//HP��1���炷
				m_hp -= 1;

				//��e��
				SoundSource* se2 = NewGO<SoundSource>(0);
				se2->Init(10);
				se2->Play(false);
				se2->SetVolume(0.1f);

				//����hp��0�ɂȂ�����
				if (m_hp <=0)
				{
					m_game->m_numEnemy -= 1;
					////�Q�[���N���A�[����////
 					if ( m_game->m_numEnemy <= 0)
					{
						//�Q�[���N���A�[���Ăяo��
						NewGO<GameClear>(0);
						DeleteGO(this);
						m_gameClear = true;
					}

					//�|���ꂽ�̂ŏ���
					DeleteGO(this);

					//���ʉ����Đ�����
					SoundSource* se3 = NewGO<SoundSource>(0);
					se3->Init(11);
					se3->Play(false);
					se3->SetVolume(0.2f);
				}
				else
				{
					//��_���[�W�X�e�[�g�Ɉڍs����
					m_enemyState = enEnemyState_ReceiveDamage;
				}
				return;
			}
		}
}

//�U��
void Enemy::Attack()
{
	//�U���X�e�[�g�ł͂Ȃ��Ȃ�U�����Ȃ�
	if (m_enemyState != enEnemyState_Attack)
	{
		return;
	}

	//�U������������
	if (m_isUnderAttack == true)
	{
		MakeAttackCollision();
	}
}

//�G�̒Ǐ]����
const bool Enemy::SerchPlayer() const
{
	//�x�N�g���̈����Z������
	Vector3 diff = m_player->GetPosition() - m_position;

	//�v���C���[�ɂ�����x�߂�������
	if (diff.LengthSq() <= 10000.0f * 10000.0f)
	{
		//�G�l�~�[����v���C���[�Ɍ������x�N�g���𐳋K������
		diff.Normalize();
		//�G�l�~�[�̐��ʃx�N�g���ƁA�G�l�~�[����v���C���[�Ɍ�����
		//�x�N�g���̓���(cos��)����p�x�i�Ɓj�����߂�
		float cos = m_forward.Dot(diff);
		//����(cos��)����p�x�i�Ɓj�����߂�
		float angle = acosf(cos);
		//�p�x�i�Ɓj��120����菬�������
		if (angle <= (Math::PI / 180.0f) * 120.0f)
		{
			//�v���C���[��������
			return true;
		}
	}
	//�v���C���[��������Ȃ�����
	return false;
}

void Enemy::MakeAttackCollision()
{
	//�U���̓����蔻��p�̃R���W�����I�u�W�F�N�g���쐬����
	auto collisionObject = NewGO<CollisionObject>(0);
	//���̃{�[���̃��[���h�s����擾����
	Matrix matrix = m_modelRender.GetBone(m_swordBoneId)->GetWorldMatrix();

	//�{�b�N�X��̃R���W�������쐬����
	collisionObject->CreateBox(m_position, Quaternion::Identity, Vector3(100.0f, 10.0f, 10.0f));
	collisionObject->SetWorldMatrix(matrix);
	collisionObject->SetName("enemy_attack");
}

void Enemy::ProcessCommonStateTransition()
{
	//�e�^�C�}�[������������
	m_idleTimer = 0.0f;
	m_chaseTimer = 0.0f;

	//�G�l�~�[����v���C���[�Ɍ������x�N�g�����v�Z����
	Vector3 diff = m_player->GetPosition() - m_position;

	//�v���C���[����������
	if (SerchPlayer() == true)
	{
		//�x�N�g���𐳋K������
		diff.Normalize();
		//�ړ����x��ݒ肷��
		m_moveSpeed = diff * 70.0f;
		//�U���ł��鋗���Ȃ�
		if (IsCanAttack() == true)
		{
			//�����ɂ���āA�U�����邩�ҋ@�����邩���߂�
			int ram = rand() % 100;
			if (ram > 50)
			{
				//�U���X�e�[�g�ɂ���
				m_enemyState = enEnemyState_Attack;
				m_isUnderAttack = false;
				return;
			}
			//�ҋ@�X�e�[�g�ڍs���邩
			else
			{
				//�ҋ@�X�e�[�g�ɂ���
				m_enemyState = enEnemyState_Idle;
				return;
			}
		}
		//�U���ł��Ȃ�������������
		else
		{
			//�����ɂ���āA�ǐՂ����邩���߂�
			int ram = rand() % 100;
			if (ram > 40)
			{
				//�ǐՃX�e�[�g�ɂ���
				m_enemyState = enEnemyState_Chase;
				return;
			}
		}
	}
	//�v���C���[��������Ȃ����
	else
	{
		//�ҋ@�X�e�[�g�ɂ���
		m_enemyState = enEnemyState_Idle;
		return;
	}
}

void Enemy::ProcessIdleStateTransition()
{
	m_idleTimer += g_gameTime->GetFrameDeltaTime();
	//�ҋ@���Ԃ�������x�o�߂���
	if (m_idleTimer >= 0.7f)
	{
		//���̃X�e�[�g�ֈڍs����
		ProcessCommonStateTransition();
	}
}

void Enemy::ProcessAttackStateTransition()
{
	//�ق��̃X�e�[�g�ɍs��
	ProcessCommonStateTransition();
}

void Enemy::ProcessReceiveDamageStateTransition()
{
	//��_���̃A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�U�����ꂽ�狗���֌W�Ȃ��ɂƂ肠�����Ǐ]������
		m_enemyState = enEnemyState_Chase;
		Vector3 diff = m_player->GetPosition() - m_position;
		diff.Normalize();
		//�ړ����x��ݒ肷��
		m_moveSpeed = diff * 70.0f;
	}
}

void Enemy::ProcessDownStateTransition()
{
	//�ق��̃X�e�[�g�ɍs��
	ProcessCommonStateTransition();
}

void Enemy::ProcessChaseStateTransition()
{
	if (IsCanAttack() == true)
	{
		//���̃X�e�[�^�X�ֈڍs������
		ProcessCommonStateTransition();
		return;
	}
	m_chaseTimer += g_gameTime->GetFrameDeltaTime();
	//�ǐՎ��Ԃ�������x�o�߂�����
	if (m_chaseTimer >= 0.8f)
	{
		//���̃X�e�[�g�ֈڍs������
		ProcessCommonStateTransition();
	}
}

void Enemy::ManageState()
{
	switch (m_enemyState)
	{
		//�ҋ@�X�e�[�g
	case enEnemyState_Idle:
		ProcessIdleStateTransition();
		break;
		//�ǐՃX�e�[�g
	case enEnemyState_Chase:
		ProcessChaseStateTransition();
		break;
		//�U���X�e�[�g
	case enEnemyState_Attack:
		ProcessAttackStateTransition();
		break;
		//��_���̃X�e�[�g
	case  enEnemyState_ReceiveDamage:
		ProcessReceiveDamageStateTransition();
		break;
		//�_�E���X�e�[�g
	case enEnemyState_Down:
		ProcessDownStateTransition();
		break;
	}
}

void Enemy::PlayAnimation()
{
	m_modelRender.SetAnimationSpeed(1.0f);
	switch (m_enemyState)
	{
		//�ҋ@�X�e�[�g�̎��B
	case enEnemyState_Idle:
		//�ҋ@�A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.5f);
		break;
		//�ǐՃX�e�[�g�̎��B
	case enEnemyState_Chase:
		m_modelRender.SetAnimationSpeed(1.2f);
		//����A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Run, 0.1f);
		break;
		//�U���X�e�[�g�̎��B
	case enEnemyState_Attack:
		m_modelRender.SetAnimationSpeed(1.6f);
		//�U���A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Attack, 0.1f);
		break;
		//��_���[�W�X�e�[�g�̎��B
	case enEnemyState_ReceiveDamage:
		m_modelRender.SetAnimationSpeed(1.3f);
		//��_���[�W�A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Damage, 0.1f);
		break;
		//�_�E���X�e�[�g�̎��B
	case enEnemyState_Down:
		//�_�E���A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Down, 0.1f);
		break;
	default:
		break;
	}
}


const bool Enemy::IsCanAttack() const
{
	Vector3 diff = m_player->GetPosition() - m_position;
	//�G�l�~�[�ƓG�̋������߂�������
	if (diff.LengthSq() <= 1.0f * 1.0f)
	{
		//�U������
		return true;
	}
	//�U���ł��Ȃ�
	return false;
}


void Enemy::Render(RenderContext&rc)
{
	m_modelRender.Draw(rc);
}

