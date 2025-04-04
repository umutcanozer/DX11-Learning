#pragma once
#include "Graphics.h"
#include <iostream>
class Component {
public:
	virtual ~Component() = default;
	virtual void Update() {};
	virtual void Bind(Graphics& gfx) {};

protected:
	static ID3D11DeviceContext* GetContext(Graphics& gfx);
	static ID3D11Device* GetDevice(Graphics& gfx);
};