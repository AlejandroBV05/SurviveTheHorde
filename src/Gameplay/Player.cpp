#include "./Gameplay/Player.h"
#include "./Core/ConfigLoader.h"
#include <SFML/Window/Keyboard.hpp>

// Tamaño de cada frame
constexpr int FRAME_WIDTH = 192;
constexpr int FRAME_HEIGHT = 192;

// Config del player
Player::Player() : m_position(960.f, 540.f), m_speed(200.f), m_currentFrame(0), m_totalFrames(1), m_frameTime(0.12f), m_elapsedTime(0.f), m_state(PlayerState::Idle), m_facing(Facing::Right), m_attackHeld(false), m_health(500.f), m_maxHealth(500.f)
{
    // Carga de valores tipo Data-drivem
    auto config = ConfigLoader::load("../data/config/player.txt");

    m_maxHealth = config["max_health"];
    m_health = m_maxHealth;
    m_speed = config["speed"];
    m_attack = config["attack"];

    //Texturas del player y ajuste de sprites
    m_idleTexture.loadFromFile("../data/textures/player/Warrior_Idle.png");
    m_runTexture.loadFromFile("../data/textures/player/Warrior_Run.png");
    m_attackTexture.loadFromFile("../data/textures/player/Warrior_Attack.png");
    m_attackUpTexture.loadFromFile("../data/textures/player/Warrior_AttackUp.png");
    m_attackDownTexture.loadFromFile("../data/textures/player/Warrior_AttackDown.png");
    m_deadTexture.loadFromFile("../data/textures/player/Warrior_Dead.png");

    m_sprite.setTexture(m_idleTexture);
    m_sprite.setTextureRect({ 0, 0, FRAME_WIDTH, FRAME_HEIGHT });
    
    m_frameWidth = FRAME_WIDTH;
    m_frameHeight = FRAME_HEIGHT;

    m_sprite.setOrigin(FRAME_WIDTH / 2.f, FRAME_HEIGHT * 0.85f);
    m_sprite.setPosition(m_position);
    m_sprite.setScale(1.f, 1.f);
}

// Para que ek player reciba daño
void Player::takeDamage(float dmg)
{
    if (m_state == PlayerState::Dead)
        return;

    m_health -= dmg;
}

// Comprobar si en este frame puede hacer daño
bool Player::isAttackFrame() const
{
    if (m_state != PlayerState::Attack &&
        m_state != PlayerState::AttackUp &&
        m_state != PlayerState::AttackDown)
        return false;

    return m_currentFrame == 2 || m_currentFrame == 3;
}

sf::FloatRect Player::getHitbox() const
{
    sf::Vector2f pos = m_sprite.getPosition();
    return { pos.x - 30.f, pos.y - 60.f, 60.f, 80.f };
}

// Marca para saber si ya hizo daño con el ataque
void Player::markDamageDone()
{
    m_hasDealtDamage = true;
}

sf::FloatRect Player::getAttackHitbox() const
{
    if (!isAttackFrame() || m_hasDealtDamage)
        return sf::FloatRect(0, 0, 0, 0);

    sf::Vector2f pos = m_sprite.getPosition();

    if (m_state == PlayerState::Attack)
    {
        if (m_facing == Facing::Right)
            return { pos.x + 30.f, pos.y - 50.f, 70.f, 40.f };
        else
            return { pos.x - 90.f, pos.y - 50.f, 70.f, 40.f };
    }
    else if (m_state == PlayerState::AttackUp)
    {
        return { pos.x - 25.f, pos.y - 100.f, 50.f, 40.f };
    }
    else if (m_state == PlayerState::AttackDown)
    {
        return { pos.x - 25.f, pos.y + 10.f, 50.f, 40.f };
    }

    return {};
}

// Eviat que el player salga de la pantalla
void Player::clampToScreen()
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

float Player::getHealth() const
{
    return m_health;
}

float Player::getMaxHealth() const
{
    return m_maxHealth;
}

//Comprueba si la animación de muerte termino
bool Player::isDeadAnimationFinished() const
{
    return m_deadFinished;
}

sf::Vector2f Player::getPosition() const
{
    return m_position;
}

bool Player::isAttacking() const
{
    return m_state == PlayerState::Attack ||
        m_state == PlayerState::AttackUp ||
        m_state == PlayerState::AttackDown;
}

