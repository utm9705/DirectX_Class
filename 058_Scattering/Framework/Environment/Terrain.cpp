#include "framework.h"
#include "Terrain.h"
#include "Viewer/Frustum.h"

Terrain::Terrain(InitializeInfo& info)
	: Renderer(info.shader), info(info)
	, baseTexture(NULL), layerTexture(NULL), alphaTexture(NULL)
{
	//Pass(1);
	Topology(D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);

	sBaseTexture = shader->AsShaderResource("BaseMap");
	sLayerTexture = shader->AsShaderResource("LayerMap");
	sAlphaTexture = shader->AsShaderResource("AlphaMap");


	sHeightTexture = shader->AsShaderResource("HeightMap");

	frustum = new Frustum(1000);
	
	buffer = new CBuffer(&bufferDesc, sizeof(BufferDesc));
	sBuffer = shader->AsConstantBuffer("CB_Terrain");

	bufferDesc.HeightRatio = info.HeightRatio;
}

Terrain::~Terrain()
{
	SAFE_DELETE_ARRAY(vertices);
	SAFE_DELETE_ARRAY(indices);

	SAFE_DELETE(heightMap);	
}

void Terrain::BaseTexture(wstring file)
{
	SAFE_DELETE(baseTexture);

	baseTexture = new Texture(file);
	sBaseTexture->SetResource(baseTexture->SRV());
}

void Terrain::LayerTexture(wstring layer, wstring alpha)
{
	SAFE_DELETE(layerTexture);
	SAFE_DELETE(alphaTexture);

	layerTexture = new Texture(L"Terrain/" + layer);
	alphaTexture = new Texture(L"Terrain/" + alpha);

	sLayerTexture->SetResource(layerTexture->SRV());
	sAlphaTexture->SetResource(alphaTexture->SRV());
}

void Terrain::Ready()
{
	heightMap = new Texture(info.heightMap);
	sHeightTexture->SetResource(heightMap->SRV());

	heightMap->ReadPixels(DXGI_FORMAT_R8G8B8A8_UINT, &heightMapPixel);


	width = this->heightMap->GetWidth() - 1;
	height = this->heightMap->GetHeight() - 1;


	patchVertexRows = (width / info.CellsPerPatch) + 1;
	patchVertexCols = (height / info.CellsPerPatch) + 1;

	vertexCount = patchVertexRows * patchVertexCols;
	faceCount = (patchVertexRows - 1) * (patchVertexCols - 1);
	indexCount = faceCount * 4;

	
	CalcBoundY();
	CreateVertexData();
	CreateIndexData();

	CreateVertexBuffer(vertices, vertexCount, sizeof(VertexTerrain), 0);
	CreateIndexBuffer(indices, indexCount);


	bufferDesc.TexelCellSpaceU = 1.0f / ((float)heightMap->GetWidth() - 1.0f);
	bufferDesc.TexelCellSpaceV = 1.0f / ((float)heightMap->GetHeight() - 1.0f);
}

void Terrain::Update()
{
	Renderer::Update();

	frustum->GetPlanes(bufferDesc.WorldFrustumPlanes);
	buffer->Apply();
}

void Terrain::Render()
{
	Renderer::Render();

	if(baseTexture != NULL)
		sBaseTexture->SetResource(baseTexture->SRV());

	if (layerTexture != NULL)
		sLayerTexture->SetResource(layerTexture->SRV());

	if (alphaTexture != NULL)
		sAlphaTexture->SetResource(alphaTexture->SRV());
	
	sBuffer->SetConstantBuffer(buffer->Buffer());
	shader->DrawIndexed(0, pass, indexCount);
}

bool Terrain::InBounds(UINT row, UINT col)
{
	return row >= 0 && row < height && col >= 0 && col < width;
}

void Terrain::CalcBoundY()
{
	bounds.assign(faceCount, D3DXVECTOR2());

	for (UINT row = 0; row < patchVertexRows - 1; row++)
	{
		for (UINT col = 0; col < patchVertexCols - 1; col++)
			CalcPatchBounds(row, col);
	}
}

void Terrain::CalcPatchBounds(UINT row, UINT col)
{
	UINT x0 = col * info.CellsPerPatch;
	UINT x1 = (col + 1) * info.CellsPerPatch;

	UINT y0 = row * info.CellsPerPatch;
	UINT y1 = (row + 1) * info.CellsPerPatch;


	float minY = FLT_MAX;
	float maxY = FLT_MIN;

	for (UINT y = y0; y <= y1; y++)
	{
		for (UINT x = x0; x <= x1; x++)
		{
			float data = 0.0f;
			if(InBounds(y, x))
				data = heightMapPixel[y * height + col].b;

			minY = min(minY, data);
			maxY = max(maxY, data);
		}
	}

	UINT patchID = row * (patchVertexCols - 1) + col;
	bounds[patchID] = D3DXVECTOR2(minY, maxY);
}

void Terrain::CreateVertexData()
{
	vertices = new VertexTerrain[vertexCount];

	float halfWidth = 0.5f * (float)width;
	float halfDepth = 0.5f * (float)height;

	float patchWidth = (float)width / (float)(patchVertexCols - 1);
	float patchDepth = (float)height / (float)(patchVertexRows - 1);

	float du = 1.0f / (float)(patchVertexCols - 1);
	float dv = 1.0f / (float)(patchVertexRows - 1);

	for (UINT row = 0; row < patchVertexRows; row++)
	{
		float z = halfDepth - (float)row * patchDepth;
		for (UINT col = 0; col < patchVertexCols; col++)
		{
			float x = -halfWidth + (float)col * patchWidth;
			UINT vertId = row * patchVertexCols + col;

			vertices[vertId].Position = D3DXVECTOR3(x, 0, z);
			vertices[vertId].Uv = D3DXVECTOR2(col * du, row * dv);
		}
	}

	for (UINT row = 0; row < patchVertexRows - 1; row++)
	{
		for (UINT col = 0; col < patchVertexCols - 1; col++)
		{
			UINT patchID = row * (patchVertexCols - 1) + col;
			UINT vertID = row * patchVertexCols + col;
			
			vertices[vertID].BoundsY = bounds[patchID];
		}
	}
}

void Terrain::CreateIndexData()
{
	vector<WORD> indices;
	for (WORD row = 0; row < (WORD)patchVertexRows - 1; row++)
	{
		for (WORD col = 0; col < (WORD)patchVertexCols - 1; col++)
		{
			indices.push_back(row * (WORD)patchVertexCols + col);
			indices.push_back(row * (WORD)patchVertexCols + col + 1);
			indices.push_back((row + 1) * (WORD)patchVertexCols + col);
			indices.push_back((row + 1) * (WORD)patchVertexCols + col + 1);
		}
	}

	this->indices = new UINT[indexCount];
	copy
	(
		indices.begin(), indices.end(),
		stdext::checked_array_iterator<UINT *>(this->indices, indexCount)
	);
}
