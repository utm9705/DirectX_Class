#include "Framework.h"
#include "ModelAnimInstance.h"
#include "Model/ModelClip.h"

ModelAnimInstance::ModelAnimInstance(Model * model, ModelClip* clip, wstring shaderFile)
	: ModelInstance(model, shaderFile)
	, clip(clip)
{
	height = clip->FrameCount();

	ZeroMemory(frames, sizeof(FrameDesc) * MAX_INST_MODEL);
}

ModelAnimInstance::~ModelAnimInstance()
{
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

	__super::Render();
}

void ModelAnimInstance::MappedData()
{
	//Map
	{
		transforms = new D3DXMATRIX*[height];
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
