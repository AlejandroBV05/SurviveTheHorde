#pragma once
#include <vector>
#include "./Gameplay/Enemy.h"
#include <cstdlib>

// Numero random entre ek minimo y el maximo
inline float randf(float min, float max)
{
    return min + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (max - min);
}

class EnemyPool
{
public:
    EnemyPool(std::size_t size);

    // Spawn del enemigo
    void spawn(sf::Vector2f pos);
    // Spawn de la wave
    void spawnWave(int count, const sf::Vector2f& center, float radius);
    void update(float dt, const sf::Vector2f& playerPos);
    // Enemigos vivos
    int getAliveCount() const;
    void render(sf::RenderWindow& window);
    void reset();

    // Lista de enemigos
    std::vector<Enemy>& getEnemies();

private:
    //Vector dondfe se guardan los enemigos
    std::vector<Enemy> m_enemies;
};