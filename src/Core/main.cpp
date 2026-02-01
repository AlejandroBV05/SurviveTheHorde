#include <SFML/Graphics.hpp>
#include <tmxlite/Map.hpp>
#include "./Core/EnemyPool.h"
#include "./Gameplay/Player.h"
#include "./Gameplay/MainMenu.h"
#include "./Gameplay/GameOverScreen.h"
#include "./Systems/ScoreManager.h"
#include "./Systems/MapLayer.hpp"
#include "./UI/HealthBar.h"
#include <memory>

int main() 
{ 
	// Crea la pantalla del juego
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "Survive The Horde"); 
	window.setFramerateLimit(60);  

	// Como se va a ver el juego
	sf::View gameView; 
	gameView.setSize(1920.f, 1080.f); 
	gameView.setCenter(960.f, 540.f); 

	// Como se va a ver la interfaz
	sf::View uiView; 
	uiView.setSize(1920.f, 1080.f); 
	uiView.setCenter(960.f, 540.f); 

	// Config textos
	sf::Font font;
	font.loadFromFile("../data/fonts/arial.ttf");

	sf::Text scoreText;
	scoreText.setFont(font);
	GameOverScreen gameOver(font);
	scoreText.setCharacterSize(32);
	scoreText.setFillColor(sf::Color::White);
	scoreText.setPosition(20.f, 120.f);

	// Background del menu principal
	tmx::Map menuMap;
	menuMap.load("../data/textures/level/menu.tmx");

	MapLayer menuMapLayerZero(menuMap, 0);
	MapLayer menuMapLayerOne(menuMap, 1);
	MapLayer menuMapLayerTwo(menuMap, 2);
	MapLayer menuMapLayerThree(menuMap, 3);

	// Background del juego
	tmx::Map gameMap;
	gameMap.load("../data/textures/level/level01.tmx");

	MapLayer gameMapLayerZero(gameMap, 0);
	MapLayer gameMapLayerOne(gameMap, 1);

	Player player; 
	Menu menu(font); 
	GameState state = GameState::Menu; 

	// Pool de los enemigos
	EnemyPool enemies(100);
	int wave = 1;
	int enemiesPerWave = 3;   
	bool waveActive = false;

	// Sistema data driven del highscore
	ScoreManager score("../data/config/highscore.txt");

	// Barra dew vida
	HealthBar hp;
	hp.setMaxHealth(player.getMaxHealth());
	hp.setHealth(player.getHealth());
	hp.setPosition({ 20.f, 20.f });

	sf::Clock clock; 

	// Bucle principal del juago
	while (window.isOpen()) 
	{ 
		sf::Event event;
		while (window.pollEvent(event)) 
	    { 
			if (event.type == sf::Event::Closed) window.close(); 
	    } 
		
		float deltaTime = clock.restart().asSeconds(); 

		// Estados del juego
		if (state == GameState::Menu) 
	    { 
			menu.update(window, state, uiView);

			if (state == GameState::Playing)
			{
				score.resetCurrentScore();
			}
	    } 
	    else if (state == GameState::Playing) 
	    {
			// Spawn de oleadas
			if (!waveActive)
			{
				enemies.spawnWave(enemiesPerWave, player.getPosition(),	400.f);
				waveActive = true;
			}

			//Actualizacion del player y enemigos
			player.update(deltaTime);
			enemies.update(deltaTime, player.getPosition());
			hp.setHealth(player.getHealth());

			if (waveActive && enemies.getAliveCount() == 0)
			{
				wave++;
				enemiesPerWave++;
				waveActive = false;
			}

			// Ataques del player
			for (auto& e : enemies.getEnemies())
			{
				if (!e.isActive())
					continue;

				if (player.isAttacking() && player.getAttackHitbox().intersects(e.getHitbox()))
				{
					e.takeDamage(player.m_attack);
					player.markDamageDone();
				}
			}

			// Ataques de los enemigos
			for (auto& e : enemies.getEnemies())
			{
				if (!e.isActive())
					continue;

				if (e.isAttacking() && e.getAttackHitbox().intersects(player.getHitbox()))
				{
					player.takeDamage(e.m_attack);
					e.markDamageDone();
				}
			}

			// Game over / muerte dek player
			if (player.getHealth() <= 0.f)
			{
				score.saveIfHighScore();
				if (player.isDeadAnimationFinished())
				{
					gameOver.setScores(score.getCurrentScore(),	score.getHighScore());
					state = GameState::GameOver;

					player.reset();
					enemies.reset();
					score.resetCurrentScore();
					wave = 1;
					enemiesPerWave = 3;
					waveActive = false;
				}
			}
			
			// Puntos al matar enemigos
			for (auto& e : enemies.getEnemies())
			{
				if (e.isDead() && !e.wasDeathReported())
				{
					score.addPoints(10);
					e.markDeathReported();
				}
			}
	    }
		else if (state == GameState::GameOver)
		{
			gameOver.update(window, state, uiView);
			
			if (state == GameState::Playing)
			{
				score.resetCurrentScore();
			}
		}
	    else if (state == GameState::Exit) 
	    {
			window.close(); 
	    } 

		window.clear(); 

		// Renderizado
		if (state == GameState::Menu) 
	    {
			window.setView(window.getDefaultView());

			window.draw(menuMapLayerZero);
			window.draw(menuMapLayerOne);
			window.draw(menuMapLayerTwo);
			window.draw(menuMapLayerThree);

			window.setView(uiView);
			menu.render(window);
	    } 
	    else if (state == GameState::Playing) 
		{ 
			window.setView(gameView);

			window.draw(gameMapLayerZero);
			window.draw(gameMapLayerOne);

			enemies.render(window);
			player.render(window);
			window.setView(uiView);
			hp.render(window);
			
			scoreText.setString("Score: " + std::to_string(score.getCurrentScore()) + "\nHi-Score: " + std::to_string(score.getHighScore()));
			window.draw(scoreText);
		}
		else if (state == GameState::GameOver)
		{
			window.setView(window.getDefaultView());

			window.draw(menuMapLayerZero);
			window.draw(menuMapLayerOne);
			window.draw(menuMapLayerTwo);
			window.draw(menuMapLayerThree);

			gameOver.render(window);
		}

		window.display(); 
	} 

	return 0; 
}