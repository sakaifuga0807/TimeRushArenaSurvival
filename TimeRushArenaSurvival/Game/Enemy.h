#pragma once

//�N���X�錾
class Player;
class Game;

class Enemy:public	IGameObject
{
public:
	//�G�l�~�[�X�e�[�g
	enum EnEnenemyState
	{
		enEnemyState_Idle,					//�ҋ@�B
		enEnemyState_Chase,					//�ǐՁB
		enEnemyState_Attack,				//�U���B
		enEnemyState_ReceiveDamage,			//��_���[�W�B
		enEnemyState_Down,					//�_�E���B
	};
public:
	//�����o�֐�
	Enemy();
	~Enemy();

	void Update();
	void Render(RenderContext&rc);
	bool Start();

	void Init(Vector3 initPos);
	//��]����
	void Rotation();
	//�U������
	void Attack();
	//�ǐՏ���
	void Chase();
	//�U���p�̃R���W��������鏈��
	void MakeAttackCollision();
	//�����蔻��
	void Collision();
	//�Ǐ]����
	const bool SerchPlayer() const;
	//�A�j���[�V����
	void PlayAnimation();
	//�v���C���[�ƓG�̔���
	void IsHit();


	//HP��ݒ肷��
	void setHP(const int hp)
	{
		m_hp = hp;
	}

	//���W��ݒ�
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}

	//���W���擾
	const Vector3& GetPosition() const
	{
		return m_position;
	}

	//�傫�����擾
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}

	////�X�e�[�g�֌W////
	//�X�e�[�g�Ǘ�
	void ManageState();

	//�ҋ@�X�e�[�g
	void ProcessIdleStateTransition();

	//�ǐՃX�e�[�g
	void ProcessChaseStateTransition();

	//�U���X�e�[�g
	void ProcessAttackStateTransition();

	//��_���X�e�[�g
	void ProcessReceiveDamageStateTransition();

	//�_�E���X�e�[�g
	void ProcessDownStateTransition();

	//�X�e�[�g�̈ڍs����
	void ProcessCommonStateTransition();

	//�U���ł��鋗�����ǂ����𒲂ׂ�
	//�U���ł���Ȃ�true
	const bool IsCanAttack() const;
	//�A�j���[�V�����C�x���g�p�̊֐�
	//void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);


	//�A�j���[�V�����̃X�e�[�g
	enum EnAnimationClip
	{
		enAnimationClip_Idle,
		enAnimationClip_Walk,
		enAnimationClip_Run,
		enAnimationClip_Attack,
		enAnimationClip_Damage,
		enAnimationClip_Down,
		enAnimationClip_Num,
	};

	//�����o�ϐ�
	bool m_gameClear = false;
	int m_stopTimer;
	Game*m_game;
	SoundSource* m_bgm = nullptr;
	bool m_GameClearFlag=false;
	int m_DefeatedEnemy = 50;
	AnimationClip m_animationClips[enAnimationClip_Num];//�A�j���[�V�����N���b�v
	EnEnenemyState m_enemyState = enEnemyState_Idle;//�G�̃X�e�[�g
	Quaternion m_rotation;
	Vector3 m_moveVector;//�ړ������x�N�g��
	Vector3 m_position;//���W
	ModelRender m_modelRender;//���f�������_�[
	Vector3 m_moveSpeed;//�ړ����x
	CharacterController m_charaCon;//�L�����R��

	int m_swordBoneId = -1;
	bool m_isUnderAttack = false;//�U�������H
	Vector3 m_forward = Vector3::AxisZ;//�G�̐��ʃx�N�g��
	Player* m_player = nullptr;//�v���C���[
	Vector3 m_scale = Vector3(15.0f,15.0f,15.0f);//�X�P�[��
	int m_hp = 6;//HP
	float m_idleTimer=0.0f ;//�ǐՃ^�C�}�[
	float m_chaseTimer=0.0f;//�ҋ@�^�C�}�[
	float m_characonR = 0.0f; //�L�����R���̔��a
	float m_hitCoolTime = 3.0f;//��������N�[���^�C��
	float m_GameClearDrawTime = 1.5f;//�Q�[���N���A��\��������^�C�}�[
	bool m_flag = true;//�����true�œ��삳����
};

