#include "Framework.h"
#include "Projector.h"
#include "Fixity.h"

Projector::Projector(Shader * shader, wstring textureFile)
	: shader(shader)
{
	fixity = new Fixity();
	fixity->RotationDegree(90, 0);
	fixity->Position(0, 10, 0);

	
	perspective = new Perspective(1, 1, Math::PI * 0.5f, 0, 1);

	texture = new Texture(textureFile);

	sMap = shader->AsShaderResource("ProjectionMap");
	sView = shader->AsMatrix("View2");
	sProjection = shader->AsMatrix("Projection2");
}

Projector::~Projector()
{
	SAFE_DELETE(texture);
	SAFE_DELETE(shader);

	SAFE_DELETE(fixity);
	SAFE_DELETE(perspective);
}

void Projector::Update()
{
	D3DXVECTOR3 position;
	fixity->Position(&position);
	ImGui::SliderFloat3("Position", (float *)&position, -100, 100);
	fixity->Position(position);

	D3DXVECTOR2 rotation;
	fixity->RotationDegree(&rotation);
	ImGui::SliderFloat2("Rotation", (float *)&rotation, -180, 180);
	fixity->RotationDegree(rotation);

	static D3DXVECTOR2 size(10, 10);
	ImGui::SliderFloat2("Size", (float *)&size, 0.1f, 10);

	D3DXMatrixOrthoLH(&ortho, size.x, size.y, 0, 1);
	D3DXMATRIX P = ortho;

	D3DXMATRIX V = fixity->Matrix();
	//D3DXMATRIX P = perspective->Matrix();

	sView->SetMatrix(V);
	sProjection->SetMatrix(P);
	sMap->SetResource(texture->SRV());
}