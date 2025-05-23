#include "stdafx.h"
#include "Enemy.h"

#include "GameClear.h"
#include "GameOver.h"
#include "Player.h"
#include "Game.h"
#include "collision/CollisionObject.h"

#include <time.h>
#include <stdlib.h>

Enemy::Enemy()
{
	//座標と大きさをセットする
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetScale(m_scale);
}

Enemy::~Enemy()
{
	DeleteGO(this);
}

void Enemy::Init(Vector3 initPos)
{
	m_position = initPos;
	m_rotation.SetRotationY(Math::PI);
}

bool Enemy::Start()
{
	////アニメーションを読み込む////
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/enemy/SkeletonIdle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Walk].Load("Assets/animData/enemy/SkeletonWalk.tka");
	m_animationClips[enAnimationClip_Walk].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Run].Load("Assets/animData/enemy/SkeletonRun.tka");
	m_animationClips[enAnimationClip_Run].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Attack].Load("Assets/animData/enemy/SkeletonAttack.tka");
	m_animationClips[enAnimationClip_Attack].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Damage].Load("Assets/animData/enemy/SkeletonDamage.tka");
	m_animationClips[enAnimationClip_Damage].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Down].Load("Assets/animData/enemy/SkeletonDeath.tka");
	m_animationClips[enAnimationClip_Down].SetLoopFlag(false);

	//音を読み込む
	g_soundEngine->ResistWaveFileBank(9, "Assets/sound/DamageSound.wav");
	g_soundEngine->ResistWaveFileBank(10, "Assets/sound/EnemySound.wav");
	g_soundEngine->ResistWaveFileBank(11, "Assets/sound/EnemyDeadSoud.wav");

	//スケルトンのモデルを読み込む
	m_modelRender.Init("Assets/modelData/Skeleton.tkm");//,m_animationClips,enAnimationClip_Num);
	
	//キャラコンを初期化する
	m_charaCon.Init(57.0f, 150.0f, m_position);
	m_characonR = 57.0f;

	//剣のボーンのIDを取得する
	m_swordBoneId = m_modelRender.FindBoneID(L"Sword");

	//プレイヤーのインスタンスアドレスを検索する
	m_player = FindGO<Player>("player");
	m_game = FindGO<Game>("game");
	
	
	//乱数を初期化する
	srand((unsigned)time(NULL));
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);

	return true;
}

void Enemy::Update()
{
	//追跡処理
	Chase();
	//回転処理
	Rotation();
	//当たり判定
	Collision();
	//攻撃処理
	//Attack();
	//アニメーションの再生
	PlayAnimation();
	//ステート管理
	ManageState();

	IsHit();


	m_modelRender.SetPosition(m_position);
	//モデルを更新
	m_modelRender.Update();
}

void Enemy::IsHit()
{
	//エネミーからプレイヤーに向かって伸びるベクトルを計算する
	Vector3 toPlayer = m_player->m_position - m_position;
	//プレイヤーとの距離を計算する
	float distToPlayer = toPlayer.Length();
	//プレイヤーとの距離がある程度近かったら判定を取る
	if (distToPlayer < m_characonR + m_player->m_characonR+1)
	//フラグをたてない
	if (m_flag == false)
	{
		m_player->m_playerHP--;//プレイヤーのHPを減らす
		//音
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(9);
		se->Play(false);
		se->SetVolume(0.2f);

		m_flag = true;//フラグをたてる
		m_hitCoolTime = 3.0f;//クールタイム
	}

	//フラグを立てる
	if (m_flag == true)
	{
		m_hitCoolTime -= g_gameTime->GetFrameDeltaTime();//フレームごと
		if (m_hitCoolTime <= 0.0f)
		{
			m_flag = false;//フラグをたてない
		}
	}
}
void Enemy::Rotation()
{
	if (fabsf(m_moveSpeed.x) < 0.001f && fabsf(m_moveSpeed.z) < 0.001f)
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

	//回転を設定する。
	m_modelRender.SetRotation(m_rotation);

	//プレイヤーの前ベクトルを計算する。
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
}

