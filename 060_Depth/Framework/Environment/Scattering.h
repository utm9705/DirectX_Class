#pragma once

class Scattering
{
public:
	Scattering(Shader* shader);
	~Scattering();

	RenderTarget* Rayleigh() { return rayleighTarget; }
	RenderTarget* Mie() { return mieTarget; }

	void Ready();
	void Update();
	
	void PreRender();
	void PostRender();

private:
	Shader* shader;

	class Render2D* render2D;
	RenderTarget* mieTarget, *rayleighTarget;

	ID3D11Buffer* vertexBuffer;
};