#include "Health.h"

Health::Health(int maxHealth)
    : m_maxHealth(maxHealth),
    m_currentHealth(maxHealth)
{
}

int Health::GetCurrent() const
{
    return m_currentHealth;
}

int Health::GetMax() const
{
    return m_maxHealth;
}

float Health::GetRatio() const
{
    if (m_maxHealth <= 0) return 0.f;
    return static_cast<float>(m_currentHealth) / static_cast<float>(m_maxHealth);
}

void Health::TakeDamage(int amount)
{
    m_currentHealth -= amount;
    if (m_currentHealth < 0)
        m_currentHealth = 0;
}

void Health::Heal(int amount)
{
    m_currentHealth += amount;
    if (m_currentHealth > m_maxHealth)
        m_currentHealth = m_maxHealth;
}

bool Health::IsDead() const
{
    return m_currentHealth <= 0;
}

void Health::SetMax(int maxHealth, bool fill)
{
    m_maxHealth = maxHealth;
    if (fill)
        m_currentHealth = maxHealth;
    else if (m_currentHealth > m_maxHealth)
        m_currentHealth = m_maxHealth;
}
