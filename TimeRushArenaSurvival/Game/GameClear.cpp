#include "stdafx.h"
#include "GameClear.h"
#include "Title.h"
#include "Game.h"
#include "Enemy.h"
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

GameClear::GameClear()
{

	//�Q�[���N���A�̉摜��ǂݍ���
	m_spriteRender_S.Init("Assets/sprite/S.dds", 1920.0f, 1080.0f);
	m_spriteRender_A.Init("Assets/sprite/A.dds", 1920.0f, 1080.0f);
	m_spriteRender_B.Init("Assets/sprite/B.dds", 1920.0f, 1080.0f);
	m_spriteRender_C.Init("Assets/sprite/C.dds", 1920.0f, 1080.0f);

	//���ʉ���ǂݍ���
	g_soundEngine->ResistWaveFileBank(4, "Assets/sound/GameClear.wav");
	g_soundEngine->ResistWaveFileBank(5, "Assets/sound/Enter.wav");
	
	m_game = FindGO<Game>("game");

	//BGM
	m_bgm = NewGO<SoundSource>(0);
	m_bgm->Init(4);
	m_bgm->Play(true);
	m_bgm->SetVolume(0.3f);
}

GameClear::~GameClear()
{
	DeleteGO(this);
	DeleteGO(m_bgm);
}

void GameClear::Update()
{
	TimerUI();

	if (g_pad[0]->IsTrigger(enButtonB))
	{
		//�^�C�g���̃I�u�W�F�N�g�����
		NewGO<Title>(0);
		//���̍Đ�
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(1);
		se->Play(false);
		se->SetVolume(1.0f);

		//���g���폜
		DeleteGO(this);
	}
	m_spriteRender_S.Update();
}

//m_game->m_stopTimer

void GameClear::TimerUI()
{
	const float m_stopTimer = m_game->m_stopTimer;
	wchar_t wtimer[256];
	swprintf_s(wtimer, 256, L"%dS", int(m_stopTimer));

	//�\������e�L�X�g��ݒ�B
	m_TimerfontRender.SetText(wtimer);
	//�t�H���g�̈ʒu��ݒ�B
	m_TimerfontRender.SetPosition(Vector3(-610, -150.0f, 0.0f));
	//�t�H���g�̑傫����ݒ�B
	m_TimerfontRender.SetScale(2.5f);
	//�t�H���g�̐F��ݒ�B
	m_TimerfontRender.SetColor(g_vec4Black);
	
}

void GameClear::Render(RenderContext&rc)
{
	if (m_game->m_timerlimit >= 150)
	{
		m_spriteRender_S.Draw(rc);
	}
	if (m_game->m_timerlimit<150&&m_game->m_timerlimit >= 100)
	{
		m_spriteRender_A.Draw(rc);
	}
	if (m_game->m_timerlimit < 100 && m_game->m_timerlimit >= 50)
	{
		m_spriteRender_B.Draw(rc);
	}
	if (m_game->m_timerlimit < 50 && m_game->m_timerlimit >= 0)
	{
		m_spriteRender_C.Draw(rc);
	}

	m_TimerfontRender.Draw(rc);
}