void Enemy::Chase()
{
	//追跡ステートでないなら追跡しない
	if (m_enemyState != enEnemyState_Chase)
	{
		return;
	}

	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	if (m_charaCon.IsOnGround())
	{
		//地面についた
		m_moveSpeed.y - 0.0f;
	}
	Vector3 modelPosition = m_position;
	//ちょっとだけモデルの座標をあげる
	//modelPosition.y += 2.5f;
	m_modelRender.SetPosition(modelPosition);
}


//当たり判定の実装
void Enemy::Collision()
{
		//プレイヤーの攻撃用コリジョンを取得する
		const auto& collisions = g_collisionObjectManager->FindCollisionObjects("bullet");
	    //コリジョンの配列をfor文で回す
		for (auto collision : collisions)
		{
			//コリジョンとキャラコンが衝突したら
			if (collision->IsHit(m_charaCon))
			{
				//HPを1減らす
				m_hp -= 1;

				//被弾音
				SoundSource* se2 = NewGO<SoundSource>(0);
				se2->Init(10);
				se2->Play(false);
				se2->SetVolume(0.1f);

				//もしhpが0になったら
				if (m_hp <=0)
				{
					m_game->m_numEnemy -= 1;
					////ゲームクリアー条件////
 					if ( m_game->m_numEnemy <= 0)
					{
						//ゲームクリアーを呼び出す
						NewGO<GameClear>(0);
						DeleteGO(this);
						m_gameClear = true;
					}

					//倒されたので消す
					DeleteGO(this);

					//効果音を再生する
					SoundSource* se3 = NewGO<SoundSource>(0);
					se3->Init(11);
					se3->Play(false);
					se3->SetVolume(0.2f);
				}
				else
				{
					//被ダメージステートに移行する
					m_enemyState = enEnemyState_ReceiveDamage;
				}
				return;
			}
		}
}

//攻撃
void Enemy::Attack()
{
	//攻撃ステートではないなら攻撃しない
	if (m_enemyState != enEnemyState_Attack)
	{
		return;
	}

	//攻撃中だったら
	if (m_isUnderAttack == true)
	{
		MakeAttackCollision();
	}
}

//敵の追従処理
const bool Enemy::SerchPlayer() const
{
	//ベクトルの引き算をする
	Vector3 diff = m_player->GetPosition() - m_position;

	//プレイヤーにある程度近かったら
	if (diff.LengthSq() <= 10000.0f * 10000.0f)
	{
		//エネミーからプレイヤーに向かうベクトルを正規化する
		diff.Normalize();
		//エネミーの正面ベクトルと、エネミーからプレイヤーに向かう
		//ベクトルの内積(cosθ)から角度（θ）を求める
		float cos = m_forward.Dot(diff);
		//内積(cosθ)から角度（θ）を求める
		float angle = acosf(cos);
		//角度（θ）が120°より小さければ
		if (angle <= (Math::PI / 180.0f) * 120.0f)
		{
			//プレイヤーを見つけた
			return true;
		}
	}
	//プレイヤーを見つけれなかった
	return false;
}

void Enemy::MakeAttackCollision()
{
	//攻撃の当たり判定用のコリジョンオブジェクトを作成する
	auto collisionObject = NewGO<CollisionObject>(0);
	//剣のボーンのワールド行列を取得する
	Matrix matrix = m_modelRender.GetBone(m_swordBoneId)->GetWorldMatrix();

	//ボックス状のコリジョンを作成する
	collisionObject->CreateBox(m_position, Quaternion::Identity, Vector3(100.0f, 10.0f, 10.0f));
	collisionObject->SetWorldMatrix(matrix);
	collisionObject->SetName("enemy_attack");
}

