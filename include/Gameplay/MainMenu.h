#pragma once
#include <SFML/Graphics.hpp>
#include "./UI/Button.h"

// Estados del juego
enum class GameState
{
    Menu,
    Playing,
    GameOver,
    Exit
};

class Menu
{
public:
    Menu(sf::Font& font);
    
    // Update del menu
    void update(sf::RenderWindow& window, GameState& state, const sf::View& uiView);
    // Imprime el menu
    void render(sf::RenderWindow& window);

private:
    // Botones y texto del titulo
    sf::Text m_titleText;

    Button m_playButton;
    Button m_exitButton;
};