#pragma once
class BackGround:public IGameObject
{
public:
	BackGround();
	~BackGround();

	void Render(RenderContext& rc);

	//メンバ変数
	SoundSource* m_bgm = nullptr;
	ModelRender modelRender;
	PhysicsStaticObject physicsStaticObject;
};

