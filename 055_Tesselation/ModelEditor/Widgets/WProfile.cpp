#include "stdafx.h"
#include "WProfile.h"
#include "Executes\TestTriangle.h"

WProfile::WProfile()
{
	title = "Profile";

}

WProfile::~WProfile()
{

}

void WProfile::Render()
{
	ImGui::LabelText("FPS", "%d", (int)ImGui::GetIO().Framerate);
}
