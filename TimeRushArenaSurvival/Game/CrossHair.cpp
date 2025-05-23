#include "stdafx.h"
#include "CrossHair.h"

CrossHair::CrossHair()
{
	spriteRender.Init("Assets/sprite/vectorColor.DDS",9.0f,9.0f);
}

CrossHair::~CrossHair()
{
	DeleteGO(this);
}

void CrossHair::Update()
{
	
}

void CrossHair::Render(RenderContext& rc)
{
	spriteRender.Draw(rc);
}