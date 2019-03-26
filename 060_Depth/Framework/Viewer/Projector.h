#pragma once

class Projector
{
public:
	Projector(Shader* shader, wstring textureFile);
	~Projector();

	void Update();

private:
	Shader* shader;
	vector<Mesh* > meshes;

	class Fixity* fixity;
	class Perspective* perspective;

	D3DXMATRIX ortho;

	Texture* texture;
	ID3DX11EffectShaderResourceVariable* sMap;

	ID3DX11EffectMatrixVariable* sView;
	ID3DX11EffectMatrixVariable* sProjection;
};