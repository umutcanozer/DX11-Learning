#include "MeshComponent.h"

void MeshComponent::Bind(Graphics& gfx)
{
	gfx.GetDeviceContext()->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);
	gfx.GetDeviceContext()->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
}

void MeshComponent::Draw(Graphics& gfx)
{
	gfx.DrawIndexed(indexCount);
}
