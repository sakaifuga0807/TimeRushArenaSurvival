#pragma once
//�v���C���[�N���X
class Enemy;

class Player :public IGameObject
{
public:

	enum EnPlayerState
	{
		enPlayerState_GameOver,//�Q�[���N���A�[
		enPlayerState_GameClear,//�Q�[���I�[�o�[
		enPlayerState_GameClear_Idle//�Q�[���N���A�[�i�ҋ@���j
	};

	Player();
	~Player();
	//�X�V����
	void Update();
	//��]����
	void Rotation();
	//�X�e�[�g�Ǘ�
	const void ManageState();
	//�`�揈��
	void Render(RenderContext& rc);
	//�ړ�����
	void Move();
	//�e���쐬
	void MakeBullet();
	//�U��
	void Attack();

	//���W���擾
	const Vector3& GetPosition() const
	{
		return m_position;
	}

	//�����o�ϐ�
	SoundSource* m_bgm=nullptr;
	Vector3 EnemyVec;

	Enemy*m_enemy;
	ModelRender m_modelRender;//���f�������_�[
	Vector3 m_moveSpeed;//�ړ����x
	Quaternion m_rotation;//��]
	Vector3 m_position = Vector3::Zero;//���W
	Vector3 m_forward = Vector3::AxisZ;
	CharacterController m_characterController;//�L�����R��

	int m_MaxHP =10;
	int playerState = 0;//�v���C���[�̏�
	int m_playerHP=10;//�v���C���[��HP
	float m_characonR = 0.0f; //�L�����R���̔��a

private:
	
};
