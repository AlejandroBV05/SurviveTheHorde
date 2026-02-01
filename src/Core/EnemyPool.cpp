#include "./Core/EnemyPool.h"

// Numero maximo de enemigos a spawnear
EnemyPool::EnemyPool(size_t count)
{
    m_enemies.resize(count);
}

// Spawn del enemigo
void EnemyPool::spawn(sf::Vector2f pos)
{
    for (auto& e : m_enemies)
    {
        if (!e.isActive())
        {
            e.spawn(pos);
            return;
        }
    }
}

// Spàwn de las oleadas
void EnemyPool::spawnWave(int count, const sf::Vector2f& center, float radius)
{
    int spawned = 0;

    // Posicion alrededor del player
    for (int i = 0; i < count; ++i)
    {
        float angle = (2.f * 3.14159f / count) * i;
        sf::Vector2f pos(center.x + std::cos(angle) * radius, center.y + std::sin(angle) * radius);

        spawn(pos);
    }
}

// Actualiza los enemigos
void EnemyPool::update(float dt, const sf::Vector2f& playerPos)
{
    for (auto& e : m_enemies)
        e.update(dt, playerPos);
}

// Comprueba los enemigos aun vivos
int EnemyPool::getAliveCount() const
{
    int alive = 0;
    for (const auto& e : m_enemies)
        if (e.isActive())
            alive++;
    return alive;
}

// Reset de los enemigos
void EnemyPool::reset()
{
    for (auto& e : m_enemies) 
    {
        if(e.isActive())
            e.takeDamage(9999.f);
    }
}

// Imprime todos los enemigos
void EnemyPool::render(sf::RenderWindow& window)
{
    for (auto& e : m_enemies)
        e.render(window);
}

//Vector que guarda los enemigos
std::vector<Enemy>& EnemyPool::getEnemies()
{
    return m_enemies;
}