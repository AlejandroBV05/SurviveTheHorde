#include "./Gameplay/GameOverScreen.h"

// Config menu de game over
GameOverScreen::GameOverScreen(sf::Font& font)
    : m_retryButton("../data/textures/ui/Button_Yellow.png",
        "../data/textures/ui/Button_Yellow_Pressed.png",
        { 960.f, 650.f }, "RETRY", font), 
        m_menuButton("../data/textures/ui/Button_Purple.png",
        "../data/textures/ui/Button_Purple_Pressed.png",
        { 960.f, 780.f}, "MAIN MENU", font)
{
    // Texto de game over
    m_titleText.setFont(font);
    m_titleText.setString("GAME OVER");
    m_titleText.setCharacterSize(96);
    m_titleText.setFillColor(sf::Color::Red);

    // Puntuacion acutal
    m_scoreText.setFont(font);
    m_scoreText.setCharacterSize(48);
    m_scoreText.setFillColor(sf::Color::White);

    // Hi score
    m_highScoreText.setFont(font);
    m_highScoreText.setCharacterSize(36);
    m_highScoreText.setFillColor(sf::Color::Yellow);

    auto bounds = m_titleText.getLocalBounds();
    m_titleText.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
    m_titleText.setPosition(960.f, 200.f);
}

// Textos de la puntuacon
void GameOverScreen::setScores(int score, int highScore)
{
    // Puntuacion normal
    m_scoreText.setString("Score: " + std::to_string(score));
    auto sb = m_scoreText.getLocalBounds();
    m_scoreText.setOrigin(sb.left + sb.width / 2.f, sb.top + sb.height / 2.f);
    m_scoreText.setPosition(960.f, 360.f);

    // Hi score
    m_highScoreText.setString("High Score: " + std::to_string(highScore));
    auto hb = m_highScoreText.getLocalBounds();
    m_highScoreText.setOrigin(hb.left + hb.width / 2.f, hb.top + hb.height / 2.f);
    m_highScoreText.setPosition(960.f, 430.f);
}

// Update de los botones
void GameOverScreen::update(sf::RenderWindow& window, GameState& state, const sf::View& uiView)
{
    m_retryButton.update(window, uiView);
    m_menuButton.update(window, uiView);

    // Boton de reinicio
    if (m_retryButton.isClicked(window, uiView))
        state = GameState::Playing;

    // Boton de vuelta al main menu
    if (m_menuButton.isClicked(window, uiView)) 
    {
        state = GameState::Menu;
        return;
    }
}

// Imprime el menu
void GameOverScreen::render(sf::RenderWindow& window)
{
    window.draw(m_titleText);
    window.draw(m_scoreText);
    window.draw(m_highScoreText);

    m_retryButton.render(window);
    m_menuButton.render(window);
}