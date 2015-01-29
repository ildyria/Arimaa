#include "DialogBubble.h"
#include "ResourceManager.h"

#define BUBBLE_OFFSET sf::Vector2f(30,15)

DialogBubble::DialogBubble() : m_on(false)
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
	if (m_on)
	{
		app.Draw(m_sprite);
		app.Draw(m_text);
	}
}

void DialogBubble::update(float elapsedTime)
{
}


void DialogBubble::toggle()
{
	m_on = true;
}

void DialogBubble::untoggle()
{
	m_on = false;
}


void DialogBubble::setPosition(sf::Vector2f pos)
{
	m_sprite.SetPosition(pos);
	m_text.SetPosition(pos + BUBBLE_OFFSET);
}

void DialogBubble::setText(std::string t)
{
	m_text.SetText(t);
}