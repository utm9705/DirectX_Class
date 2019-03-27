#pragma once

class Context
{
public:
	static Context* Get();
	static void Create();
	static void Delete();

private:
	Context();
	~Context();

public:
	void Update();
	void Render();

	class Camera* GetCamera() { return camera; }
	class Perspective* GetPerspective() { return perspective; }
	class Viewport* GetViewport() { return viewport; }

	D3DXMATRIX& View();
	D3DXMATRIX& Projection();

	D3DXCOLOR& LightColor() { return lightColor; }
	D3DXVECTOR3& LightDirection() { return lightDirection; }
	D3DXVECTOR3& LightPosition() { return lightPosition; }

private:
	static Context* instance;

private:
	class Perspective* perspective;
	class Viewport* viewport;
	class Camera* camera;

	D3DXCOLOR lightColor;
	D3DXVECTOR3 lightDirection;
	D3DXVECTOR3 lightPosition;
};