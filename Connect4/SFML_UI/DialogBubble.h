#pragma once
#include <SFML\Graphics.hpp>

class DialogBubble
{
public:
	DialogBubble();
	~DialogBubble();

	void loadAssets();
	void unloadAssets();

	void draw(sf::RenderWindow &app);
	void update(float elapsedTime);

	void toggle();
	void untoggle();

	void setPosition(sf::Vector2f pos);
	void setText(std::string t = "");

private:
	sf::Sprite m_sprite;
	sf::String m_text;
	bool m_on;
	float m_timer;
	sf::Vector2f m_origPos;

	void setOpacity(int op);
};

