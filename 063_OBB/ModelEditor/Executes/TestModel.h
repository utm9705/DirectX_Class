#pragma once
#include "Systems/IExecute.h"

class TestModel : public IExecute
{
public:
	void Initialize() override;
	void Ready() override;
	void Destroy() override;

	void Update() override;

	void PreRender() override;

	void Render() override;
	void PostRender() override {}

	void ResizeScreen() override {}

private:
	void ExportTower();
	void CreateTower();

	void ExportTank();
	void CreateTank();

	void ExportHanjo();
	void CreateHanjo();

private:
	Model* towerModel;
	Shader* towerShader;
	ModelRender* tower;

	Model* tankModel;
	Shader* tankShader;
	ModelRender* tank;


	Model* hanjoModel;
	Shader* hanjoShader;
	ModelRender* hanjo;
	
};