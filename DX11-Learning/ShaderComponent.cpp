#include "ShaderComponent.h"

ShaderComponent::ShaderComponent(ShaderData args)
{
	HRESULT hr;
#pragma region vertex shader
	hr = D3DReadFileToBlob(args.vsPath.c_str(), &m_pVBlob);
	if (FAILED(hr))
	{
		std::wcerr << L"Failed to load compiled shader: " << args.vsPath << L"\n";
		std::wcerr << L"HRESULT = 0x" << std::hex << hr << "\n";
		return;
	}

	hr = args.gfx.GetDevice()->CreateVertexShader(
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
		std::wcerr << L"Failed to load compiled pixel shader: " << args.psPath << L"\n";
		std::wcerr << L"HRESULT = 0x" << std::hex << hr << "\n";
		return;
	}
	hr = args.gfx.GetDevice()->CreatePixelShader(
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
	hr = args.gfx.GetDevice()->CreateInputLayout(
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
	gfx.GetDeviceContext()->IASetInputLayout(m_pInputLayout.Get());
	gfx.GetDeviceContext()->VSSetShader(m_pVertexShader.Get(), nullptr, 0u);
	gfx.GetDeviceContext()->PSSetShader(m_pPixelShader.Get(), nullptr, 0u);
}
