#include "stdafx.h"
#include "Player.h"
#include "Bullet.h"
#include "GameOver.h"
#include "Enemy.h"
#include "GameClear.h"

#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

Player::Player()
{
	//音の読み込み
	g_soundEngine->ResistWaveFileBank(8, "Assets/sound/FootSteps.wav");

	/*//足音
	SoundSource* se = NewGO<SoundSource>(0);
	se->Init(8);
	se->Play(false);
	se->SetVolume(0.3f);*/

	//モデル読み込み
	m_modelRender.Init("Assets/modelData/unityChan.tkm");
	//キャラコンを初期化
	m_characterController.Init(23.0f, 75.0f, m_position);
	m_characonR = 25.0f;
	m_position.Set(0.0f, 0.0f, 0.0f);

	//敵のインスタンスを検索する
	m_enemy = FindGO<Enemy>("enemy");
}

Player::~Player()
{
	DeleteGO(this);
}

void Player::Update()
{
	//移動処理
	Move();

	//回転処理
	//Rotation();

	//ステート管理
	ManageState();

	//弾を発射
	Attack();

	//モデルを更新する
	m_modelRender.Update();
}


void Player::Attack()
{
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		MakeBullet();
	}
}

void Player::Move()
{
	//xzの移動速度を0.0fにする
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;

	//左スティックの入力量を取得
	Vector3 stickL;
	stickL.x = g_pad[0]->GetLStickXF();
	stickL.y = g_pad[0]->GetLStickYF();

	//カメラの前方向と右方向のベクトルを持ってくる
	Vector3 forward = g_camera3D->GetForward();
	Vector3 right = g_camera3D->GetRight();
	//y方向には移動させない
	forward.y = 0.0f;
	right.y = 0.0f;

	//左スティックの入力量と750を乗算
	right *= stickL.x * 750.0f;
	forward *= stickL.y * 750.0f;


	//移動速度にスティックの入力量を加算する
	m_moveSpeed += right + forward;

	//キャラコンを使って座標を移動させる
	m_position = m_characterController.Execute(m_moveSpeed, 1.0f / 60.0f);

	m_position = m_characterController.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	if (m_characterController.IsOnGround())
	{
		m_moveSpeed.y = 0.0f;
	}

	//絵描きさんに座標を教える
	m_modelRender.SetPosition(m_position);
}

void Player::Rotation()
{
	if (fabsf(m_moveSpeed.x) < 0.001f
		&& fabsf(m_moveSpeed.z) < 0.001f)
	{
		//m_moveSpeed.xとm_moveSpeed.zの絶対値がともに0.001以下ということは
		//このフレームではキャラは移動していないので旋回する必要はない。
		return;
	}
	//atan2はtanθの値を角度(ラジアン単位)に変換してくれる関数。
	//m_moveSpeed.x / m_moveSpeed.zの結果はtanθになる。
	//atan2を使用して、角度を求めている。
	//これが回転角度になる。
	float angle = atan2(-m_moveSpeed.x, m_moveSpeed.z);
	//atanが返してくる角度はラジアン単位なので
	//SetRotationDegではなくSetRotationを使用する。
	m_rotation.SetRotationY(-angle);

	//回転を設定する
	m_modelRender.SetRotation(m_rotation);

	//プレイヤーの正面ベクトルを計算する
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
}

void Player::MakeBullet()
{
	//弾を作成する
	Bullet* bullet = NewGO<Bullet>(0);
	Vector3 bulletPosition = m_position;
	//座標を少し上げる
	bulletPosition.y += 70.0f;
	//座標をセットする
	bullet->SetPosition(bulletPosition);
	bullet->SetRotaiton(m_rotation);
	bullet->SetName("bullet");
	//bullet;
}

//ステート管理
const void Player::ManageState()
{
	//HPが0になったらゲームオーバーを呼び出す
	if (m_playerHP == 0)
	{
		NewGO<GameOver>;
		DeleteGO(this);
	}
}

//描画処理
void Player::Render(RenderContext& renderContext)
{
	//モデルを表示する
	//m_modelRender.Draw(renderContext);

}