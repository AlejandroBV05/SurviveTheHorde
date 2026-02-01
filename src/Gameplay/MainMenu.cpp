#include "./Gameplay/MainMenu.h"

// Config menu principal
Menu::Menu(sf::Font& font)
    : m_playButton("../data/textures/ui/Button_Blue.png",
        "../data/textures/ui/Button_Blue_Pressed.png",
        { 960.f, 600.f },"PLAY", font),
        m_exitButton("../data/textures/ui/Button_Red.png",
        "../data/textures/ui/Button_Red_Pressed.png",
        { 960.f, 740.f }, "EXIT", font)
{
    // Titulo del juego
    m_titleText.setFont(font);
    m_titleText.setString("SURVIVE THE HORDE");
    m_titleText.setCharacterSize(96);
    m_titleText.setFillColor(sf::Color::White);

    sf::FloatRect b = m_titleText.getLocalBounds();
    m_titleText.setOrigin(b.left + b.width / 2, b.top + b.height / 2);
    m_titleText.setPosition(960.f, 200.f);
}

// Actualiza los botones
void Menu::update(sf::RenderWindow& window, GameState& state, const sf::View& uiView)
{
    m_playButton.update(window, uiView);
    m_exitButton.update(window, uiView);

    // Boton de play
    if (m_playButton.isClicked(window, uiView))
        state = GameState::Playing;

    // Boton de cerrar ap
    if (m_exitButton.isClicked(window, uiView))
        state = GameState::Exit;
}

// Imprime el menu
void Menu::render(sf::RenderWindow& window)
{
    window.draw(m_titleText);
    m_playButton.render(window);
    m_exitButton.render(window);
}