#include "stdafx.h"
#include "Game.h"
#include "BackGround.h"
#include "GameOver.h"
#include "Player.h"
#include "GameCamera.h"
#include "Enemy.h"
#include "CrossHair.h"
#include "GameClear.h"

#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

//コンストラクタ
Game::Game()
{
	//m_stopTimer = 0.0f;
	
}

//デストラクタ
Game::~Game()
{
	DeleteGO(m_bgm);
	DeleteGO(m_player);
	DeleteGO(m_gamecamera);
	DeleteGO(m_crossHair);
	DeleteGO(m_backGround);
	const auto& enemys = FindGOs<Enemy>("enemy");
	for (auto enemy : enemys)
	{
		DeleteGO(enemy);
	}
}

//更新処理
void Game::Update()
{
	//ゲームクリアーしたときに消去する
	if(m_enemy->m_gameClear==true)

	//レベルをマップチップをアップデート
	m_levelRender.Update();

	//タイマーをアップデート8
	CountTimer();

	//HP
	EnemyManager();

	//タイマーをアップデート
	GameConditionsFlag();

	//////////HPバーを減らす処理///////////////////////////////
	if (m_player->m_playerHP <= 6)
	{
		Vector4 m_color_Yello(0.0f, 1.0f, 1.0f, 1.0f);
		m_spriteRender.SetMulColor(m_color_Yello);
	}
	if (m_player->m_playerHP <= 3)
	{
		Vector4 m_color_Red(1.0f, 0.0f, 0.0f, 1.0f);
		m_spriteRender.SetMulColor(m_color_Red);
	}

	int MaxHP = m_player->m_MaxHP;
	int NowHP = m_player->m_playerHP;
	float wari = (float)NowHP / (float)MaxHP;
	Vector3 scal = { 0.45f,1.0f,0.5f };
	scal.x *= wari;
	m_spriteRender2.SetScale(scal);
	m_spriteRender.Update();
	m_spriteRender2.Update();
	//////////////////////////////////////////////////////////


}

bool Game::Start()
{
	m_backGround = NewGO<BackGround>(0);
	//プレイヤーのオブジェクト作成
	m_player = NewGO<Player>(0, "player");
	//カメラオブジェクト作成
	m_gamecamera = NewGO<GameCamera>(0, "gamecamera");
	//弾のオブジェクト作成
	//m_bullet = NewGO<Bullet>(0, "bullet");
	//クロスヘアのオブジェクトを作成
	m_crossHair = NewGO<CrossHair>(0, "crossHair");
	//エネミー(スケルトン)のオブジェクトを作成
	Enemy* enemy = NewGO<Enemy>(0, "skeleton");

	m_enemy = FindGO<Enemy>("skeleton");


	//HPバー処理
	m_spriteRender2.Init("Assets/sprite/midoo.DDS", 990, 65);
	m_spriteRender.Init("Assets/sprite/HPBAR.DDS", 1024, 128);

	m_spriteRender.SetScale(Vector3(0.5f, 0.5f, 0.5f));
	m_spriteRender.SetPosition(Vector3(-20.0f, -435.0f, 0.0f));
	m_spriteRender2.SetPosition(Vector3(-210.0f, -435.0f, 0.0f));
	m_spriteRender2.SetScale(Vector3(0.35f, 1.0f, 0.5f));
	m_spriteRender2.SetPivot(Vector2(0.0f, 0.5f));


	//レベルを構築する
	m_levelRender.Init("Assets/level/level.tkl", [&](LevelObjectData& objData) {
		//名前がバックグラウンドだったら
		/*if (objData.EqualObjectName(L"a-ground") == true)
		{

			//バックグラウンドのオブジェクトを作成
			m_backGround = NewGO<BackGround>(0, "backGround");
			return true;
		}*/

		//名前がskeltonだったら
		if (objData.EqualObjectName(L"Skeleton_Skin1") == true)
		{
			//エネミー(スケルトン)のオブジェクトを作成
			Enemy* enemy = NewGO<Enemy>(0,"skeleton");
			//座標を設定する
			enemy->SetPosition(objData.position);
			//大きさを設定する
			enemy->SetScale(objData.scale);
			//HPをセットする
			enemy->setHP(6);
			//敵の数を数えたいので+1をする
			m_numEnemy++;
			//m_enemyInstList.push_back(m_numEnemy);
			return true;
		}
		return true;
	});
	return true;
}

void Game::NotifyGameClear()
{
	m_gameClearFlag = true;
}

//ゲームーオーバー条件
void Game::GameConditionsFlag()
{
	////ゲームオーバー条件////
	//タイマーが0になったらゲームオーバーを表示させる
	if (m_timerlimit<= 0)
	{
		NewGO<GameOver>(0);
		DeleteGO(this);
	}

	//プレイヤーのHPが0になったゲームオーバー
	if (m_player->m_playerHP == 0)
	{
		NewGO<GameOver>(0);
		DeleteGO(this);
	}

	
}

//タイマーの表示
void Game::CountTimer()
{
	wchar_t wcsbuf[256];
	swprintf_s(wcsbuf, 256, L"残り時間%d", int(m_timerlimit));
	
	//表示するテキストを設定。
	m_TimerfontRender.SetText(wcsbuf);
	//フォントの位置を設定。
	m_TimerfontRender.SetPosition(Vector3(600.0f, 490.0f, 0.0f));
	//フォントの大きさを設定。
	m_TimerfontRender.SetScale(1.2f);
	//フォントの色を設定。
	m_TimerfontRender.SetColor(g_vec4Black);

	bool isTimerRunnin = true;
	//残り時間を計算
	if (isTimerRunnin)
	{
		//タイマーを減少させる。
		m_timerlimit -= g_gameTime->GetFrameDeltaTime();
		if (m_enemy->m_DefeatedEnemy<=0)
		{
			isTimerRunnin = false;
			m_stopTimer = m_timerlimit;
		}
	}



	//const float m_stopTimer = m_timerlimit;
	
	//タイマーが30秒以下だったら赤色にする
	if (m_timerlimit <= 30)
	{
		m_TimerfontRender.SetColor(m_color_Red);
	}
}

//残りの敵の数
void Game::EnemyManager()
{
	wchar_t tex1[256];
	swprintf_s(tex1, 256, L"残りの敵:%d", int(m_numEnemy));

	//表示するテキストを設定。
	m_HPfontRender.SetText(tex1);
	//フォントの位置を設定。
	m_HPfontRender.SetPosition(Vector3(-150.0f, 490.0f, 0.0f));
	//フォントの大きさを設定。
	m_HPfontRender.SetScale(1.2f);
	//フォントの色を設定。
	m_HPfontRender.SetColor(g_vec4Black);
}

void Game::Render(RenderContext& rc)
{
	//タイマー表示
	m_TimerfontRender.Draw(rc);
	//HP表示
	m_HPfontRender.Draw(rc);

	m_spriteRender.Draw(rc);
	m_spriteRender2.Draw(rc);
}