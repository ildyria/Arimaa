#include "ResourceManager.h"
#include <iostream>
#include <ctime>

using namespace sf;
using namespace std;

std::map<std::string, sf::Image> ResourceManager::images;
std::map<std::string, sf::Font> ResourceManager::fonts;
const sf::Color ResourceManager::DEFAULT_TRANSPARENT_COLOR(255,255,255);

sf::Image* ResourceManager::getImage(std::string imageFile)
{
	//first we try to use the theme
	std::string filepath = IMAGE_DIR + ConfigOptions::getTheme() + "/" + imageFile;
	if (images.find(filepath) == images.end()) //the image is not already loaded
	{
		if (images[filepath].LoadFromFile(filepath)) //the image loads (or tries to)
		{
			return &images[filepath];
		}
		//else
		images.erase(filepath); //the image was created by [] so we remove it;
	}
	else
		return &images[filepath];

	//if the image was not found in theme, use the default one
	filepath = IMAGE_DIR + imageFile;
	if (images.find(filepath) == images.end()) //the image is not already loaded
	{
		if (images[filepath].LoadFromFile(filepath)) //the image loads (or tries to)
		{
			return &images[filepath];
		}
		//else
		images.erase(filepath); //the image was created by [] so we remove it;
	}
	else
		return &images[filepath];

	return nullptr; //this is the case where the image couldn't be successfully loaded
}

sf::Image* ResourceManager::getImage(std::string imageFile, sf::Color transparentColor)
{
	Image* i = getImage(imageFile);
	if (i != nullptr)
		i->CreateMaskFromColor(transparentColor);
	return i;
}

sf::Font* ResourceManager::getFont(std::string fontFile)
{
	fontFile = FONT_DIR + fontFile;
	if(fonts.find(fontFile) == fonts.end()) //the font is not already loaded
	{
		if(!fonts[fontFile].LoadFromFile(fontFile)) //the image loads (or tries to)
		{
			fonts.erase(fontFile); //the image was created by [] so we delete it;
			return nullptr; //this is the case where the image couldn't be successfully loaded
		}
	}
	return &fonts[fontFile];
}
