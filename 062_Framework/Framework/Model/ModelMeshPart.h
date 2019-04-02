#pragma once

class ModelMeshPart
{
public:
	friend class Model;
	friend class ModelMesh;

public:
	void SetShader(Shader* shader);
	void Pass(UINT val) { pass = val; }

private:
	ModelMeshPart();
	~ModelMeshPart();

private:
	//Deprecated
	//void Transforms(D3DXMATRIX* transforms);

	//Deprecated
	//void SetTransform(Transform* transform);

	void TransformsSRV(ID3D11ShaderResourceView* srv);

	void Binding(Model* model);

	void Update();
	void Render(UINT count);

private:
	struct BoneDesc
	{
		//Deprecated
		//D3DXMATRIX Transforms[MAX_MODEL_TRANFORMS];

		UINT Index;
		float Padding[3];
	} boneDesc;

private:
	wstring name;

	class ModelMesh* parent;

	UINT pass;
	Shader* shader;

	//Deprecated
	//Transform* transform;
	PerFrame* perFrame;

	Material* material;
	wstring materialName;

	UINT startVertex;
	UINT vertexCount;

	UINT startIndex;
	UINT indexCount;

	ConstantBuffer* boneBuffer;
	ID3DX11EffectConstantBuffer* sBoneBuffer;

	ID3D11ShaderResourceView* transformsSRV;
	ID3DX11EffectShaderResourceVariable* sTransformsSrv;
};