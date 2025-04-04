#include "GameObject.h"
#include "TransformComponent.h"
#include "ConstantBuffer.h"

GameObject::GameObject()
{

}

void GameObject::Update(float deltaTime, Graphics& gfx)
{
    struct ConstantBuffer {
        DirectX::XMMATRIX transform;
    };

    const ConstantBuffer cb = {
        {
            DirectX::XMMatrixTranspose(
                DirectX::XMMatrixRotationZ(deltaTime) *
                DirectX::XMMatrixRotationX(deltaTime) *
                DirectX::XMMatrixTranslation(0.f,0.f,4.f) *
                DirectX::XMMatrixPerspectiveLH(1.0f, 3.f / 4.f, 0.5f, 10.f)
            )
        }
    };
    std::unique_ptr<Component> cubeSharedVConstantBuffer = std::make_unique<VertexConstantBuffer<ConstantBuffer>>(gfx, cb);
}

void GameObject::Bind(Graphics& gfx)
{
    for (auto& comp : m_sharedComponents)
        comp->Bind(gfx);
}




