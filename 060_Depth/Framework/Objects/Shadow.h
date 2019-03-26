#pragma once

class Shadow
{
public:
	Shadow(Shader* shader);
	~Shadow();

	void Ready();
	void PreRender();
	void Update();
	void PostRender();

private:
	struct MatrixDesc
	{
		D3DXMATRIX View;
		D3DXMATRIX Projection;
	} matrixDesc;

private:
	Shader* shader;

	CBuffer* matrixBuffer;
	ID3DX11EffectConstantBuffer* sMatrixBuffer;

	class RenderTarget* depthTarget;
	class Fixity* lightCamera;
	class Perspective* lightPerspective;

	class Render2D* depthRender;
};