#pragma once
#include <SFML/Graphics.hpp>

// Estados del player
enum class PlayerState
{
    Idle,
    Run,
    Attack,
    AttackUp,
    AttackDown,
    Dead
};

// Hacia donde apunta el enemigo
enum class Facing
{
    Left,
    Right
};

class Player
{ 
public:
    Player();

    void update(float deltaTime);
    void render(sf::RenderWindow& window);

    // Variables de la vida
    void takeDamage(float dmg);
    float getHealth() const;
    float getMaxHealth() const;

    // Posicion y daño
    sf::Vector2f getPosition() const;
    bool isAttackFrame() const;
    sf::FloatRect getHitbox() const;
    sf::FloatRect getAttackHitbox() const;
    void markDamageDone();
    float m_attack;
    bool isAttacking() const;
    bool isDeadAnimationFinished() const;

    void reset();

private:
    void updateAnimation(float deltaTime);
    void setState(PlayerState state);
    void clampToScreen();

private:
    // Variables del player
    sf::Vector2f m_position;
    float m_speed;
    bool m_hasDealtDamage = false;

    int m_currentFrame;
    int m_totalFrames;
    float m_frameTime;
    float m_elapsedTime;
    int m_frameWidth;
    int m_frameHeight;

    PlayerState m_state;
    Facing m_facing;

    bool m_attackHeld;

    float m_health;
    float m_maxHealth;
    bool m_deadFinished = false;

    sf::Texture m_idleTexture;
    sf::Texture m_runTexture;
    sf::Texture m_attackTexture;
    sf::Texture m_attackUpTexture;
    sf::Texture m_attackDownTexture;
    sf::Texture m_deadTexture;
    sf::Sprite  m_sprite;
};
