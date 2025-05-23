#include "stdafx.h"
#include "Title.h"
#include "Game.h"

#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

//�R���X�g���N�^
Title::Title()
{
	
}

//�f�X�g���N�^
Title::~Title()
{
	DeleteGO(this);
	DeleteGO(m_bgm);
}

bool Title::Start()
{
	m_spriteRender.Init("Assets/sprite/TimeRushArenaSurvival.dds", 1920.0f, 1080.0f);

	//����ǂݍ���
	g_soundEngine->ResistWaveFileBank(0, "Assets/sound/Title.wav");
	//���艹
	g_soundEngine->ResistWaveFileBank(1, "Assets/sound/Enter.wav");

	//BGM
	m_bgm = NewGO<SoundSource>(0);
	m_bgm->Init(0);
	m_bgm->Play(true);
	m_bgm->SetVolume(0.3f);
	return true;
}

//�X�V����
void Title::Update()
{
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		NewGO<Game>(0, "game");
		m_spriteRender.Update();
		//���ʉ����Đ�����
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(1);
		se->Play(false);
		se->SetVolume(1.0f);
		DeleteGO(this);
	}
}

//�����_�[
void Title::Render(RenderContext & rc)
{
	m_spriteRender.Draw(rc);
}