#include "stdafx.h"
#include "WProfile.h"
#include "Executes\TestAnimator.h"

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
	ImGui::SliderFloat("Testfloat", &f, 0, 10);
	
}
