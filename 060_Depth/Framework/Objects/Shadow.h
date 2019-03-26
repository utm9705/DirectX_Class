#pragma once

class Shadow
{
public:
	Shadow(Shader* shader); //원래는 배열 또는 테크니큐
	~Shadow();

	void Ready();
	void Update();
	void PreRender();
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
	//조명공간에 관련한 애들(깊이)
	class RenderTarget* depthTarget;
	class Fixity* lightCamera;
	class Perspective* lightPerspective;

	class Render2D* depthRender;
};