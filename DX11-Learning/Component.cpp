#include "Component.h"

ID3D11DeviceContext* Component::GetContext(Graphics& gfx)
{
	return gfx.m_pDeviceContext.Get();
}

ID3D11Device* Component::GetDevice(Graphics& gfx)
{
	return gfx.m_pDevice.Get();
}