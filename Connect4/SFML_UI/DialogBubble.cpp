#include "DialogBubble.h"
#include "ResourceManager.h"

#define BUBBLE_OFFSET sf::Vector2f(30,15)
#define DB_HIDDEN_OFFSET sf::Vector2f(-150, 0)
#define DB_APP_SPD 5

DialogBubble::DialogBubble() : m_on(false), m_timer(0)
{
}


DialogBubble::~DialogBubble()
{
}

void DialogBubble::loadAssets()
{
	m_sprite.SetImage(*ResourceManager::getImage("Dialog.png"));
	m_text.SetFont(*ResourceManager::getFont());
	m_text.SetText("After you.");
	m_text.SetSize(48);
	m_text.SetColor(sf::Color::Black);
}

void DialogBubble::unloadAssets()
{
	ResourceManager::unloadImage("Dialog.png");
}


void DialogBubble::draw(sf::RenderWindow &app)
{
	if (m_timer != 0) //0 is hidden state
	{
		app.Draw(m_sprite);
		app.Draw(m_text);
	}
}

void DialogBubble::update(float elapsedTime)
{
	if (m_on)
	{
		m_timer += elapsedTime*DB_APP_SPD;
		if (m_timer > 1)
			m_timer = 1;
	}
	else
	{
		m_timer -= elapsedTime*DB_APP_SPD;
		if (m_timer < 0)
			m_timer = 0;
	}

	sf::Vector2f currPos = m_origPos + (1 - m_timer)*DB_HIDDEN_OFFSET;
	m_sprite.SetPosition(currPos);
	m_text.SetPosition(currPos + BUBBLE_OFFSET);

	setOpacity((int)(255 * m_timer));
}


void DialogBubble::toggle()
{
	m_on = true;
}

void DialogBubble::untoggle()
{
	if (m_on)
	{
		m_on = false;
		m_text.SetText("After you.");
	}
}


void DialogBubble::setPosition(sf::Vector2f pos)
{
	m_sprite.SetPosition(pos);
	m_text.SetPosition(pos + BUBBLE_OFFSET);
	m_origPos = pos;
}

void DialogBubble::setText(std::string t)
{
	m_text.SetText(t);
}

void DialogBubble::setOpacity(int op)
{
	m_sprite.SetColor(sf::Color(255, 255, 255, op));
	m_text.SetColor(sf::Color(0, 0, 0, op));
}