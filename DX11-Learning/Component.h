#pragma once
#include "Graphics.h"
#include <iostream>

class Component {
public:
	virtual ~Component() = default;
	virtual void Bind(Graphics& gfx) = 0;
};