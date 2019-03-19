#include "Framework.h"
#include "Sky.h"
#include "Renders/Render2D.h"
#include "Viewer/RenderTargetView.h"

Sky::Sky()
	: Renderer()
	, realTime(false), theta(0), phi(0)
	, radius(1), slices(32), stacks(32)
{
	shader = new Shader(L"058_Scattering.fx");
	SetShader(shader);

	mieTarget = new RenderTargetView(128, 64);
	rayleighTarget = new RenderTargetView(128, 64);

	render2D = new Render2D();
	render2D->Scale(200, 200, 1);
	render2D->RotationDegree(0, 0, -90);

	targetDesc.InvWaveLength.x = 1.0f / powf(targetDesc.WaveLength.x, 4.0f);
	targetDesc.InvWaveLength.y = 1.0f / powf(targetDesc.WaveLength.y, 4.0f);
	targetDesc.InvWaveLength.z = 1.0f / powf(targetDesc.WaveLength.z, 4.0f);

	targetDesc.WaveLengthMie.x = powf(targetDesc.WaveLength.x, -0.84f);
	targetDesc.WaveLengthMie.y = powf(targetDesc.WaveLength.y, -0.84f);
	targetDesc.WaveLengthMie.z = powf(targetDesc.WaveLength.z, -0.84f);

	targetBuffer = new CBuffer(&targetDesc, sizeof(TargetDesc));
	sTargetBuffer = shader->AsConstantBuffer("CB_Target");

	sStarIntensity = shader->AsScalar("StarIntensity");
	sStarMap = shader->AsShaderResource("StarMap");
	sRayleighMap = shader->AsShaderResource("RayleighMap");
	sMieMap = shader->AsShaderResource("MieMap");
}

Sky::~Sky()
{
	SAFE_DELETE(targetBuffer);
	SAFE_DELETE(render2D);
	SAFE_DELETE(shader);

	SAFE_DELETE(mieTarget);
	SAFE_DELETE(rayleighTarget);

	SAFE_DELETE_ARRAY(vertices);
}

void Sky::Ready()
{
	render2D->Ready();

	starMap = new Texture(L"Starfield.png");
	sStarMap->SetResource(starMap->SRV());

	CreateQuadData();
	CreateSphereData();
	CreateCloudData();
}

void Sky::Update()
{
	Renderer::Update();

	render2D->Update();
	targetBuffer->Apply();
	sTargetBuffer->SetConstantBuffer(targetBuffer->Buffer());

	D3DXVECTOR3 position;
	Context::Get()->GetCamera()->Position(&position);
	
	position.y -= 0.2f;
	Position(position);
}

void Sky::PreRender()
{
	if (prevTheta == theta && prevPhi == phi)
		return;

	mieTarget->Set();
	rayleighTarget->Set();

	ID3D11RenderTargetView* rtvs[2];
	rtvs[0] = rayleighTarget->GetRTV();
	rtvs[1] = mieTarget->GetRTV();

	ID3D11DepthStencilView* dsv;
	dsv = rayleighTarget->GetDSV();

	D3D::Get()->SetRenderTargets(2, rtvs, dsv);

	Renderer::Render();
	shader->Draw(0, 0, 6);
}

void Sky::RenderCloud()
{

}

