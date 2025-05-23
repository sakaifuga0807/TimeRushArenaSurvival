#include "stdafx.h"
#include "BackGround.h"

#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

BackGround::BackGround()
{
	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();

	//音を読み込む
	g_soundEngine->ResistWaveFileBank(7, "Assets/sound/Gameplay.wav");

	//BGM
	m_bgm = NewGO<SoundSource>(0);
	m_bgm->Init(7);
	m_bgm->Play(true);
	m_bgm->SetVolume(0.2f);

	//ステージの読み込み
	modelRender.Init("Assets/stage/ground.tkm");
	modelRender.Update();
	physicsStaticObject.CreateFromModel(modelRender.GetModel(),modelRender.GetModel().GetWorldMatrix());
}

BackGround::~BackGround()
{
	DeleteGO(this);
	DeleteGO(m_bgm);
}

void BackGround::Render(RenderContext& rc)
{
	modelRender.Draw(rc);
}