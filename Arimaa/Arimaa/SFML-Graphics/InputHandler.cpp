#include "InputHandler.h"

using namespace sf;
using namespace std;

InputHandler::InputHandler(void)
{
}

InputHandler::~InputHandler(void)
{
}

void InputHandler::map(std::string control, sf::Key::Code key)
{
	vector<Key::Code> keys;
	keys.push_back(key);
	map(control, keys);
}

void InputHandler::map(std::string control, std::vector<sf::Key::Code> keys)
{
	if(boundKeys.find(control) != boundKeys.end()) //if the control is found in the map
	{
		std::vector<Key::Code>* mappedKeys = &boundKeys[control];
		for(Key::Code c : keys)
		{
			mappedKeys->push_back(c);
		}
	}
	else
	{
		boundKeys[control]=keys;
	}
}

void InputHandler::map(std::string control, sf::Mouse::Button button)
{
	vector<Mouse::Button> buttons;
	buttons.push_back(button);
	map(control, buttons);
}

void InputHandler::map(std::string control, std::vector<sf::Mouse::Button> buttons)
{
	if(boundMouseButtons.find(control) != boundMouseButtons.end()) //if the control is found in the map
	{
		std::vector<Mouse::Button>* mappedMouseButtons = &boundMouseButtons[control];
		for(Mouse::Button b : buttons)
		{
			mappedMouseButtons->push_back(b);
		}
	}
	else
	{
		boundMouseButtons[control]=buttons;
	}
}

void InputHandler::unMap(std::string control)
{
	std::map<std::string, std::vector<sf::Key::Code>>::iterator it1 = boundKeys.find(control);
	if(it1 != boundKeys.end()) //if the control is found in the map
	{
		boundKeys.erase(it1);
	}

	std::map<std::string, std::vector<sf::Mouse::Button>>::iterator it2 = boundMouseButtons.find(control);
	if(it2 != boundMouseButtons.end()) //if the control is found in the map
	{
		boundMouseButtons.erase(it2);
	}
}

bool InputHandler::testEvent(sf::Event e, std::string control)
{
	if(e.Type == Event::KeyPressed)
	{
		std::map<std::string, std::vector<sf::Key::Code>>::iterator it = boundKeys.find(control);
		if(it != boundKeys.end()) //if the control is found in the map
		{
			if( find(it->second.begin(), it->second.end(), e.Key.Code) != it->second.end())
				return true;
		}
	}
	else if (e.Type == Event::MouseButtonPressed)
	{
		std::map<std::string, std::vector<sf::Mouse::Button>>::iterator it = boundMouseButtons.find(control);
		if(it != boundMouseButtons.end()) //if the control is found in the map
		{
			if( find(it->second.begin(), it->second.end(), e.MouseButton.Button) != it->second.end())
				return true;
		}
	}

	return false; //in all other cases
}

bool InputHandler::isHeldDown(const sf::Input& input, std::string control)
{
	//b1 : a corresponding key has been pressed
	//b2 : a corresponding mouse button has been pressed
	bool b1 = false, b2 = false;
	unsigned int i;

	std::map<std::string, std::vector<sf::Key::Code>>::iterator it1 = boundKeys.find(control);
	if(it1 != boundKeys.end()) //if the control is found in the map
	{
		i = 0;
		while(i < it1->second.size() && !b1)
		{
			b1 = input.IsKeyDown(it1->second.at(i));
			i++;
		}
	}

	std::map<std::string, std::vector<sf::Mouse::Button>>::iterator it2 = boundMouseButtons.find(control);
	if(it2 != boundMouseButtons.end()) //if the control is found in the map
	{
		i = 0;
		while(i < it2->second.size() && !b2)
		{
			b2 = input.IsMouseButtonDown(it2->second.at(i));
			i++;
		}
	}
	return (b1 || b2);
}
