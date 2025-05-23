#pragma once

class Game;

class GameClear :public IGameObject
{
public:
	GameClear();
	~GameClear();


	void Render(RenderContext& rc);
	void Update();
	void TimerUI();


	//ƒƒ“ƒo•Ï”
	FontRender m_TimerfontRender;
	int scoreTime;
	SoundSource* m_bgm = nullptr;
	Game *m_game;
	SpriteRender m_spriteRender_S;
	SpriteRender m_spriteRender_A;
	SpriteRender m_spriteRender_B;
	SpriteRender m_spriteRender_C;
};

