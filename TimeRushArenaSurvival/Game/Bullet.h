#pragma once

//class Bullet;
class Player;
class Enemy;
class GameCamera;
#include "graphics/effect/EffectEmitter.h"

class Bullet :public IGameObject
{
public:
	Bullet();
	~Bullet();
	void Update();
	//コリジョン作成
	void CreateCollision();
	//エフェクト作成
	void CreateEffect();
	void Render(RenderContext& rc);
	bool Start();
	//座標をセット
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}

	//回転をセット
	void SetRotaiton(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}

	//移動速度
	void SetVelocity(const Vector3& velocity)
	{
		m_velocity = velocity;
	}

	//座標を取得
	const Vector3& GetPosition() const
	{
		return m_position;
	}

	//メンバ変数
	Enemy* m_enemy;
	const float bulletSpeed = 2000.0f;
	Vector3 m_toCameraPos;//注視点から視点に向かうベクトル
	Vector3 m_velocity;
	SoundSource* m_bgm = nullptr;
	GameCamera*m_gameCamera;
	CollisionObject* m_collisionObject;//コリジョンオブジェクト
	int Damage = 1;
	ModelRender m_modelRender;//モデルレンダー
	float m_timer = 0.0f;//タイマー
	Player* m_player;//プレイヤー
	Vector3 m_position;//座標
	Vector3 m_direction; //弾の移動方向
	Vector3 m_moveSpeed;//移動速度
	Vector3 m_scale = Vector3::One;//大きさ
	Quaternion m_rotation;//回転

	EffectEmitter* m_effectEmitter;
};