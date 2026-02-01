#include "./Gameplay/Enemy.h"
#include "./Core/ConfigLoader.h"
#include <cmath>

// Config del enemigo
Enemy::Enemy()
    : m_active(false),
    m_speed(120.f), m_health(50.f),
    m_maxHealth(50.f), m_state(EnemyState::Idle),
    m_facing(EnemyFacing::Left), m_currentFrame(0),
    m_totalFrames(1), m_frameTime(0.12f),
    m_elapsedTime(0.f), m_frameWidth(192), m_frameHeight(192)
{
    // Carga de valores tipo Data-drivem
    auto config = ConfigLoader::load("../data/config/enemy.txt");

    m_maxHealth = config["max_health"];
    m_health = m_maxHealth;
    m_speed = config["speed"];
    m_attack = config["attack"];

    //Texturas del enemigo y ajuste de sprites
    m_idleTexture.loadFromFile("../data/textures/enemy/Torch_Idle.png");
    m_runTexture.loadFromFile("../data/textures/enemy/Torch_Run.png");
    m_attackTexture.loadFromFile("../data/textures/enemy/Torch_Attack.png");
    m_attackUpTexture.loadFromFile("../data/textures/enemy/Torch_AttackUp.png");
    m_attackDownTexture.loadFromFile("../data/textures/enemy/Torch_AttackDown.png");
    m_deadTexture.loadFromFile("../data/textures/enemy/Torch_Dead.png");

    m_sprite.setTexture(m_idleTexture);
    m_sprite.setOrigin(m_frameWidth / 2.f, m_frameHeight * 0.85f);
    m_sprite.setScale(2.f, 2.f);
}

// Spawn del enemigo
void Enemy::spawn(sf::Vector2f pos)
{
    m_position = pos;
    m_health = m_maxHealth;
    m_active = true;
    m_deathReported = false;
    setState(EnemyState::Idle);
}

bool Enemy::isActive() const
{
    return m_active;
}

bool Enemy::isDead() const
{
    return m_state == EnemyState::Dead;
}

// Comprueba si ya ha contado la muerte
bool Enemy::wasDeathReported() const
{
    return m_deathReported;
}

// Marca que ya se conto la muerte
void Enemy::markDeathReported()
{
    m_deathReported = true;
}

sf::Vector2f Enemy::getPosition() const
{
    return m_position;
}

// Comprueba cuando puede hacer daño
bool Enemy::isAttackFrame() const
{
    if (m_state != EnemyState::Attack &&
        m_state != EnemyState::AttackUp &&
        m_state != EnemyState::AttackDown)
        return false;

    return m_currentFrame == 4 || m_currentFrame == 5;
}

sf::FloatRect Enemy::getHitbox() const
{
    sf::Vector2f pos = m_sprite.getPosition();
    return { pos.x - 30.f, pos.y - 60.f, 60.f, 80.f };
}

// Comprueba si ya hizo daño en este golpe
void Enemy::markDamageDone()
{
    m_hasDealtDamage = true;
}

sf::FloatRect Enemy::getAttackHitbox() const
{
    if (!isAttackFrame() || m_hasDealtDamage)
        return sf::FloatRect(0, 0, 0, 0);

    sf::Vector2f pos = m_sprite.getPosition();

    if (m_state == EnemyState::Attack)
    {
        if (m_facing == EnemyFacing::Right)
            return { pos.x + 20.f, pos.y - 40.f, 60.f, 30.f };
        else
            return { pos.x - 80.f, pos.y - 40.f, 60.f, 30.f };
    }
    else if (m_state == EnemyState::AttackUp)
    {
        return { pos.x - 25.f, pos.y - 100.f, 50.f, 40.f };
    }
    else if (m_state == EnemyState::AttackDown)
    {
        return { pos.x - 25.f, pos.y + 10.f, 50.f, 40.f };
    }

    return {};
}

// Para que el enemigo reciba daño
void Enemy::takeDamage(float dmg)
{
    if (!m_active || m_state == EnemyState::Dead)
        return;

    m_health -= dmg;
    if (m_health <= 0.f)
        setState(EnemyState::Dead);
}

bool Enemy::isAttacking() const
{
    return m_state == EnemyState::Attack ||
           m_state == EnemyState::AttackUp ||
           m_state == EnemyState::AttackDown;
}

