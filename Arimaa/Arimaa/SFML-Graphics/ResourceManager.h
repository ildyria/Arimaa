#pragma once
#include <SFML/Graphics.hpp>

#define DEFAULT_FONT "Assets/Fonts/LinBiolinum_Rah.ttf"

/// <summary>
/// Gathers all the graphic and audio resource used in the game.
/// Resources can be loaded or unloaded from memory by using the right functions
/// </summary>
class ResourceManager
{
public:
	/// <summary>
	/// Returns a pointer to the image. The image will be loaded if neccessary, or taken from images
	/// </summary>
	static sf::Image* getImage(std::string imageFile);
	static sf::Image* getImage(std::string imageFile, sf::Color transparentColor);
	static inline void unloadImage(std::string imageFile) { images.erase(imageFile); }

	static sf::Font* getFont(std::string fontFile = DEFAULT_FONT);
	static inline void unloadFont(std::string fontFile) { fonts.erase(fontFile); }

	static const sf::Color DEFAULT_TRANSPARENT_COLOR; 

private:
	static std::map<std::string, sf::Image> images;
	static std::map<std::string, sf::Font> fonts;
};
