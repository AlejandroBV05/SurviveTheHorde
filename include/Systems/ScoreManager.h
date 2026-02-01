#pragma once
#include <string>

class ScoreManager
{
public:
    ScoreManager(const std::string& filePath);

    // Suma puntos
    void addPoints(int points);
    // Resetea puntos actuales
    void resetCurrentScore();

    // Deveulve puntuacion actual
    int getCurrentScore() const;
    //Devuelve Hi score
    int getHighScore() const;

    // Guarda los puntos al superarla
    void saveIfHighScore();

private:
    //Carga el Hi score
    void loadHighScore();
    // Guarda el Hi score
    void saveHighScore();

    // Puntuacion actual y hi score
    int m_currentScore;
    int m_highScore;
    std::string m_filePath;
};