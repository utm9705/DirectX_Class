#pragma once

class CBuffer
{
public:
	CBuffer(void* pData, UINT dataSize);
	~CBuffer();

	void Apply();
	ID3D11Buffer* Buffer() { return buffer; }

private:
	ID3D11Buffer* buffer;

	void* data;
	UINT dataSize;
};