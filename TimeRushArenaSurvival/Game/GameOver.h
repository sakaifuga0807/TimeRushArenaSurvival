#pragma once


class GameOver:public IGameObject
{
public:
	GameOver();
	~GameOver();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	SpriteRender m_spriteRender;
	SoundSource* m_bgm=nullptr;
};

