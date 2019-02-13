#pragma once
#include "Systems/Widget.h"

class WMenuBar : public Widget
{
public:
	WMenuBar();
	~WMenuBar();

	void Begin() override;
	void End() override;

	void Render() override;

	bool GetExport() { return bExport; }

private: 
	bool bMetricWindow;
	bool bDemoWindow;

	bool bExport;
};