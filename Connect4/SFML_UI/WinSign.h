#pragma once
#include <SFML\Graphics.hpp>

class WinSign
{
public:
	WinSign();
	~WinSign();

	void loadAssets();
	void unloadAssets();

	void draw(sf::RenderWindow &app);
	void update(float elapsedTime);

	void activate(int player);
	void unactivate();

private:
	sf::Sprite m_sprite;
	float m_timer;
	bool m_on;

	inline void setOpacity(int opacity) { m_sprite.SetColor(sf::Color(255, 255, 255, opacity)); }
};

