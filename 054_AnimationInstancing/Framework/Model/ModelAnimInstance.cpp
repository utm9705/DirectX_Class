#include "Framework.h"
#include "ModelAnimInstance.h"
#include "Model/ModelClip.h"

ModelAnimInstance::ModelAnimInstance(Model * model, ModelClip* clip, wstring shaderFile)
	: maxCount(0), model(model), clip(clip)
{
	for (Material* material : model->Materials())
		material->SetShader(shaderFile);

	ZeroMemory(frames, sizeof(FrameDesc) * MAX_INST_MODEL);
}

ModelAnimInstance::ModelAnimInstance()
{
	for (UINT i = 0; i < clip->FrameCount(); i++)
		SAFE_DELETE_ARRAY(transforms[i]);
	SAFE_DELETE_ARRAY(transforms);

	SAFE_RELEASE(texture);
	SAFE_RELEASE(srv);
}

void ModelAnimInstance::Ready()
{
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
	desc.Width = 128 * 4;
	desc.Height = clip->FrameCount();
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


	//Map
	{
		transforms = new D3DXMATRIX*[clip->FrameCount()];
		for (UINT i = 0; i < clip->FrameCount(); i++)
			transforms[i] = new D3DXMATRIX[128];

		D3DXMATRIX boneTransforms[128];
		for (UINT c = 0; c < clip->FrameCount(); c++)
		{
			for (UINT b = 0; b < model->BoneCount(); b++)
			{
				ModelBone* bone = model->BoneByIndex(b);

				D3DXMATRIX parentTransform;
				D3DXMATRIX invGlobal = bone->Transform();
				D3DXMatrixInverse(&invGlobal, NULL, &invGlobal);

				int parentIndex = bone->ParentIndex();
				if (parentIndex < 0)
					D3DXMatrixIdentity(&parentTransform);
				else
					parentTransform = boneTransforms[parentIndex];

				D3DXMATRIX animation;
				ModelKeyframe* frame = clip->Keyframe(bone->Name());

				if (frame != NULL)
				{
					ModelKeyframeData data = frame->Transforms[c];

					D3DXMATRIX S, R, T;
					D3DXMatrixScaling(&S, data.Scale.x, data.Scale.y, data.Scale.z);
					D3DXMatrixRotationQuaternion(&R, &data.Rotation);
					D3DXMatrixTranslation(&T, data.Translation.x, data.Translation.y, data.Translation.z);

					animation = S * R * T;
				}
				else
					D3DXMatrixIdentity(&animation);

				boneTransforms[b] = animation * parentTransform;
				transforms[c][b] = invGlobal * boneTransforms[b];
			}
		}


		D3D11_MAPPED_SUBRESOURCE subResource;
		D3D::GetDC()->Map(texture, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
		{
			for (UINT i = 0; i < clip->FrameCount(); i++)
			{
				void* p = ((D3DXMATRIX *)subResource.pData) + (i * 128);

				memcpy(p, transforms[i], sizeof(D3DXMATRIX) * 128);
			}
		}
		D3D::GetDC()->Unmap(texture, 0);
	}
}

UINT ModelAnimInstance::AddWorld(D3DMATRIX & world)
{
	memcpy(&worlds[maxCount], &world, sizeof(D3DXMATRIX));

	maxCount++;


	D3D11_MAPPED_SUBRESOURCE subResource;
	for (UINT i = 0; i < model->MeshCount(); i++)
	{
		ID3D11Buffer* instance = model->MeshByIndex(i)->InstanceBuffer();

		D3D::GetDC()->Map(instance, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
		{
			memcpy(subResource.pData, worlds, sizeof(D3DXMATRIX) * maxCount);
		}
		D3D::GetDC()->Unmap(instance, 0);
	}

	for (Material* material : model->Materials())
		material->GetShader()->AsShaderResource("Transforms")->SetResource(srv);


	return maxCount - 1;
}

void ModelAnimInstance::Render()
{
	for (UINT i = 0; i < maxCount; i++)
	{
		frames[i].FrameTime += Time::Delta();

		float invFrameRate = 1.0f / clip->FrameRate();
		if (frames[i].FrameTime > invFrameRate)
		{
			frames[i].FrameTime = 0.0f;

			frames[i].Curr = (frames[i].Curr + 1) % clip->FrameCount();
			frames[i].Next = (frames[i].Curr + 1) % clip->FrameCount();
		}

		frames[i].Time = frames[i].FrameTime / invFrameRate;
	}//for(i)

	for (Material* material : model->Materials())
		material->GetShader()->Variable("Frames")->SetRawValue(frames, 0, sizeof(FrameDesc) * MAX_INST_MODEL);

	for (ModelMesh* mesh : model->Meshes())
		mesh->RenderInstance(maxCount);
}
