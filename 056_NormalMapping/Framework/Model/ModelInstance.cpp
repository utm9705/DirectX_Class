#include "Framework.h"
#include "ModelInstance.h"

ModelInstance::ModelInstance(Model * model, wstring shaderFile)
	: maxCount(0), model(model), height(MAX_INST_MODEL)
{
	for (Material* material : model->Materials())
		material->SetShader(shaderFile);
}

ModelInstance::~ModelInstance()
{
	for (UINT i = 0; i < height; i++)
		SAFE_DELETE_ARRAY(transforms[i]);
	SAFE_DELETE_ARRAY(transforms);

	SAFE_RELEASE(texture);
	SAFE_RELEASE(srv);
}

void ModelInstance::Ready()
{
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
	desc.Width = 128 * 4;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	HRESULT hr = D3D::GetDevice()->CreateTexture2D(&desc, NULL, &texture);
	assert(SUCCEEDED(hr));

	CsResource::CreateSrv(texture, &srv);


	MappedData();
}

UINT ModelInstance::AddWorld(D3DXMATRIX & world)
{
	memcpy(&worlds[maxCount], &world, sizeof(D3DXMATRIX));

	maxCount++;


	D3D11_MAPPED_SUBRESOURCE subResource;
	for (UINT i = 0; i < model->MeshCount(); i++)
	{
		ID3D11Buffer* instanceBuffer = model->MeshByIndex(i)->InstanceBuffer();

		D3D::GetDC()->Map(instanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
		{
			memcpy(subResource.pData, worlds, sizeof(D3DXMATRIX) * maxCount);
		}
		D3D::GetDC()->Unmap(instanceBuffer, 0);
	}

	for (Material* material : model->Materials())
		material->GetShader()->AsShaderResource("Transforms")->SetResource(srv);

	return maxCount - 1;
}

void ModelInstance::MappedData()
{
	transforms = new D3DXMATRIX*[height];
	for (UINT i = 0; i < height; i++)
		transforms[i] = new D3DXMATRIX[128];

	D3DXMATRIX boneTransforms[128];
	for (UINT i = 0; i < height; i++)
	{
		for (UINT b = 0; b < model->BoneCount(); b++)
		{
			ModelBone* bone = model->BoneByIndex(b);

			D3DXMATRIX transform;
			D3DXMATRIX parentTransform;

			int parentIndex = bone->ParentIndex();
			if (parentIndex < 0)
				D3DXMatrixIdentity(&parentTransform);
			else
				parentTransform = boneTransforms[parentIndex];


			D3DXMATRIX inv = bone->Transform();

			boneTransforms[b] = parentTransform;
			transforms[i][b] = inv * boneTransforms[b];
		}
	}


	D3D11_MAPPED_SUBRESOURCE subResource;
	D3D::GetDC()->Map(texture, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		for (UINT i = 0; i < height; i++)
		{
			void* p = ((D3DXMATRIX *)subResource.pData + (i * 128));

			memcpy(p, transforms[i], sizeof(D3DXMATRIX) * 128);
		}
	}
	D3D::GetDC()->Unmap(texture, 0);
}

void ModelInstance::Render()
{
	for (ModelMesh* mesh : model->Meshes())
		mesh->RenderInstance(maxCount);
}
