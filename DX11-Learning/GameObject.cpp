#include "GameObject.h"
#include "TransformComponent.h"
#include "ConstantBuffer.h"

GameObject::GameObject()
{
}


void GameObject::Update(float deltaTime, Graphics& gfx)
{

}

void GameObject::Bind(Graphics& gfx)
{
    for (auto& comp : m_sharedComponents)
        comp->Bind(gfx);
}




