#include "./UI/Button.h"

// Config de los botones
Button::Button(const std::string& normalTex, const std::string& hoverTex, sf::Vector2f position, const std::string& label, sf::Font& font)
{
    // Texturas y ajuste de sprites de los botones
    m_normalTexture.loadFromFile(normalTex);
    m_hoverTexture.loadFromFile(hoverTex);

    m_sprite.setTexture(m_normalTexture);
    m_sprite.setPosition(position);
    m_sprite.setScale(2.f, 2.f);
    m_sprite.setOrigin(m_sprite.getLocalBounds().width / 2.f, m_sprite.getLocalBounds().height / 2.f);
    
    m_text.setFont(font);
    m_text.setString(label);
    m_text.setCharacterSize(28);
    m_text.setFillColor(sf::Color::Black);

    sf::FloatRect tb = m_text.getLocalBounds();
    m_text.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 1.f);
    m_text.setPosition(position);
}

// Comprueba si el mouse esta encima del boton
bool Button::isMouseOver(sf::RenderWindow& window, const sf::View& uiView)
{
    sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
    sf::Vector2f mousePos = window.mapPixelToCoords(mousePixel, uiView);

    return m_sprite.getGlobalBounds().contains(mousePos);
}

// Actualiza el boton dependiendo de si estas encima o no
void Button::update(sf::RenderWindow& window, const sf::View& uiView)
{
    if (isMouseOver(window, uiView))
        m_sprite.setTexture(m_hoverTexture);
    else
        m_sprite.setTexture(m_normalTexture);
}

// Comprueba si el boton fue pulsado
bool Button::isClicked(sf::RenderWindow& window, const sf::View& uiView)
{
    bool pressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);

    if (!pressed && m_wasPressed && isMouseOver(window, uiView))
    {
        m_wasPressed = false;
        return true;
    }

    m_wasPressed = pressed;
    return false;
}

void Button::render(sf::RenderWindow& window)
{
    window.draw(m_sprite);
    window.draw(m_text);
}