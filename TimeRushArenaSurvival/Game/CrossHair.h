#pragma once
class CrossHair:public IGameObject
{
public:
	CrossHair();
	~CrossHair();
	void Update();
	void Render(RenderContext& rc);
	//ƒƒ“ƒo•Ï”
	CrossHair* clrocchair;
	SpriteRender spriteRender;
	Vector3 position;
};

