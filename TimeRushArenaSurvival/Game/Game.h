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

//Game�V�[�����Ǘ�����N���X
class Game : public IGameObject
{
public:
	Game();
	~Game();

	//�X�V����
	void Update();
	bool Start();
	void Render(RenderContext& rc);
	void CountTimer();
	void GameConditionsFlag();
	void EnemyManager();
	void NotifyGameClear();

	//�G�l�~�[���S�ł��Ă�����true��Ԃ�
	const bool IsWannihilationEnemy() const
	{
		return m_numEnemy == m_numDefeatedEnemy;
	}

	//�|���ꂽ�G�̐���+1����
	void AddDefeattedEnemyNunber()
	{
		m_numDefeatedEnemy;
	}



	//�����o�ϐ�
	float m_stopTimer;
	Vector4 m_color_Red{ 1.0f,0.0f,0.0f,1.0f};//�ԐF
	Game* m_game;
	bool m_gameClearFlag = false;
	SoundSource* m_bgm = nullptr;
	int m_GameClearDrawTime;
	FontRender m_HPfontRender;//HP�̃t�H���g�����_�[
	FontRender m_TimerfontRender;//�^�C�}�[�̃t�H���g�����_�[
	Enemy* m_enemy;//�G
	BackGround* m_backGround;//�w�i
	Player* m_player;//�v���C���[
	GameCamera* m_gamecamera;//�J����
	CrossHair* m_crossHair;//�N���X�w�A
	LevelRender m_levelRender;//���x�������_�[
	float m_timerlimit = 300.0f;//�^�C�}�[
	int m_numEnemy = 0;//�G�̐�
	int m_numDefeatedEnemy =0;//�G�̎��S��


	SpriteRender m_spriteRender;
	SpriteRender m_spriteRender2;
	


	SpriteRender m_spriteRender3;
	SpriteRender m_spriteRender4;
	SpriteRender m_spriteRender5;
	SpriteRender m_spriteRender6;
	SpriteRender m_spriteRender7;

};