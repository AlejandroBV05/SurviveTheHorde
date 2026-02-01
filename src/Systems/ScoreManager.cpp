#include "./Systems/ScoreManager.h"
#include <fstream>

// Carga del HI score
ScoreManager::ScoreManager(const std::string& filePath)
    : m_currentScore(0), m_highScore(0), m_filePath(filePath)
{
    loadHighScore();
}

// Suma a la puntuacion actual
void ScoreManager::addPoints(int points)
{
    m_currentScore += points;
}

// Reset de los puntos actuales
void ScoreManager::resetCurrentScore()
{
    m_currentScore = 0;
}

// Devuelve puntuacion actual
int ScoreManager::getCurrentScore() const
{
    return m_currentScore;
}

// Devuelve Hi score
int ScoreManager::getHighScore() const
{
    return m_highScore;
}

// Guarda la puntuacion al supoerarla
void ScoreManager::saveIfHighScore()
{
    if (m_currentScore > m_highScore)
    {
        m_highScore = m_currentScore;
        saveHighScore();
    }
}

// Carga data driven del Hi score
void ScoreManager::loadHighScore()
{
    std::ifstream file(m_filePath);
    if (file.is_open())
        file >> m_highScore;
}

// Guarda el Hi score en el fichero
void ScoreManager::saveHighScore()
{
    std::ofstream file(m_filePath);
    if (file.is_open())
        file << m_highScore;
}