#pragma once
#include "Systems/IExecute.h"

class Test : public IExecute
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
	Shader* shader;

	class CubeSky* sky;

	Material* floor;
	Material* stone;
	Material* brick;
	Material* wall;

	MeshSphere* sphere[10];
	MeshCylinder* cylinder[10];
	MeshCube* cube;
	MeshGrid* grid;

	D3DXCOLOR specular;
};