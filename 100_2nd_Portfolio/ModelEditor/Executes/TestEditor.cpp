#include "stdafx.h"
#include "TestEditor.h"

#include "Widgets/WProfile.h"
#include "Widgets/WMenuBar.h"

void TestEditor::Initialize()
{
	bWindow = true;
}

void TestEditor::Ready()
{
	wProfile = new WProfile();
	Gui::Get()->AddWidget(wProfile);

	wMenuBar = new WMenuBar();
	Gui::Get()->AddWidget(wMenuBar);
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
	if (wMenuBar->GetExport())
	{
		ImGui::Begin("Export");
		{
			ImGui::Checkbox("ExportMesh", &bExportMesh);
			ImGui::Checkbox("ExportMaterial", &bExportMaterial);
			ImGui::Checkbox("ExportAnimation", &bExportAnimation);

		}
		ImGui::End();
	}//if(Export)
}

void TestEditor::Render()
{
	
}