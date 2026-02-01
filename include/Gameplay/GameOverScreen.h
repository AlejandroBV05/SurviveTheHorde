#pragma once
#include <SFML/Graphics.hpp>
#include "./UI/Button.h"
#include "./Gameplay/MainMenu.h"

class GameOverScreen
{
public:
    GameOverScreen(sf::Font& font);

    // Update del menu
    void update(sf::RenderWindow& window, GameState& state, const sf::View& uiView);
    // Imprime el menu
    void render(sf::RenderWindow& window);

    // Textos de la puntuacon
    void setScores(int score, int highScore);

private:
    // Textos de las puntuaciones y botones
    sf::Text m_titleText;
    sf::Text m_scoreText;
    sf::Text m_highScoreText;

    Button m_retryButton;
    Button m_menuButton;
};