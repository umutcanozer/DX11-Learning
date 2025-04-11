#pragma once
#include "Component.h"


class ConstantBuffer : public Component
{
public:
	ConstantBuffer(const ConstantBuffer&) = delete;
	ConstantBuffer& operator=(const ConstantBuffer&) = delete;

protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer;
};

