#pragma once
#include "Component.h"

class MeshComponent : public Component
{
public:
	template<class V>
	MeshComponent(Graphics& gfx,
	const std::vector<V>& vertices,
	const std::vector<unsigned short> indices) :
	stride(sizeof(V))
	{
		vertexCount = static_cast<unsigned int>(vertices.size());
		indexCount = static_cast<unsigned int>(indices.size());

#pragma region vertex buffer
		D3D11_BUFFER_DESC vbDesc = {};
		D3D11_SUBRESOURCE_DATA vData = {};

		vbDesc.Usage = D3D11_USAGE_DEFAULT;
		vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbDesc.CPUAccessFlags = 0;
		vbDesc.MiscFlags = 0;
		vbDesc.ByteWidth = UINT(sizeof(V) * vertices.size());
		vbDesc.StructureByteStride = sizeof(V);

		vData.pSysMem = vertices.data();
		hr = GetDevice(gfx)->CreateBuffer(&vbDesc, &vData, &m_pVertexBuffer);
		if (FAILED(hr))
		{
			std::cerr << "CreateBuffer failed with error: " << hr << std::endl;
			return;
		}
#pragma endregion

#pragma region index buffer
		D3D11_BUFFER_DESC ibDesc = {};
		D3D11_SUBRESOURCE_DATA iData = {};
		ibDesc.Usage = D3D11_USAGE_DEFAULT;
		ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibDesc.CPUAccessFlags = 0;
		ibDesc.MiscFlags = 0;
		ibDesc.ByteWidth = UINT(indexCount * sizeof(unsigned short));
		ibDesc.StructureByteStride = sizeof(unsigned short);

		iData.pSysMem = indices.data();
		hr = GetDevice(gfx)->CreateBuffer(&ibDesc, &iData, &m_pIndexBuffer);
		if (FAILED(hr))
		{
			std::cerr << "CreateBuffer failed with error: " << hr << std::endl;
			return;
		}
#pragma endregion
	}
	MeshComponent(const MeshComponent&) = delete;
	MeshComponent& operator=(const MeshComponent&) = delete;


	void Draw(Graphics& gfx);
	void Bind(Graphics& gfx) override;
private:
	HRESULT hr;
	UINT vertexCount;
	UINT indexCount;

	UINT stride;
	UINT offset;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVertexBuffer;

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer;

	D3D11_PRIMITIVE_TOPOLOGY type;
};

