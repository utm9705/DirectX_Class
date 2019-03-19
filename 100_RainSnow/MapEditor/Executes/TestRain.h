#pragma once
#include "Systems/IExecute.h"

class TestRain : public IExecute
{
public:
	void Initialize() override;
	void Ready() override;
	void Destroy() override;

	void Update() override;

	void PreRender() override;

	void Render() override;
	void PostRender() override;

	void ResizeScreen() override {}

private:

	class Terrain* terrain;

	class Rain* rain;
	class Snow* snow;



	vector<class RainInst*> vecRain;

	class RainInst* rainInst;
	class SnowInst* snowInst;
};