#pragma once
#include "Systems/Widget.h"

class WProfile : public Widget
{
public:
	WProfile();
	~WProfile();

	void Render() override;

private:
	float f = 0;
};