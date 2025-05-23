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

//�R���X�g���N�^
Game::Game()
{
	//m_stopTimer = 0.0f;
	
}

//�f�X�g���N�^
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

//�X�V����
void Game::Update()
{
	//�Q�[���N���A�[�����Ƃ��ɏ�������
	if(m_enemy->m_gameClear==true)

	//���x�����}�b�v�`�b�v���A�b�v�f�[�g
	m_levelRender.Update();

	//�^�C�}�[���A�b�v�f�[�g8
	CountTimer();

	//HP
	EnemyManager();

	//�^�C�}�[���A�b�v�f�[�g
	GameConditionsFlag();

	//////////HP�o�[�����炷����///////////////////////////////
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
	//�v���C���[�̃I�u�W�F�N�g�쐬
	m_player = NewGO<Player>(0, "player");
	//�J�����I�u�W�F�N�g�쐬
	m_gamecamera = NewGO<GameCamera>(0, "gamecamera");
	//�e�̃I�u�W�F�N�g�쐬
	//m_bullet = NewGO<Bullet>(0, "bullet");
	//�N���X�w�A�̃I�u�W�F�N�g���쐬
	m_crossHair = NewGO<CrossHair>(0, "crossHair");
	//�G�l�~�[(�X�P���g��)�̃I�u�W�F�N�g���쐬
	Enemy* enemy = NewGO<Enemy>(0, "skeleton");

	m_enemy = FindGO<Enemy>("skeleton");


	//HP�o�[����
	m_spriteRender2.Init("Assets/sprite/midoo.DDS", 990, 65);
	m_spriteRender.Init("Assets/sprite/HPBAR.DDS", 1024, 128);

	m_spriteRender.SetScale(Vector3(0.5f, 0.5f, 0.5f));
	m_spriteRender.SetPosition(Vector3(-20.0f, -435.0f, 0.0f));
	m_spriteRender2.SetPosition(Vector3(-210.0f, -435.0f, 0.0f));
	m_spriteRender2.SetScale(Vector3(0.35f, 1.0f, 0.5f));
	m_spriteRender2.SetPivot(Vector2(0.0f, 0.5f));


	//���x�����\�z����
	m_levelRender.Init("Assets/level/level.tkl", [&](LevelObjectData& objData) {
		//���O���o�b�N�O���E���h��������
		/*if (objData.EqualObjectName(L"a-ground") == true)
		{

			//�o�b�N�O���E���h�̃I�u�W�F�N�g���쐬
			m_backGround = NewGO<BackGround>(0, "backGround");
			return true;
		}*/

		//���O��skelton��������
		if (objData.EqualObjectName(L"Skeleton_Skin1") == true)
		{
			//�G�l�~�[(�X�P���g��)�̃I�u�W�F�N�g���쐬
			Enemy* enemy = NewGO<Enemy>(0,"skeleton");
			//���W��ݒ肷��
			enemy->SetPosition(objData.position);
			//�傫����ݒ肷��
			enemy->SetScale(objData.scale);
			//HP���Z�b�g����
			enemy->setHP(6);
			//�G�̐��𐔂������̂�+1������
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

//�Q�[���[�I�[�o�[����
void Game::GameConditionsFlag()
{
	////�Q�[���I�[�o�[����////
	//�^�C�}�[��0�ɂȂ�����Q�[���I�[�o�[��\��������
	if (m_timerlimit<= 0)
	{
		NewGO<GameOver>(0);
		DeleteGO(this);
	}

	//�v���C���[��HP��0�ɂȂ����Q�[���I�[�o�[
	if (m_player->m_playerHP == 0)
	{
		NewGO<GameOver>(0);
		DeleteGO(this);
	}

	
}

//�^�C�}�[�̕\��
void Game::CountTimer()
{
	wchar_t wcsbuf[256];
	swprintf_s(wcsbuf, 256, L"�c�莞��%d", int(m_timerlimit));
	
	//�\������e�L�X�g��ݒ�B
	m_TimerfontRender.SetText(wcsbuf);
	//�t�H���g�̈ʒu��ݒ�B
	m_TimerfontRender.SetPosition(Vector3(600.0f, 490.0f, 0.0f));
	//�t�H���g�̑傫����ݒ�B
	m_TimerfontRender.SetScale(1.2f);
	//�t�H���g�̐F��ݒ�B
	m_TimerfontRender.SetColor(g_vec4Black);

	bool isTimerRunnin = true;
	//�c�莞�Ԃ��v�Z
	if (isTimerRunnin)
	{
		//�^�C�}�[������������B
		m_timerlimit -= g_gameTime->GetFrameDeltaTime();
		if (m_enemy->m_DefeatedEnemy<=0)
		{
			isTimerRunnin = false;
			m_stopTimer = m_timerlimit;
		}
	}



	//const float m_stopTimer = m_timerlimit;
	
	//�^�C�}�[��30�b�ȉ���������ԐF�ɂ���
	if (m_timerlimit <= 30)
	{
		m_TimerfontRender.SetColor(m_color_Red);
	}
}

//�c��̓G�̐�
void Game::EnemyManager()
{
	wchar_t tex1[256];
	swprintf_s(tex1, 256, L"�c��̓G:%d", int(m_numEnemy));

	//�\������e�L�X�g��ݒ�B
	m_HPfontRender.SetText(tex1);
	//�t�H���g�̈ʒu��ݒ�B
	m_HPfontRender.SetPosition(Vector3(-150.0f, 490.0f, 0.0f));
	//�t�H���g�̑傫����ݒ�B
	m_HPfontRender.SetScale(1.2f);
	//�t�H���g�̐F��ݒ�B
	m_HPfontRender.SetColor(g_vec4Black);
}

void Game::Render(RenderContext& rc)
{
	//�^�C�}�[�\��
	m_TimerfontRender.Draw(rc);
	//HP�\��
	m_HPfontRender.Draw(rc);

	m_spriteRender.Draw(rc);
	m_spriteRender2.Draw(rc);
}