void Sky::Render()
{
	UINT stride = sizeof(VertexTexture);
	UINT offset = 0;

	D3D::GetDC()->IASetVertexBuffers(0, 1, &skyVB, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(skyIB, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//빛의 높이 = 해의 높이
	sStarIntensity->SetFloat(Context::Get()->LightDirection().y);
	sRayleighMap->SetResource(rayleighTarget->GetSRV());
	sMieMap->SetResource(mieTarget->GetSRV());

	shader->DrawIndexed(0, 1, skyIC);
}

void Sky::PostRender()
{
	render2D->SRV(rayleighTarget->GetSRV());
	render2D->Position(0 + 100, 0 + 100, 0);
	render2D->Update();
	render2D->Render();

	render2D->SRV(mieTarget->GetSRV());
	render2D->Position(200 + 100, 0 + 100, 0);
	render2D->Update();
	render2D->Render();
}

void Sky::CreateQuadData()
{
	VertexTexture quadVertices[6];

	quadVertices[0].Position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	quadVertices[1].Position = D3DXVECTOR3(-1.0f, +1.0f, 0.0f);
	quadVertices[2].Position = D3DXVECTOR3(+1.0f, -1.0f, 0.0f);
	quadVertices[3].Position = D3DXVECTOR3(+1.0f, -1.0f, 0.0f);
	quadVertices[4].Position = D3DXVECTOR3(-1.0f, +1.0f, 0.0f);
	quadVertices[5].Position = D3DXVECTOR3(+1.0f, +1.0f, 0.0f);

	quadVertices[0].Uv = D3DXVECTOR2(0, 1);
	quadVertices[1].Uv = D3DXVECTOR2(0, 0);
	quadVertices[2].Uv = D3DXVECTOR2(1, 1);
	quadVertices[3].Uv = D3DXVECTOR2(1, 1);
	quadVertices[4].Uv = D3DXVECTOR2(0, 0);
	quadVertices[5].Uv = D3DXVECTOR2(1, 0);

	CreateVertexBuffer(quadVertices, 6, sizeof(VertexTexture), 0);
}

void Sky::CreateSphereData()
{
	UINT domeCount = 32;
	UINT latitude = domeCount / 2; // 위도
	UINT longitude = domeCount; // 경도

	skyVC = longitude * latitude * 2;
	skyIC = (longitude - 1) * (latitude - 1) * 2 * 8;

	VertexTexture* vertices = new VertexTexture[skyVC];

	UINT index = 0;
	for (UINT i = 0; i < longitude; i++)
	{
		float xz = 100.0f * (i / (longitude - 1.0f)) * Math::PI / 180.0f;

		for (UINT j = 0; j < latitude; j++)
		{
			float y = Math::PI * j / (latitude - 1);

			vertices[index].Position.x = sinf(xz) * cosf(y);
			vertices[index].Position.y = cosf(xz);
			vertices[index].Position.z = sinf(xz) * sinf(y);
			vertices[index].Position *= (float)radius; // 크기를 키우려고 임의의 값 곱한거

			vertices[index].Uv.x = 0.5f / (float)longitude + i / (float)longitude;
			vertices[index].Uv.y = 0.5f / (float)latitude + j / (float)latitude;

			index++;
		} // for(j)
	}  // for(i)

	for (UINT i = 0; i < longitude; i++)
	{
		float xz = 100.0f * (i / (longitude - 1.0f)) * Math::PI / 180.0f;

		for (UINT j = 0; j < latitude; j++)
		{
			float y = (Math::PI * 2.0f) - (Math::PI * j / (latitude - 1));

			vertices[index].Position.x = sinf(xz) * cosf(y);
			vertices[index].Position.y = cosf(xz);
			vertices[index].Position.z = sinf(xz) * sinf(y);
			vertices[index].Position *= (float)radius; // 크기를 키우려고 임의의 값 곱한거

			vertices[index].Uv.x = 0.5f / (float)longitude + i / (float)longitude;
			vertices[index].Uv.y = 0.5f / (float)latitude + j / (float)latitude;

			index++;
		} // for(j)
	}  // for(i)


	index = 0;
	UINT* indices = new UINT[skyIC * 3];

	for (UINT i = 0; i < longitude - 1; i++)
	{
		for (UINT j = 0; j < latitude - 1; j++)
		{
			indices[index++] = i * latitude + j;
			indices[index++] = (i + 1) * latitude + j;
			indices[index++] = (i + 1) * latitude + (j + 1);

			indices[index++] = (i + 1) * latitude + (j + 1);
			indices[index++] = i * latitude + (j + 1);
			indices[index++] = i * latitude + j;
		}
	}

	UINT offset = latitude * longitude;
	for (UINT i = 0; i < longitude - 1; i++)
	{
		for (UINT j = 0; j < latitude - 1; j++)
		{
			indices[index++] = offset + i * latitude + j;
			indices[index++] = offset + (i + 1) * latitude + (j + 1);
			indices[index++] = offset + (i + 1) * latitude + j;

			indices[index++] = offset + i * latitude + (j + 1);
			indices[index++] = offset + (i + 1) * latitude + (j + 1);
			indices[index++] = offset + i * latitude + j;
		}
	}

	//CreateVertexBuffer
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.ByteWidth = sizeof(VertexTexture) * skyVC;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = vertices;

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &skyVB);
		assert(SUCCEEDED(hr));
	}

	//CreateIndexBuffer
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.ByteWidth = sizeof(UINT) * skyIC;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = indices;

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &skyIB);
		assert(SUCCEEDED(hr));
	}

	SAFE_DELETE_ARRAY(vertices);
	SAFE_DELETE_ARRAY(indices);
}

void Sky::CreateCloudData()
{
	int perm[256] =
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

	int gradValues[48] =
	{
		+1,+1,+0, -1,+1,+0, +1,-1,+0, -1,-1,+0,
		+1,+0,+1, -1,+0,+1, +1,+0,-1, -1,+0,-1, 	
		+0,+1,+1, +0,-1,+1, +0,+1,-1, +0,-1,-1, 
		+1,+1,+0, +0,-1,+1, -1,+1,+0, +0,-1,-1
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

			pixels[y * 256 + x] = color;
		}
	}

	D3D11_SUBRESOURCE_DATA subResource = { 0 };
	subResource.pSysMem = pixels;
	subResource.SysMemPitch = 256 * 4;

	HRESULT hr = D3D::GetDevice()->CreateTexture2D(&desc, &subResource, &cloudTexture);
	assert(SUCCEEDED(hr));

	CsResource::CreateSrv(cloudTexture, &cloudSrv);
	SAFE_DELETE_ARRAY(pixels);

	D3DX11SaveTextureToFile(D3D::GetDC(), cloudTexture, D3DX11_IFF_PNG, L"../Cloud.png");
}
