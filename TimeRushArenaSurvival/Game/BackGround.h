#pragma once
class BackGround:public IGameObject
{
public:
	BackGround();
	~BackGround();

	void Render(RenderContext& rc);

	//�����o�ϐ�
	SoundSource* m_bgm = nullptr;
	ModelRender modelRender;
	PhysicsStaticObject physicsStaticObject;
};

