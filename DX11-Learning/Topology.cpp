#include "Topology.h"

Topology::Topology(D3D11_PRIMITIVE_TOPOLOGY topology) :
	m_topology(topology)
{
}

void Topology::Bind(Graphics& gfx)
{
	GetContext(gfx)->IASetPrimitiveTopology(m_topology);
}

