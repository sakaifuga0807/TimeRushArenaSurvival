#pragma once

//�^�C�g���N���X
class Title :public IGameObject
{
public:
	Title();
	~Title();
	//�����o�֐�
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	//�����o�ϐ�
	SpriteRender m_spriteRender;//�X�v���C�g�����_�[
	SoundSource*m_bgm = nullptr;//�^�C�g����BGM
};