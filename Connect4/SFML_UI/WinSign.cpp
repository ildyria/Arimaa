#include "WinSign.h"
#include "ConfigOptions.h"
#include "ResourceManager.h"

#define NB_SIGNS 3
#define WS_START_POS ConfigOptions::getNativeCenter()
#define WS_END_POS sf::Vector2f((float)ConfigOptions::getNativeCenter().x, (float)m_sprite.GetImage()->GetHeight() / (2*NB_SIGNS))


WinSign::WinSign() : m_timer(0)
{
}


WinSign::~WinSign()
{
}

void WinSign::loadAssets()
{
	m_sprite.SetImage(*ResourceManager::getImage("Victory_Sign.png"));

	m_sprite.SetPosition(WS_START_POS);
	m_sprite.SetCenter(m_sprite.GetSize().x / 2, m_sprite.GetSize().y / (2 * NB_SIGNS));
}

void WinSign::unloadAssets()
{
	ResourceManager::unloadImage("Victory_Sign.png");
}

void WinSign::draw(sf::RenderWindow &app)
{
	if (m_on)
	{
		app.Draw(m_sprite);
	}
}

void WinSign::update(float elapsedTime)
{
	if (m_on)
	{
		m_timer += elapsedTime;
		if (m_timer > 1) //finished appearing
		{
			setOpacity(255);

			if (m_timer > 2)
				m_sprite.SetPosition(WS_END_POS);
			else
				m_sprite.SetPosition( (m_timer - 1)*WS_END_POS + (2 - m_timer)*WS_START_POS );
		}
		else
		{
			setOpacity((int)(255 * m_timer));
		}
	}
}

void WinSign::activate(int player)
{
	if (!m_on)
	{
		m_on = true;

		int width = (int) m_sprite.GetSize().x;
		int height = (int)m_sprite.GetSize().y / NB_SIGNS;
		int x = 0;
		int y = player * height;
		m_sprite.SetSubRect(sf::IntRect(x, y, x + width, y + height));
	}
}

void WinSign::unactivate()
{
	if (m_on)
	{
		m_on = false;
		m_timer = 0;
	}
}