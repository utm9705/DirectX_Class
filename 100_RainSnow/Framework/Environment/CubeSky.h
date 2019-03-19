#pragma once

class CubeSky
{
public:
	CubeSky(Shader* shader, wstring cubeFile);
	~CubeSky();

	void Update();
	void Render();

private:
	MeshSphere* sphere;

	ID3D11ShaderResourceView* srv;
};