// Para que el enemigo no salga de la pantalla
void Enemy::clampToScreen()
{
    sf::FloatRect b = m_sprite.getGlobalBounds();
    sf::Vector2f pos = m_sprite.getPosition();

    if (b.left < 0.f)
        pos.x -= b.left;

    if (b.top < 0.f)
        pos.y -= b.top;

    if (b.left + b.width > 1920.f)
        pos.x -= (b.left + b.width - 1920.f);

    if (b.top + b.height > 1080.f)
        pos.y -= (b.top + b.height - 1080.f);

    m_sprite.setPosition(pos);
    m_position = pos;
}

void Enemy::update(float deltaTime, const sf::Vector2f& playerPos)
{
    if (!m_active)
        return;

    if (m_state == EnemyState::Dead)
    {
        updateAnimation(deltaTime);
        return;
    }

    // Movimiento y ataque del enemigo
    sf::Vector2f dir = playerPos - m_position;
    float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);

    if (!isAttacking() && dist < 100.f)
    {
        if (std::abs(dir.x) < 50.f)
        {
            if (dir.y < 0)
                setState(EnemyState::AttackUp);
            else
                setState(EnemyState::AttackDown);
        }
        else
        {
            setState(EnemyState::Attack);
        }

        updateAnimation(deltaTime);
        return;
    }

    if (isAttacking())
    {
        updateAnimation(deltaTime);
        return;
    }

    if (dist > 1.f)
    {
        dir /= dist;

        if (dir.x < 0)
            m_facing = EnemyFacing::Left;
        else
            m_facing = EnemyFacing::Right;

        m_position += dir * m_speed * deltaTime;
        setState(EnemyState::Run);
    }

    m_sprite.setPosition(m_position);
    clampToScreen();
    updateAnimation(deltaTime);

    float sx = (m_facing == EnemyFacing::Left) ? -1.f : 1.f;
    m_sprite.setScale(sx, 1.f);
}

// Estados del enemigo
void Enemy::setState(EnemyState state)
{
    if (m_state == state)
        return;

    m_state = state;
    m_currentFrame = 0;
    m_elapsedTime = 0.f;

    switch (m_state)
    {
    case EnemyState::Idle:
        m_sprite.setTexture(m_idleTexture);
        m_totalFrames = 7;
        m_frameWidth = 192;
        m_frameHeight = 192;
        break;
    case EnemyState::Run:
        m_sprite.setTexture(m_runTexture);
        m_totalFrames = 6;
        m_frameWidth = 192;
        m_frameHeight = 192;
        break;
    case EnemyState::Attack:
        m_sprite.setTexture(m_attackTexture);
        m_hasDealtDamage = false;
        m_totalFrames = 6;
        m_frameWidth = 192;
        m_frameHeight = 192;
        break;
    case EnemyState::AttackUp:
        m_sprite.setTexture(m_attackUpTexture);
        m_hasDealtDamage = false;
        m_totalFrames = 6;
        m_frameWidth = 192;
        m_frameHeight = 192;
        break;
    case EnemyState::AttackDown:
        m_sprite.setTexture(m_attackDownTexture);
        m_hasDealtDamage = false;
        m_totalFrames = 6;
        m_frameWidth = 192;
        m_frameHeight = 192;
        break;
    case EnemyState::Dead:
        m_sprite.setTexture(m_deadTexture);
        m_totalFrames = 14;
        m_frameWidth = 128;
        m_frameHeight = 128;
        break;
    }

    m_sprite.setTextureRect({ 0, 0, m_frameWidth, m_frameHeight });
}

// Actualizacion de las animaciones
void Enemy::updateAnimation(float deltaTime)
{
    m_elapsedTime += deltaTime;
    if (m_elapsedTime < m_frameTime)
        return;

    m_elapsedTime = 0.f;
    m_currentFrame++;

    if ((m_state == EnemyState::Attack ||
        m_state == EnemyState::AttackUp ||
        m_state == EnemyState::AttackDown) && m_currentFrame >= m_totalFrames)
        setState(EnemyState::Idle);

    if (m_state == EnemyState::Dead && m_currentFrame >= m_totalFrames)
    {
        m_active = false;
        return;
    }

    m_currentFrame %= m_totalFrames;
    m_sprite.setTextureRect({m_currentFrame * m_frameWidth, 0, m_frameWidth, m_frameHeight});
}

// Imprime al enemigo
void Enemy::render(sf::RenderWindow& window)
{
    if (m_active)
        window.draw(m_sprite);
}