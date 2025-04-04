#include "TransformComponent.h"

TransformComponent::TransformComponent()
	: position(0.f, 0.f, 0.f), rotation(0.f, 0.f, 0.f), scale(1.f, 1.f, 1.f)
{
}

DirectX::XMMATRIX TransformComponent::GetWorldMatrix() const
{
	using namespace DirectX;
	XMMATRIX translation = XMMatrixTranslationFromVector(position);
	XMMATRIX rotationMat = XMMatrixRotationRollPitchYawFromVector(rotation);
	XMMATRIX scaling = XMMatrixScalingFromVector(scale);
	return scaling * rotationMat * translation;
}
