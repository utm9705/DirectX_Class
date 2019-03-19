#pragma once

class ModelMesh : public Renderer
{
public:
	friend class Model;
	friend class Models;

public:
	void SetShader(Shader* shader) override;

	void Render();
	void RenderInstance(UINT count);

	wstring Name() { return name; }

	int ParentBoneIndex() { return parentBoneIndex; }
	class ModelBone* ParentBone() { return parentBone; }

	void Copy(ModelMesh** clone);

	Model::ModelVertex* Vertices() { return vertices; }
	UINT VertexCount() { return vertexCount; }

	ID3D11Buffer* InstanceBuffer() { return instanceBuffer; }

	void TransformsSRV(ID3D11ShaderResourceView* srv);

private:
	void Binding();

private:
	ModelMesh();
	~ModelMesh();

	wstring name;

	int parentBoneIndex;
	class ModelBone* parentBone;

	vector<class ModelMeshPart *> meshParts;

	
	UINT vertexCount;
	Model::ModelVertex* vertices;

	UINT indexCount;
	UINT* indices;

	ID3D11Buffer* instanceBuffer;


	ID3DX11EffectScalarVariable* sBoneIndex;

	ID3D11ShaderResourceView* transformsSRV;
	ID3DX11EffectShaderResourceVariable* sTransforms;
};