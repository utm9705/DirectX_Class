#include "Framework.h"
#include "Cloud.h"

Cloud::Cloud(Shader * shader)
	: shader(shader), texture(NULL), srv(NULL)
{

}

Cloud::~Cloud()
{
	SAFE_RELEASE(vertexBuffer);

	SAFE_RELEASE(texture);
	SAFE_RELEASE(srv);
}

void Cloud::Ready()
{
	CreateTexture();


	VertexTexture vertices[6];

	vertices[0].Position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	vertices[1].Position = D3DXVECTOR3(-1.0f, +1.0f, 0.0f);
	vertices[2].Position = D3DXVECTOR3(+1.0f, -1.0f, 0.0f);
	vertices[3].Position = D3DXVECTOR3(+1.0f, -1.0f, 0.0f);
	vertices[4].Position = D3DXVECTOR3(-1.0f, +1.0f, 0.0f);
	vertices[5].Position = D3DXVECTOR3(+1.0f, +1.0f, 0.0f);

	vertices[0].Uv = D3DXVECTOR2(0, 1);
	vertices[1].Uv = D3DXVECTOR2(0, 0);
	vertices[2].Uv = D3DXVECTOR2(1, 1);
	vertices[3].Uv = D3DXVECTOR2(1, 1);
	vertices[4].Uv = D3DXVECTOR2(0, 0);
	vertices[5].Uv = D3DXVECTOR2(1, 0);


	Renderer::CreateVertexBuffer(vertices, 6, sizeof(VertexTexture), 0, &vertexBuffer);
}

void Cloud::Update()
{
}

void Cloud::Render()
{
	UINT stride = sizeof(VertexTexture);
	UINT offset = 0;

	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	shader->Draw(0, 2, 6);
}

void Cloud::CreateTexture()
{
	int perm[] =
	{
		151,160,137,91,90,15,
		131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
		190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
		88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
		77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
		102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
		135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
		5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
		223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
		129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
		251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
		49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
		138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
	};

	int gradValues[] =
	{
		+1, +1, +0, -1, +1, +0, +1, -1, +0, -1, -1, +0,
		+1, +0, +1, -1, +0, +1, +1, +0, -1, -1, +0, -1,
		+0, +1, +1, +0, -1, +1, +0, +1, -1, +0, -1, -1,
		+1, +1, +0, +0, -1, +1, -1, +1, +0, +0, -1, -1
	};

	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
	desc.Width = 256;
	desc.Height = 256;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	D3DXCOLOR* pixels = new D3DXCOLOR[256 * 256];
	for (int y = 0; y < 256; y++)
	{
		for (int x = 0; x < 256; x++)
		{
			int value = perm[(x + perm[y]) & 0xFF];

			D3DXCOLOR color;
			color.r = (float)(gradValues[value & 0x0F] * 64 + 64);
			color.g = (float)(gradValues[value & 0x0F + 1] * 64 + 64);
			color.b = (float)(gradValues[value & 0x0F + 2] * 64 + 64);
			color.a = (float)value;

			
			UINT index = desc.Width * y + x;
			pixels[index] = color;
		}
	}

	D3D11_SUBRESOURCE_DATA subResource = { 0 };
	subResource.pSysMem = pixels;
	subResource.SysMemPitch = 256 * 4;

	HRESULT hr = D3D::GetDevice()->CreateTexture2D(&desc, &subResource, &texture);
	assert(SUCCEEDED(hr));

	CsResource::CreateSrv(texture, &srv);
	SAFE_DELETE_ARRAY(pixels);

	/*hr = D3DX11SaveTextureToFile(D3D::GetDC(), cloudTexture, D3DX11_IFF_PNG, L"../Cloud.png");
	assert(SUCCEEDED(hr));*/
}