void Enemy::ProcessCommonStateTransition()
{
	//各タイマーを初期化する
	m_idleTimer = 0.0f;
	m_chaseTimer = 0.0f;

	//エネミーからプレイヤーに向かうベクトルを計算する
	Vector3 diff = m_player->GetPosition() - m_position;

	//プレイヤーを見つけたら
	if (SerchPlayer() == true)
	{
		//ベクトルを正規化する
		diff.Normalize();
		//移動速度を設定する
		m_moveSpeed = diff * 70.0f;
		//攻撃できる距離なら
		if (IsCanAttack() == true)
		{
			//乱数によって、攻撃するか待機させるか決める
			int ram = rand() % 100;
			if (ram > 50)
			{
				//攻撃ステートにする
				m_enemyState = enEnemyState_Attack;
				m_isUnderAttack = false;
				return;
			}
			//待機ステート移行するか
			else
			{
				//待機ステートにする
				m_enemyState = enEnemyState_Idle;
				return;
			}
		}
		//攻撃できない距離だったら
		else
		{
			//乱数によって、追跡させるか決める
			int ram = rand() % 100;
			if (ram > 40)
			{
				//追跡ステートにする
				m_enemyState = enEnemyState_Chase;
				return;
			}
		}
	}
	//プレイヤーを見つけれなければ
	else
	{
		//待機ステートにする
		m_enemyState = enEnemyState_Idle;
		return;
	}
}

void Enemy::ProcessIdleStateTransition()
{
	m_idleTimer += g_gameTime->GetFrameDeltaTime();
	//待機時間がある程度経過した
	if (m_idleTimer >= 0.7f)
	{
		//他のステートへ移行する
		ProcessCommonStateTransition();
	}
}

void Enemy::ProcessAttackStateTransition()
{
	//ほかのステートに行く
	ProcessCommonStateTransition();
}

void Enemy::ProcessReceiveDamageStateTransition()
{
	//被ダメのアニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//攻撃されたら距離関係なしにとりあえず追従させる
		m_enemyState = enEnemyState_Chase;
		Vector3 diff = m_player->GetPosition() - m_position;
		diff.Normalize();
		//移動速度を設定する
		m_moveSpeed = diff * 70.0f;
	}
}

void Enemy::ProcessDownStateTransition()
{
	//ほかのステートに行く
	ProcessCommonStateTransition();
}

void Enemy::ProcessChaseStateTransition()
{
	if (IsCanAttack() == true)
	{
		//他のステータスへ移行させる
		ProcessCommonStateTransition();
		return;
	}
	m_chaseTimer += g_gameTime->GetFrameDeltaTime();
	//追跡時間がある程度経過したら
	if (m_chaseTimer >= 0.8f)
	{
		//他のステートへ移行させる
		ProcessCommonStateTransition();
	}
}

void Enemy::ManageState()
{
	switch (m_enemyState)
	{
		//待機ステート
	case enEnemyState_Idle:
		ProcessIdleStateTransition();
		break;
		//追跡ステート
	case enEnemyState_Chase:
		ProcessChaseStateTransition();
		break;
		//攻撃ステート
	case enEnemyState_Attack:
		ProcessAttackStateTransition();
		break;
		//被ダメのステート
	case  enEnemyState_ReceiveDamage:
		ProcessReceiveDamageStateTransition();
		break;
		//ダウンステート
	case enEnemyState_Down:
		ProcessDownStateTransition();
		break;
	}
}

void Enemy::PlayAnimation()
{
	m_modelRender.SetAnimationSpeed(1.0f);
	switch (m_enemyState)
	{
		//待機ステートの時。
	case enEnemyState_Idle:
		//待機アニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.5f);
		break;
		//追跡ステートの時。
	case enEnemyState_Chase:
		m_modelRender.SetAnimationSpeed(1.2f);
		//走りアニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Run, 0.1f);
		break;
		//攻撃ステートの時。
	case enEnemyState_Attack:
		m_modelRender.SetAnimationSpeed(1.6f);
		//攻撃アニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Attack, 0.1f);
		break;
		//被ダメージステートの時。
	case enEnemyState_ReceiveDamage:
		m_modelRender.SetAnimationSpeed(1.3f);
		//被ダメージアニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Damage, 0.1f);
		break;
		//ダウンステートの時。
	case enEnemyState_Down:
		//ダウンアニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Down, 0.1f);
		break;
	default:
		break;
	}
}


const bool Enemy::IsCanAttack() const
{
	Vector3 diff = m_player->GetPosition() - m_position;
	//エネミーと敵の距離が近かったら
	if (diff.LengthSq() <= 1.0f * 1.0f)
	{
		//攻撃する
		return true;
	}
	//攻撃できない
	return false;
}


void Enemy::Render(RenderContext&rc)
{
	m_modelRender.Draw(rc);
}

