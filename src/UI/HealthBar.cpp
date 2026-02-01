#include "./UI/HealthBar.h"
#include <algorithm>

HealthBar::HealthBar()
{
    // Valores de la vida del player
    m_maxHealth = 100.f;
    m_currentHealth = 100.f;
    
    // Icono a la izquierda de la barra
    m_iconTexture.loadFromFile("../data/textures/ui/Avatars_01.png");
    m_icon.setTexture(m_iconTexture);
    m_icon.setScale(0.5f, 0.5f);

    // Fondo de la barra de vida
    m_bg.setSize({ 200.f, 16.f });
    m_bg.setFillColor(sf::Color(40, 40, 40, 220));

    // Vida del player
    m_fill.setSize({ 200.f, 16.f });
    m_fill.setFillColor(sf::Color(200, 40, 40));
}

// Cambia la vida maxima
void HealthBar::setMaxHealth(float max)
{
    m_maxHealth = max;
}

// Cambia la vida actual
void HealthBar::setHealth(float current)
{
    // Hace que no se pase
    m_currentHealth = std::clamp(current, 0.f, m_maxHealth);

    float ratio = m_currentHealth / m_maxHealth;
    m_fill.setSize({ 200.f * ratio, 16.f });
}

// Colocacion de la barra en pantalla
void HealthBar::setPosition(const sf::Vector2f& pos)
{
    // Icono
    m_icon.setPosition(pos);
    float iconWidth = m_icon.getGlobalBounds().width;

    // Barra de vida
    sf::Vector2f barPos(pos.x + iconWidth + 10.f, pos.y + (m_icon.getGlobalBounds().height / 2.f) - 9.f);

    m_bg.setPosition(barPos);
    m_fill.setPosition(barPos);
}

// Imprime la barra y el icono
void HealthBar::render(sf::RenderWindow& window)
{
    window.draw(m_icon);
    window.draw(m_bg);
    window.draw(m_fill);
}