#include "Framework.h"
#include "ModelRender.h"
#include "Model/ModelMesh.h"

ModelRender::ModelRender(Shader * shader, Model * model)
	: model(model), drawCount(0)
{
	for (ModelMesh* mesh : model->Meshes())
		mesh->SetShader(shader);

	instanceBuffer = new VertexBuffer(worlds, MAX_MODEL_INSTANCE, sizeof(D3DXMATRIX), 1, true);

	
	//Create Texture
	{
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
		desc.Width = MAX_MODEL_TRANFORMS * 4;
		desc.Height = MAX_MODEL_INSTANCE;
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
	}

	//Create SRV
	{
		D3D11_TEXTURE2D_DESC desc;
		texture->GetDesc(&desc);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		srvDesc.Format = desc.Format;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

		HRESULT hr = D3D::GetDevice()->CreateShaderResourceView(texture, &srvDesc, &srv);
		assert(SUCCEEDED(hr));
	}

	for (UINT i = 0; i < MAX_MODEL_INSTANCE; i++)
		transforms[i] = new Transform();


	for (ModelMesh* mesh : model->Meshes())
		mesh->TransformsSRV(srv);


	UpdateBoneTransforms();
}

ModelRender::~ModelRender()
{
	for (UINT i = 0; i < MAX_MODEL_INSTANCE; i++)
		SAFE_DELETE(transforms[i]);
		

	SAFE_RELEASE(texture);
	SAFE_RELEASE(srv);
}

void ModelRender::Update()
{
	for (ModelMesh* mesh : model->Meshes())
		mesh->Update();
}

void ModelRender::UpdateTransforms()
{
	for (UINT i = 0; i < drawCount; i++)
		memcpy(worlds[i], transforms[i]->World(), sizeof(D3DXMATRIX));


	D3D11_MAPPED_SUBRESOURCE subResource;
	D3D::GetDC()->Map(instanceBuffer->Buffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy(subResource.pData, worlds, sizeof(D3DXMATRIX) * drawCount);
	}
	D3D::GetDC()->Unmap(instanceBuffer->Buffer(), 0);
}

void ModelRender::Render()
{
	instanceBuffer->Render();
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	for (ModelMesh* mesh : model->Meshes())
	{
		//Deprecated
		//mesh->SetTransform(transform);

		mesh->Render(drawCount);
	}
}

UINT ModelRender::AddTransform()
{
	drawCount++;

	return drawCount - 1;
}

Transform * ModelRender::GetTransform(UINT index)
{
	return transforms[index];
}

void ModelRender::UpdateBoneTransforms()
{
	D3DXMATRIX bones[MAX_MODEL_TRANFORMS];
	for (UINT i = 0; i < MAX_MODEL_INSTANCE; i++)
	{
		for (UINT b = 0; b < model->BoneCount(); b++)
		{
			ModelBone* bone = model->BoneByIndex(b);

			D3DXMATRIX parentTransform;
			int parentIndex = bone->ParentIndex();
			if (parentIndex < 0)
				D3DXMatrixIdentity(&parentTransform);
			else
				parentTransform = bones[parentIndex];


			D3DXMATRIX matrix = bone->Transform();

			bones[b] = parentTransform;
			boneTransforms[i][b] = matrix * bones[b];
		}
	}


	D3D11_MAPPED_SUBRESOURCE subResource;
	D3D::GetDC()->Map(texture, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		for (UINT i = 0; i < MAX_MODEL_INSTANCE; i++)
		{
			void* p = ((D3DXMATRIX *)subResource.pData + (i * MAX_MODEL_TRANFORMS));

			memcpy(p, boneTransforms[i], sizeof(D3DXMATRIX) * MAX_MODEL_TRANFORMS);
		}
	}
	D3D::GetDC()->Unmap(texture, 0);


	for (ModelMesh* mesh : model->Meshes())
		mesh->TransformsSRV(srv);
}