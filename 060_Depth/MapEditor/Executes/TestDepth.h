#pragma once
#include "Systems/IExecute.h"

class TestDepth : public IExecute
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
	class CubeSky* sky;

	Shader* shader;
	
	MeshSphere* sphere[10];
	MeshCylinder* cylinder[10];
	MeshCube* cube;
	MeshGrid* grid;

	Texture* floor;
	Texture* stone;
	Texture* brick;
	Texture* wall;

	class Shadow* shadow;
};