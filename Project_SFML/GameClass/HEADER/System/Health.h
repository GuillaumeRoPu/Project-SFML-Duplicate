#pragma once

class Health
{
public:
    Health(int maxHealth = 100);

    int   GetCurrent() const;
    int   GetMax() const;
    float GetRatio() const;

    void  TakeDamage(int amount);
    void  Heal(int amount);
    bool  IsDead() const;

    void  SetMax(int maxHealth, bool fill = true);

private:
    int m_maxHealth;
    int m_currentHealth;
};
