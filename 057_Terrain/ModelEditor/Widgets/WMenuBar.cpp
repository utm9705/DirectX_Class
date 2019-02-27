#include "stdafx.h"
#include "WMenuBar.h"

WMenuBar::WMenuBar()
	: bMetricWindow(false), bDemoWindow(false)
{
	
}

WMenuBar::~WMenuBar()
{

}

void WMenuBar::Begin()
{

}

void WMenuBar::End()
{

}

void WMenuBar::Render()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::MenuItem("Load");
			ImGui::MenuItem("Save");
			ImGui::Separator();
			ImGui::MenuItem("Exit");

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Window"))
		{
			ImGui::MenuItem("Metric", NULL, &bMetricWindow);
			ImGui::MenuItem("Demo", NULL, &bDemoWindow);

			ImGui::EndMenu();
		}
		
		ImGui::EndMainMenuBar();
	}

	if (bMetricWindow == true) ImGui::ShowMetricsWindow();
	if (bDemoWindow == true) ImGui::ShowDemoWindow();
}
