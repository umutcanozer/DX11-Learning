#pragma once
#include "Component.h"

template<typename C>
class ConstantBuffer : public Component
{
public:
	void UpdateBuffer(Graphics& gfx, const C& consts)
	{

		D3D11_MAPPED_SUBRESOURCE msr;
		GetContext(gfx)->Map(
			m_pConstantBuffer.Get(), 0u,
			D3D11_MAP_WRITE_DISCARD, 0u,
			&msr
		);
		memcpy(msr.pData, &consts, sizeof(consts));
		GetContext(gfx)->Unmap(m_pConstantBuffer.Get(), 0u);
	}
	ConstantBuffer(Graphics& gfx, const C& consts) {
		D3D11_BUFFER_DESC cbDesc = {};
		D3D11_SUBRESOURCE_DATA cData = {};
		cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbDesc.Usage = D3D11_USAGE_DYNAMIC;
		cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbDesc.MiscFlags = 0u;
		cbDesc.ByteWidth = sizeof(C);
		cbDesc.StructureByteStride = 0u;

		cData.pSysMem = &consts;
		HRESULT hr = GetDevice(gfx)->CreateBuffer(&cbDesc, &cData, &m_pConstantBuffer);
		if (FAILED(hr)) {
			std::cerr << "CreateBuffer failed with error: " << hr << std::endl;
			return;
		}
	}ConstantBuffer(Graphics& gfx)
	{
		D3D11_BUFFER_DESC cbd;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(C);
		cbd.StructureByteStride = 0u;
		HRESULT hr = GetDevice(gfx)->CreateBuffer(&cbd, nullptr, &m_pConstantBuffer);
		if (FAILED(hr)) {
			std::cerr << "CreateBuffer failed with error: " << hr << std::endl;
			return;
		}
	}
	ConstantBuffer(const ConstantBuffer&) = delete;
	ConstantBuffer& operator=(const ConstantBuffer&) = delete;

protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer;
};

template<typename C>
class VertexConstantBuffer : public ConstantBuffer<C>
{
	using ConstantBuffer<C>::m_pConstantBuffer;
	using Component::GetContext;
public:
	using ConstantBuffer<C>::ConstantBuffer;
	void Bind(Graphics& gfx) override
	{
		GetContext(gfx)->VSSetConstantBuffers(0u, 1u, m_pConstantBuffer.GetAddressOf());
	}
};

template<typename C>
class PixelConstantBuffer : public ConstantBuffer<C>
{
	using ConstantBuffer<C>::m_pConstantBuffer;
	using Component::GetContext;
public:
	using ConstantBuffer<C>::ConstantBuffer;
	void Bind(Graphics& gfx) override
	{
		GetContext(gfx)->PSSetConstantBuffers(0u, 1u, m_pConstantBuffer.GetAddressOf());
	}
};

