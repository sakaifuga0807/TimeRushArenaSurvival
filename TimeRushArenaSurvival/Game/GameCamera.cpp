#include "stdafx.h"
#include "GameCamera.h"
#include "Player.h"
#include "Bullet.h"

GameCamera::GameCamera()
{
	
}

GameCamera::~GameCamera()
{
	DeleteGO(this);
}

bool GameCamera::Start()
{
	//注視点から視点までのベクトルを設定。
	m_toCameraPos.Set(1.0f, 125.0f, -60.0f);
	//カメラの座標を取得する
	//プレイヤーのインスタンスを探す。
	m_player = FindGO<Player>("player");

	//カメラのニアクリップとファークリップを設定する。
	g_camera3D->SetNear(1.0f);
	g_camera3D->SetFar(20000.0f);



	return true;
}

void GameCamera::Update()
{
	//カメラを更新。
    //注視点を計算する。
	Vector3 target = m_player->m_position;
	//プレイヤ
	target.y += 115.0f;

	Vector3 toCameraPosOld = m_toCameraPos;
	//パッドの入力を使ってカメラを回す。
	float x = g_pad[0]->GetRStickXF();
	float y = g_pad[0]->GetRStickYF();
	//Y軸周りの回転
	Quaternion qRot;
	qRot.SetRotationDeg(Vector3::AxisY, 1.8f * x);
	qRot.Apply(m_toCameraPos);
	//X軸周りの回転。
	Vector3 axisX;
	axisX.Cross(Vector3::AxisY, m_toCameraPos);
	axisX.Normalize();
	qRot.SetRotationDeg(axisX, 1.45f * y);
	qRot.Apply(m_toCameraPos);
	//カメラの回転の上限をチェックする。
	//注視点から視点までのベクトルを正規化する。
	//正規化すると、ベクトルの大きさが１になる。
	//大きさが１になるということは、ベクトルから強さがなくなり、方向のみの情報となるということ。
	Vector3 toPosDir = m_toCameraPos;
	toPosDir.Normalize();
	/*if (toPosDir.y < -0.1f) {
		//カメラが上向きすぎ。
		m_toCameraPos = toCameraPosOld;
	}
	else if (toPosDir.y > 0.1f) {
		//カメラが下向きすぎ。
		m_toCameraPos = toCameraPosOld;
	}*/

	//視点を計算する。
	Vector3 pos = target + m_toCameraPos;
	//メインカメラに注視点と視点を設定する。
	g_camera3D->SetTarget(target);
	g_camera3D->SetPosition(pos);

	//カメラの更新。
	g_camera3D->Update();
}

void GameCamera::CameraVector()
{
	//カメラのワールド座標を取得
	Vector3 cameraPosition = g_camera3D->GetPosition();
	Vector3 targetPosition = cameraPosition + m_toCameraPos;//注視点座標

	//カメラの方向ベクトルを計算（注視点-カメラ座標）
	Vector3 cameraDirection = targetPosition - cameraPosition;

	//カメラの向きを習得（正規化ベクトル）
	cameraDirection.Normalize();

	//弾の初期位置（カメラの少し前に生成）
	Vector3 bulletStartPosition = cameraPosition + cameraDirection * 2.0f;

	//弾の速度ベクトル（カメラの向き*速度）
	float bulletSpeed = 2000.0f;
	Vector3 bulletVelocity = cameraDirection * bulletSpeed;

	////弾のインスタンスを作成
	//Bullet* bullet = NewGO<bullet>(0);

	////初期位置をセット
	//bullet->SetPosition(bulletStartPosition);
	//bullet->SetVelocity(bulletVelocity);
}


