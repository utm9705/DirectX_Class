#include "Framework.h"
#include "Sky.h"
#include "Renders/Render2D.h"

Sky::Sky()
{
	render2D = new Render2D();
	render2D->Scale(100, 100, 1);	
}

Sky::~Sky()
{
}

void Sky::Ready()
{
	render2D->Ready();

	texture = new Texture(L"Box.png");
}

void Sky::Update()
{
	render2D->Update();
}

void Sky::Render()
{
	
}

void Sky::PostRender()
{
	render2D->SRV(texture->SRV());
	render2D->Render();
}
