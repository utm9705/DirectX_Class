#pragma once

class Scattering
{
public:
	Scattering(Shader* shader);
	~Scattering();

	RenderTargetView* Rayleigh() { return rayleighTarget; }
	RenderTargetView* Mie() { return mieTarget; }

	void Ready();
	void Update();
	
	void PreRender();
	void PostRender();

private:
	Shader* shader;

	class Render2D* render2D;
	RenderTargetView* mieTarget, *rayleighTarget;

	ID3D11Buffer* vertexBuffer;
};