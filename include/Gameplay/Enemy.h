#pragma once
#include <SFML/Graphics.hpp>

// Estados del enemigo
enum class EnemyState
{
    Idle,
    Run,
    Attack,
    AttackUp,
    AttackDown,
    Dead
};

// Hacia donde apunta el enemigo
enum class EnemyFacing
{
    Left,
    Right
};

class Enemy
{
public:
    Enemy();

    // Spawn de enemigo
    void spawn(sf::Vector2f pos);
    void update(float dt, const sf::Vector2f& playerPos);
    void render(sf::RenderWindow& window);

    // Variables de daño
    void takeDamage(float dmg);
    bool isActive() const;
    bool isDead() const;
    bool wasDeathReported() const;
    void markDeathReported();

    // Variables ataque
    bool isAttacking() const;
    bool isAttackFrame() const;
    sf::FloatRect getHitbox() const;
    sf::FloatRect getAttackHitbox() const;
    void markDamageDone();
    float m_attack;
    
    // Posicion
    sf::Vector2f getPosition() const;

private:
    void setState(EnemyState state);
    void updateAnimation(float dt);
    void clampToScreen();

private:
    // Variables del enemigo
    bool m_active = false;
    bool m_deathReported = false;
    bool m_hasDealtDamage = false;

    sf::Vector2f m_position;
    float m_speed;

    float m_health;
    float m_maxHealth;

    EnemyState m_state;
    EnemyFacing m_facing;

    int m_currentFrame;
    int m_totalFrames;
    float m_frameTime;
    float m_elapsedTime;

    int m_frameWidth;
    int m_frameHeight;

    sf::Sprite m_sprite;
    sf::Texture m_idleTexture;
    sf::Texture m_runTexture;
    sf::Texture m_attackTexture;
    sf::Texture m_attackUpTexture;
    sf::Texture m_attackDownTexture;
    sf::Texture m_deadTexture;
};