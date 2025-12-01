#pragma once

#include "Components/Component.h"
#include "System/Health.h"

class HealthComponent : public Component
{
public:

    HealthComponent();

    void Awake() override {}
    void Start() override {}
    void Update() override;
    void FixedUpdate() override {}

    void TakeDamage(int amount);
    void Heal(int amount);

    bool IsDead() const { return m_health.IsDead(); }

    Health& GetHealth() { return m_health; }
    const Health& GetHealth() const { return m_health; }

    void SetMaxHealth(int max, bool fill = true) { m_health.SetMax(max, fill); }

private:
    Health m_health;
};
