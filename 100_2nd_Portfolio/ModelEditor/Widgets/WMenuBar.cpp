#include "stdafx.h"
#include "WMenuBar.h"

WMenuBar::WMenuBar()
	: bMetricWindow(false), bDemoWindow(false)
{

	bExport = false;
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
			ImGui::MenuItem("Export", NULL, &bExport);

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
