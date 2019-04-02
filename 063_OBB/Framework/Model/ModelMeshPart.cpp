#include "framework.h"
#include "ModelMeshPart.h"
#include "ModelMesh.h"

ModelMeshPart::ModelMeshPart()
	: material(NULL), perFrame(NULL)
	, pass(0), shader(NULL)
	//, transform(NULL)
{
	boneBuffer = new ConstantBuffer(&boneDesc, sizeof(BoneDesc));
}

ModelMeshPart::~ModelMeshPart()
{
	SAFE_DELETE(boneBuffer);
	//SAFE_DELETE(transform);
	SAFE_DELETE(perFrame);
}

void ModelMeshPart::SetShader(Shader * shader)
{
	this->shader = shader;


	/*SAFE_DELETE(transform);
	transform = new Transform(shader);*/

	SAFE_DELETE(perFrame);
	perFrame = new PerFrame(shader);

	material->SetShader(shader);
	sBoneBuffer = shader->AsConstantBuffer("CB_Bone");

	sTransformsSrv = shader->AsSRV("BoneTransforms");
}

//void ModelMeshPart::Transforms(D3DXMATRIX * transforms)
//{
//	memcpy(boneDesc.Transforms, transforms, sizeof(D3DXMATRIX) * MAX_MODEL_TRANFORMS);
//}

//void ModelMeshPart::SetTransform(Transform * transform)
//{
//	this->transform->Set(transform);
//}

void ModelMeshPart::TransformsSRV(ID3D11ShaderResourceView * srv)
{
	transformsSRV = srv;
}

void ModelMeshPart::Binding(Model * model)
{
	Material* srcMaterial = model->MaterialByName(materialName);

	material = new Material();
	material->Ambient(srcMaterial->Ambient());
	material->Diffuse(srcMaterial->Diffuse());
	material->Specular(srcMaterial->Specular());

	if (srcMaterial->DiffuseMap() != NULL)
		material->DiffuseMap(srcMaterial->DiffuseMap()->GetFile());

	if (srcMaterial->SpecularMap() != NULL)
		material->SpecularMap(srcMaterial->SpecularMap()->GetFile());

	if (srcMaterial->NormalMap() != NULL)
		material->NormalMap(srcMaterial->NormalMap()->GetFile());
}

void ModelMeshPart::Update()
{
	boneDesc.Index = parent->BoneIndex();

	//transform->Update();
	perFrame->Update();
}

void ModelMeshPart::Render(UINT count)
{
	boneBuffer->Apply();
	sBoneBuffer->SetConstantBuffer(boneBuffer->Buffer());

	sTransformsSrv->SetResource(transformsSRV);

	//transform->Render();
	perFrame->Render();
	material->Render();

	//shader->DrawIndexed(0, pass, indexCount, startIndex, 0);
	shader->DrawIndexedInstanced(0, pass, indexCount, count, startIndex);
}