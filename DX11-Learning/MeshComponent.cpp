#include "MeshComponent.h"

void MeshComponent::Bind(Graphics& gfx)
{
	GetContext(gfx)->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);
	GetContext(gfx)->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
}

void MeshComponent::Draw(Graphics& gfx)
{
	GetContext(gfx)->DrawIndexed(indexCount, 0u, 0u);
}
