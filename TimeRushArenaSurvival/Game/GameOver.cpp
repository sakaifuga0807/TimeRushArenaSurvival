#include "stdafx.h"
#include "GameOver.h"
#include "Title.h"

#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

GameOver::GameOver()
{
	m_spriteRender.Init("Assets/sprite/GameOver.dds", 1920.0f, 1080.0f);
}

GameOver::~GameOver()
{
	DeleteGO(this);
	DeleteGO(m_bgm);
}

bool GameOver::Start()
{
	//‰¹‚Ì“Ç‚İ‚İ
	g_soundEngine->ResistWaveFileBank(2, "Assets/sound/GameOver.wav");
	g_soundEngine->ResistWaveFileBank(3, "Assets/sound/Enter.wav");

	//BGM
	m_bgm = NewGO<SoundSource>(0);
	m_bgm->Init(2);
	m_bgm->Play(true);
	m_bgm->SetVolume(0.3f);
	return true;
}

void GameOver::Update()
{
	if (g_pad[0]->IsPress(enButtonA))
	{
		NewGO<Title>(0);
		//Œˆ’è‰¹‚ğÄ¶
		SoundSource* se = NewGO<SoundSource>(1);
		se->Init(1);
		se->Play(false);
		se->SetVolume(1.0f);
	}
}

void GameOver::Render(RenderContext& rc)
{
	m_spriteRender.Draw(rc);
}