#include "stdafx.h"
#include "TestEditor.h"
#include "Widgets/WProfile.h"
#include "Widgets/WMenuBar.h"

void TestEditor::Initialize()
{
	bWindow = true;
	color = D3DXCOLOR(1, 1, 1, 1);
}

void TestEditor::Ready()
{
	wProfile = new WProfile();
	Gui::Get()->AddWidget(wProfile);

	wMenuBar = new WMenuBar();
	Gui::Get()->AddWidget(wMenuBar);

	Gui::Get()->AddText(100, 100, 1.0f, 0, 0, "Test Text");
}

void TestEditor::Destroy()
{
	SAFE_DELETE(wProfile);
	SAFE_DELETE(wMenuBar);
}

void TestEditor::Update()
{
	
}

void TestEditor::PreRender()
{

}

void TestEditor::Render()
{
	
}