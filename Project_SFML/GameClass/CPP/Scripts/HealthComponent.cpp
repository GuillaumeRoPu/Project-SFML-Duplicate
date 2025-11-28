#include "Scripts/HealthComponent.h"
#include "Objects/GameObject.h"   // pour poubvoir utiliser owner->active

HealthComponent::HealthComponent(int maxHealth)
    : m_health(maxHealth)
{
}

void HealthComponent::Update()
{
    // Comportement générique à la mort, c'est pas obligatoire donc si ca vous va pas dites moi !
    // Si vous voulez gérer la mort vous-même côté Player/Enemy, vous commentez ce bloc et ca devrait être bon :)
    if (m_health.IsDead() && owner != nullptr)
    {
        owner->active = false;
    }
}

void HealthComponent::TakeDamage(int amount)
{
    m_health.TakeDamage(amount);
}

void HealthComponent::Heal(int amount)
{
    m_health.Heal(amount);
}