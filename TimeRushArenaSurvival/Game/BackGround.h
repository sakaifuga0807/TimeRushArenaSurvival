#pragma once
class BackGround:public IGameObject
{
public:
	BackGround();
	~BackGround();

	void Render(RenderContext& rc);

	//ƒƒ“ƒo•Ï”
	SoundSource* m_bgm = nullptr;
	ModelRender modelRender;
	PhysicsStaticObject physicsStaticObject;
};

