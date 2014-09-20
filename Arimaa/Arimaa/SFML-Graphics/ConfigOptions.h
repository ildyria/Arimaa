#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "SFMLOperations.h"
#include "SimpleIni.h"
#include "InputHandler.h"

class ConfigOptions
{
public:
	static void initIHandler();

	static inline sf::Vector2i getResolution() { return m_resolution; }
	static inline int screenHeight() { return m_resolution.y; }
	static inline int screenWidth() { return m_resolution.x; }
	static inline void setResolution(const sf::Vector2i res) { m_resolution = res; }
	static inline sf::Vector2f getScreenCenter() { return sfmlop::divide(m_resolution, 2); }
	static inline sf::Vector2f topLeftCorner() { return sf::Vector2f(0, 0); }
	static inline sf::Vector2f topRightCorner() { return sf::Vector2f((float)m_resolution.x, 0); }
	static inline sf::Vector2f bottomLeftCorner() { return sf::Vector2f(0, (float)m_resolution.y); }
	static inline sf::Vector2f bottomRightCorner() { return sfmlop::toFloatVect(m_resolution); }
	static inline InputHandler* getIHandler() { return &m_iHandler; }

private:
	static sf::Vector2i m_resolution;
	static InputHandler m_iHandler;

	static std::map<std::string, sf::Key::Code> getNamedKeys();
	static std::map<std::string, sf::Mouse::Button> getNamedButtons();
};
