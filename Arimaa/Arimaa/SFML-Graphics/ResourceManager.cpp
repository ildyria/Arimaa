#include "../StdAfx.h"
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
	if(images.find(imageFile) == images.end()) //the image is not already loaded
	{
		if(!images[imageFile].LoadFromFile(imageFile)) //the image loads (or tries to)
		{
			images.erase(imageFile); //the image was created by [] so we delete it;
			return NULL; //this is the case where the image couldn't be successfully loaded
		}
	}
	return &images[imageFile];
}

sf::Image* ResourceManager::getImage(std::string imageFile, sf::Color transparentColor)
{
	Image* i = getImage(imageFile);
	if(i != NULL)
		i->CreateMaskFromColor(transparentColor);
	return i;
}

sf::Font* ResourceManager::getFont(std::string fontFile)
{

	if(fonts.find(fontFile) == fonts.end()) //the font is not already loaded
	{
		if(!fonts[fontFile].LoadFromFile(fontFile)) //the image loads (or tries to)
		{
			fonts.erase(fontFile); //the image was created by [] so we delete it;
			return NULL; //this is the case where the image couldn't be successfully loaded
		}
	}
	return &fonts[fontFile];
}