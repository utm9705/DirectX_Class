#pragma once

class ModelMeshPart
{
public:
	friend class Model;
	friend class Models;
	friend class ModelMesh;

public:
	void Pass(UINT pass);

private:
	ModelMeshPart();
	~ModelMeshPart();

	void Copy(ModelMeshPart** clone);

	void Render();

private:
	UINT pass;

	wstring name;

	Material* material;
	wstring materialName;

	class ModelMesh* parent;

	UINT startVertex;
	UINT vertexCount;

	UINT startIndex;
	UINT indexCount;
};