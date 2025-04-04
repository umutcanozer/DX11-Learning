#pragma once
#include "Component.h"

struct ShaderData
{
	Graphics& gfx;
	std::wstring vsPath;
	std::wstring psPath;
	std::vector<D3D11_INPUT_ELEMENT_DESC>& layout;
};

class ShaderComponent : public Component
{
public:
    ShaderComponent(ShaderData args);
    ShaderComponent(const ShaderComponent&) = delete;
    ShaderComponent& operator=(const ShaderComponent&) = delete;
	void Bind(Graphics& gfx) override;
private:
    Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;

    Microsoft::WRL::ComPtr<ID3DBlob> m_pVBlob;
    Microsoft::WRL::ComPtr<ID3DBlob> m_pPBlob;
};