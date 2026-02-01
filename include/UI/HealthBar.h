#pragma once
#include <SFML/Graphics.hpp>

class HealthBar
{
public:
    HealthBar();

    // Cambia la vida maxima
    void setMaxHealth(float max);
    // Cambia la vida actual
    void setHealth(float current);

    // Posicion de la barra e icono
    void setPosition(const sf::Vector2f& pos);

    void render(sf::RenderWindow& window);

private:

    // Variables de la vida
    float m_maxHealth;
    float m_currentHealth;

    sf::RectangleShape m_bg;
    sf::RectangleShape m_fill;
    
    sf::Texture m_iconTexture;
    sf::Sprite  m_icon;
};