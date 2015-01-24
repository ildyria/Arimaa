#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "SFMLOperations.h"
#include <iostream>
#include <fstream>
#include "SimpleIni.h"
#include "InputHandler.h"

#define NATIVE_WIDTH 1920
#define NATIVE_HEIGHT 1080

class ConfigOptions
{
public:
	static void init();

	static inline sf::Vector2i getResolution() { return m_resolution; }
	static inline sf::Vector2i nativeResolution() { return sf::Vector2i(nativeWidth(), nativeHeight()); }
	static inline sf::View& getView() { return m_view; }
	static inline int screenHeight() { return m_resolution.y; }
	static inline int screenWidth() { return m_resolution.x; }
	static inline int nativeHeight() { return NATIVE_HEIGHT; }
	static inline int nativeWidth() { return NATIVE_WIDTH; }
	static inline void setResolution(const sf::Vector2i res) { m_resolution = res; }
	static inline sf::Vector2f getScreenCenter() { return sfmlop::divide(m_resolution, 2); }
	static inline sf::Vector2f getNativeCenter() { return sfmlop::divide(nativeResolution(), 2); }
	static inline sf::Vector2f topLeftCorner() { return sf::Vector2f(0, 0); }
	static inline sf::Vector2f topRightCorner() { return sf::Vector2f((float)m_resolution.x, 0); }
	static inline sf::Vector2f bottomLeftCorner() { return sf::Vector2f(0, (float)m_resolution.y); }
	static inline sf::Vector2f bottomRightCorner() { return sfmlop::toFloatVect(m_resolution); }
	static inline bool inFullscreen() { return m_inFullscreen; }
	static inline std::string getTheme() { return m_theme; }
	static inline InputHandler* getIHandler() { return &m_iHandler; }

private:
	static sf::Vector2i m_resolution;
	static sf::View m_view;
	static bool m_inFullscreen;
	static InputHandler m_iHandler;
	static std::string m_theme;

	static std::map<std::string, sf::Key::Code> getNamedKeys();
	static std::map<std::string, sf::Mouse::Button> getNamedButtons();
};
