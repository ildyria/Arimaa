#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <string>


class InputHandler
{
public:
	InputHandler(void);
	~InputHandler(void);

	/// <summary>
	/// Maps a (many) key(s) / mouse button(s) to a control (identified by a string)
    /// </summary>
	void map(std::string control, sf::Key::Code key);
	void map(std::string control, std::vector<sf::Key::Code> keys);
	void map(std::string control, sf::Mouse::Button button);
	void map(std::string control, std::vector<sf::Mouse::Button> buttons);

	/// <summary>
	/// Removes the keys and mouse buttons mapped to the control
    /// </summary>
	void unMap(std::string control);

	/// <summary>
	/// Returns true if the event is associated with the control
    /// </summary>
	bool testEvent(sf::Event e, std::string control);

	/// <summary>
	/// Returns true if a key or mouse button associated with the control is being held down
    /// </summary>
	bool isHeldDown(const sf::Input& input, std::string control);

private:
	/// <summary>
	/// The list of keys bound to each control
    /// </summary>
	std::map<std::string, std::vector<sf::Key::Code>> boundKeys;

	/// <summary>
	/// The list of mouse buttons bound to each control
    /// </summary>
	std::map<std::string, std::vector<sf::Mouse::Button>> boundMouseButtons;

};
