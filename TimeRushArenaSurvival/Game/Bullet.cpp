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
	//インスタンスアドレスを検索する
	m_gameCamera = FindGO<GameCamera>("gamecamera");
	m_enemy = FindGO<Enemy>("skeleton");

	//エフェクトをロードする
	EffectEngine::GetInstance()->ResistEffect(0,u"Assets/effect/black.efkefc");	
	

	//音を読み込む
	g_soundEngine->ResistWaveFileBank(6, "Assets/sound/BulletSound.wav");

	Matrix cameraMatrix = g_camera3D->GetCameraRotation();
	m_direction = Vector3(cameraMatrix.m[2][0], cameraMatrix.m[2][1], cameraMatrix.m[2][2]);
	m_direction.Normalize();

	////移動速度を計算する////
	m_moveSpeed = m_direction * bulletSpeed;


	CreateCollision();	

	CreateEffect();

	return true;
}

void Bullet::Update()
{
	//ゲームクリアーしたときに弾を発射しないようにする
	if (m_enemy->m_DefeatedEnemy <= 0)
	{
		m_moveSpeed *= 0.0f;
	}

	//座標を移動させる
	m_position += m_moveSpeed * g_gameTime->GetFrameDeltaTime() * 1.5f;
	if (m_effectEmitter->GetEffect() != nullptr) {
		m_effectEmitter->SetPosition(m_position);
	}

    //コリジョンオブジェクトに座標を設定する
	m_collisionObject->SetPosition(m_position);
	//effectEmitter->SetPosition(m_position);

	//タイマーを加算する
	m_timer += g_gameTime->GetFrameDeltaTime();

	//タイマーが0.48以上だったら
	if (m_timer >= 0.48f)
	{
		//自身を削除する
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

	//カメラの現在位置を取得（視点位置）
	Vector3 cameraPosition = g_camera3D->GetPosition();
	
	//コリジョンオブジェクトを作成
	m_collisionObject = NewGO<CollisionObject>(0);

	//箱状のコリジョンを作成する
	m_collisionObject->CreateBox(m_position, Quaternion::Identity,{45.0f,45.0f,45.0f});

	//プレイヤーの弾の名前
	m_collisionObject->SetName("bullet");

	//オブジェクトが自動で削除されないようにする
	m_collisionObject->SetIsEnableAutoDelete(false);
}

void Bullet::CreateEffect()
{
	m_effectEmitter = NewGO<EffectEmitter>(0);
	m_effectEmitter->Init(0);
	m_effectEmitter->SetScale({ 55.0f,55.0f,55.0f });
	//エフェクトの座標をセットする
	m_effectEmitter->SetPosition(m_position);
	m_effectEmitter->Play();
}



void Bullet::Render(RenderContext& rc)
{
	//m_modelRender.Draw(rc);
}