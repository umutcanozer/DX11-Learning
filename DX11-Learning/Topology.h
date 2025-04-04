#pragma once
#include "Component.h"

class Topology : public Component
{
public:
	Topology(D3D11_PRIMITIVE_TOPOLOGY topology);
	Topology(const Topology&) = delete;
	Topology& operator=(const Topology&) = delete;
	void Bind(Graphics& gfx) override;
private:
	D3D11_PRIMITIVE_TOPOLOGY m_topology;
};

