#pragma once

class ModelMesh
{
public:
	friend class Model;
	friend class Models;

public:
	void Render();
	void RenderInstance(UINT count);

	wstring Name() { return name; }

	int ParentBoneIndex() { return parentBoneIndex; }
	class ModelBone* ParentBone() { return parentBone; }

	void Copy(ModelMesh** clone);

	void Pass(UINT pass);

	ID3D11Buffer* VertexBuffer() { return vertexBuffer; }
	VertexTextureNormalBlend* Vertices() { return vertices; }
	UINT VertexCount() { return vertexCount; }

	ID3D11Buffer* InstanceBuffer() { return instanceBuffer; }

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
	VertexTextureNormalBlend* vertices;
	ID3D11Buffer* vertexBuffer;

	UINT indexCount;
	UINT* indices;
	ID3D11Buffer* indexBuffer;

	ID3D11Buffer* instanceBuffer;
};