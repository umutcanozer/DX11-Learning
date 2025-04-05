#pragma once
#include <memory>
#include <vector>
#include "Graphics.h"
#include "Component.h"
#include "ConstantBuffer.h"

class GameObject
{
public:
    void AddComponent(std::shared_ptr<Component> comp) {
        m_sharedComponents.push_back(comp);
    } 
    
    template<typename T>
    std::shared_ptr<T> GetComponent() {
        for (auto& comp : m_sharedComponents) {
            if (auto castedComp = std::dynamic_pointer_cast<T>(comp)) {
                return castedComp;
            }
        }
        return nullptr; 
    }

    GameObject();
    GameObject(const GameObject&) = delete;
    GameObject& operator=(const GameObject&) = delete;

    void Update(float deltaTime, Graphics& gfx);
	void Bind(Graphics& gfx);
private:
    std::vector<std::shared_ptr<Component>> m_sharedComponents;
    std::unique_ptr<Component> m_sharedVConstantBuffer;
};

