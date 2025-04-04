#pragma once
#include "Component.h"

class TransformComponent : public Component {
public:
	TransformComponent();
	TransformComponent(const TransformComponent&) = delete;
	TransformComponent& operator=(const TransformComponent&) = delete;

    DirectX::XMMATRIX GetWorldMatrix() const;

    void SetPosition(const DirectX::XMVECTOR& pos) { position = pos; }
    void SetRotation(const DirectX::XMVECTOR& rot) { rotation = rot; }
    void SetScale(const DirectX::XMVECTOR& sc) { scale = sc; }
private:
    DirectX::XMVECTOR position;
    DirectX::XMVECTOR rotation;
    DirectX::XMVECTOR scale;
};