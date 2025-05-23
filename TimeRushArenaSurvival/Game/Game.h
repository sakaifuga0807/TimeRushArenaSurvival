#pragma once
#include "Level3DRender/LevelRender.h"

class BackGround;
class Game;

class Player;
class GameCamera;

class Bullet;
class CrossHair;
class Enemy;
class GameClear;

//Gameシーンを管理するクラス
class Game : public IGameObject
{
public:
	Game();
	~Game();

	//更新処理
	void Update();
	bool Start();
	void Render(RenderContext& rc);
	void CountTimer();
	void GameConditionsFlag();
	void EnemyManager();
	void NotifyGameClear();

	//エネミーが全滅していたらtrueを返す
	const bool IsWannihilationEnemy() const
	{
		return m_numEnemy == m_numDefeatedEnemy;
	}

	//倒された敵の数を+1する
	void AddDefeattedEnemyNunber()
	{
		m_numDefeatedEnemy;
	}



	//メンバ変数
	float m_stopTimer;
	Vector4 m_color_Red{ 1.0f,0.0f,0.0f,1.0f};//赤色
	Game* m_game;
	bool m_gameClearFlag = false;
	SoundSource* m_bgm = nullptr;
	int m_GameClearDrawTime;
	FontRender m_HPfontRender;//HPのフォントレンダー
	FontRender m_TimerfontRender;//タイマーのフォントレンダー
	Enemy* m_enemy;//敵
	BackGround* m_backGround;//背景
	Player* m_player;//プレイヤー
	GameCamera* m_gamecamera;//カメラ
	CrossHair* m_crossHair;//クロスヘア
	LevelRender m_levelRender;//レベルレンダー
	float m_timerlimit = 300.0f;//タイマー
	int m_numEnemy = 0;//敵の数
	int m_numDefeatedEnemy =0;//敵の死亡数


	SpriteRender m_spriteRender;
	SpriteRender m_spriteRender2;
	


	SpriteRender m_spriteRender3;
	SpriteRender m_spriteRender4;
	SpriteRender m_spriteRender5;
	SpriteRender m_spriteRender6;
	SpriteRender m_spriteRender7;

};