#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Button
{
public:
    Button(const std::string& normalTex, const std::string& hoverTex, sf::Vector2f position, const std::string& label, sf::Font& font);

    //Comprueba si el boton fue pulsado
    bool isClicked(sf::RenderWindow& window, const sf::View& uiView);
    // Actualiza el boton
    void update(sf::RenderWindow& window, const sf::View& uiView);
    void render(sf::RenderWindow& window);

private:
    // Comprueba si el mouse esta encima del boton
    bool isMouseOver(sf::RenderWindow& window, const sf::View& uiView);
    bool m_wasPressed = false;

    // Texturas de los botones y texto del boton
    sf::Texture m_normalTexture;
    sf::Texture m_hoverTexture;
    sf::Sprite  m_sprite;
    
    sf::Text m_text;
};