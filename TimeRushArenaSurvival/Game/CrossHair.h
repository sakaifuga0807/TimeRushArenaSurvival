#pragma once
class CrossHair:public IGameObject
{
public:
	CrossHair();
	~CrossHair();
	void Update();
	void Render(RenderContext& rc);
	//�����o�ϐ�
	CrossHair* clrocchair;
	SpriteRender spriteRender;
	Vector3 position;
};

