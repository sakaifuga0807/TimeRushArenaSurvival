#pragma once
class CrossHair:public IGameObject
{
public:
	CrossHair();
	~CrossHair();
	void Update();
	void Render(RenderContext& rc);
	//メンバ変数
	CrossHair* clrocchair;
	SpriteRender spriteRender;
	Vector3 position;
};

