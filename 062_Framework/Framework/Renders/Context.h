#pragma once

#define MAX_POINT_LIGHT 32
struct PointLight
{
	Color Color;
	Vector3 Position;
	float Range;
	float Intensity;
	float Padding[3];
};

#define MAX_SPOT_LIGHT 32
struct SpotLight
{
	Color Color;
	Vector3 Position;
	float Inner;
	float Outer;
	Vector3 Direction;
};

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

	D3DXMATRIX View();
	D3DXMATRIX Projection();

	D3DXCOLOR& LightAmbient() { return lightAmbient; }
	D3DXCOLOR& LightSpecular() { return lightSpecular; }
	D3DXVECTOR3& LightDirection() { return lightDirection; }
	D3DXVECTOR3& LightPosition() { return lightPosition; }

	UINT PointLights(OUT PointLight* lights);
	UINT AddPointLight(PointLight light);
	PointLight& GetPointLight(UINT index) { return pointLights[index]; }
	UINT GetPointLightCount() { return pointLightCount; }

	UINT SpotLights(OUT SpotLight* lights);
	UINT AddSpotLight(SpotLight light);
	SpotLight& GetSpotLight(UINT index) { return spotLights[index]; }
	UINT GetSpotLightCount() { return spotLightCount; }

private:
	static Context* instance;

private:
	class Perspective* perspective;
	class Viewport* viewport;
	class Camera* camera;

	D3DXCOLOR lightAmbient;
	D3DXCOLOR lightSpecular;
	D3DXVECTOR3 lightDirection;
	D3DXVECTOR3 lightPosition;

	UINT pointLightCount;
	PointLight pointLights[MAX_POINT_LIGHT];

	UINT spotLightCount;
	SpotLight spotLights[MAX_SPOT_LIGHT];
};