void Player::update(float deltaTime)
{
    sf::Vector2f direction(0.f, 0.f);

    if (m_health <= 0.f && m_state != PlayerState::Dead)
    {
        setState(PlayerState::Dead);
        return;
    }

    // Movimiento del player
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        direction.x -= 1.f;
        m_facing = Facing::Left;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        direction.x += 1.f;
        m_facing = Facing::Right;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        direction.y -= 1.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        direction.y += 1.f;

    // Ataque
    bool attackPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);

    if (attackPressed && !m_attackHeld && !isAttacking() && m_state != PlayerState::Dead)
    {
        if (direction.y < 0)
            setState(PlayerState::AttackUp);
        else if (direction.y > 0)
            setState(PlayerState::AttackDown);
        else
            setState(PlayerState::Attack);
    }

    m_attackHeld = attackPressed;

    if (!isAttacking() && m_state != PlayerState::Dead)
    {
        bool moving = (direction.x != 0.f || direction.y != 0.f);
        setState(moving ? PlayerState::Run : PlayerState::Idle);

        m_position += direction * m_speed * deltaTime;
    }

    updateAnimation(deltaTime);

    m_sprite.setPosition(m_position);
    clampToScreen();

    float scaleX = (m_facing == Facing::Left) ? -1.f : 1.f;
    m_sprite.setScale(scaleX, 1.f);
}

// Estados del player
void Player::setState(PlayerState newState)
{
    if (m_state == newState)
        return;

    m_state = newState;
    m_currentFrame = 0;
    m_elapsedTime = 0.f;

    switch (m_state)
    {
    case PlayerState::Idle:
        m_sprite.setTexture(m_idleTexture);
        m_totalFrames = 4;
        m_frameWidth = 192;
        m_frameHeight = 192;
        break;

    case PlayerState::Run:
        m_sprite.setTexture(m_runTexture);
        m_totalFrames = 6;
        m_frameWidth = 192;
        m_frameHeight = 192;
        break;

    case PlayerState::Attack:
        m_sprite.setTexture(m_attackTexture);
        m_hasDealtDamage = false;
        m_totalFrames = 4;
        m_frameWidth = 192;
        m_frameHeight = 192;
        break;

    case PlayerState::AttackUp:
        m_sprite.setTexture(m_attackUpTexture);
        m_hasDealtDamage = false;
        m_totalFrames = 6;        
        m_frameWidth = 192;
        m_frameHeight = 192;
        break;

    case PlayerState::AttackDown:
        m_sprite.setTexture(m_attackDownTexture);
        m_hasDealtDamage = false;
        m_totalFrames = 6;
        m_frameWidth = 192;
        m_frameHeight = 192;
        break;

    case PlayerState::Dead:
        m_sprite.setTexture(m_deadTexture);
        m_totalFrames = 14;
        m_frameWidth = 128;
        m_frameHeight = 128;
        m_deadFinished = false;
        break;
    }

    m_sprite.setTextureRect({0, 0, m_frameWidth, m_frameHeight });
}

// Update de las animaciones
void Player::updateAnimation(float deltaTime)
{
    m_elapsedTime += deltaTime;

    if (m_elapsedTime < m_frameTime)
        return;

    m_elapsedTime = 0.f;

    m_currentFrame++;

    if (m_state == PlayerState::Dead)
    {
        if (m_currentFrame >= m_totalFrames - 1)
        {
            m_deadFinished = true;
            return;
        }

        m_currentFrame++;

        int col = m_currentFrame % 7;
        int row = m_currentFrame / 7;

        m_sprite.setTextureRect({col * m_frameWidth, row * m_frameHeight, m_frameWidth, m_frameHeight });

        return;
    }
    else if (m_state == PlayerState::Attack ||
        m_state == PlayerState::AttackUp ||
        m_state == PlayerState::AttackDown)
    {
        if (m_currentFrame >= m_totalFrames)
        {
            setState(PlayerState::Idle);
            return;
        }
    }
    else
    {
        m_currentFrame %= m_totalFrames;
    }

    m_sprite.setTextureRect({m_currentFrame * m_frameWidth, 0, m_frameWidth, m_frameHeight });
}

// Reset del player
void Player::reset()
{
    m_health = m_maxHealth;
    m_position = { 960.f, 540.f };
    m_facing = Facing::Right;
    setState(PlayerState::Idle);
}

// Imprime al player
void Player::render(sf::RenderWindow& window)
{
    window.draw(m_sprite);
}