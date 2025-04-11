#include "Topology.h"

Topology::Topology(D3D11_PRIMITIVE_TOPOLOGY topology) :
	m_topology(topology)
{
}

void Topology::Bind(Graphics& gfx)
{
	gfx.GetDeviceContext()->IASetPrimitiveTopology(m_topology);
}

