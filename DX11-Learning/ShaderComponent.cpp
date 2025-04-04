#include "ShaderComponent.h"

ShaderComponent::ShaderComponent(ShaderData args)
{
	HRESULT hr;
#pragma region vertex sahder
	hr = D3DReadFileToBlob(args.vsPath.c_str(), &m_pVBlob);
	if (FAILED(hr))
	{
		std::cerr << "D3DReadFileToBlob failed with error: " << hr << "\n";
		return;
	}

	hr = GetDevice(args.gfx)->CreateVertexShader(
		m_pVBlob->GetBufferPointer(), 
		m_pVBlob->GetBufferSize(), 
		nullptr, 
		&m_pVertexShader);
	if (FAILED(hr))
	{
		std::cerr << "CreateVertexShader failed with error: " << hr << "\n";
		return;
	}
#pragma endregion

#pragma region pixel shader
	hr = D3DReadFileToBlob(args.psPath.c_str(), &m_pPBlob);
	if (FAILED(hr))
	{
		std::cerr << "D3DReadFileToBlob failed with error: " << hr << "\n";
		return;
	}
	hr = GetDevice(args.gfx)->CreatePixelShader(
		m_pPBlob->GetBufferPointer(),
		m_pPBlob->GetBufferSize(), 
		nullptr, 
		&m_pPixelShader);
	if (FAILED(hr)) {
		std::cerr << "CreatePixelShader failed with error: " << hr << "\n";
		return;
	}
#pragma endregion

#pragma region input layout
	hr = GetDevice(args.gfx)->CreateInputLayout(
		args.layout.data(), (UINT)args.layout.size(),
		m_pVBlob->GetBufferPointer(),
		m_pVBlob->GetBufferSize(),
		&m_pInputLayout
	);
	if (FAILED(hr))
	{
		std::cerr << "CreateInputLayout failed with error: " << hr << "\n";
		return;
	}
#pragma endregion
}

void ShaderComponent::Bind(Graphics& gfx)
{
	GetContext(gfx)->IASetInputLayout(m_pInputLayout.Get());
	GetContext(gfx)->PSSetShader(m_pPixelShader.Get(), nullptr, 0u);
	GetContext(gfx)->VSSetShader(m_pVertexShader.Get(), nullptr, 0u);
}
