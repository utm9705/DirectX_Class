#include "stdafx.h"
#include "WProfile.h"
#include "Executes\TestVertex.h"

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
