// Arimaa.cpp : définit le point d'entrée pour l'application console.
//

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdio>
#include "ConfigOptions.h"
#include "InputHandler.h"
#include "Screen.h"
#include "Connect4GameScreen.h"

using namespace sf;

//comment this line to hide console (now by default in debug mode)
//#define CONSOLE_ON


int main()
{
#ifdef CONSOLE_ON
	void InitializeConsoleStdIO();
#else
#ifndef LINUX
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_HIDE);
#endif
#endif

	ConfigOptions::init();
	unsigned long windowStyle = Style::Close | Style::Titlebar;
	if(ConfigOptions::inFullscreen())
		windowStyle |= Style::Fullscreen;
	RenderWindow app(VideoMode(ConfigOptions::getResolution().x, ConfigOptions::getResolution().y, 32), "GameAI", windowStyle); //Création de la fenêtre app
	//app.SetIcon(32, 32, ResourceManager::getImage("Icon.png")->GetPixelsPtr());
	app.SetFramerateLimit(60);

	sf::String loadingText("Loading...", *ResourceManager::getFont(), 48);
	loadingText.SetCenter(loadingText.GetRect().GetWidth()/2, loadingText.GetRect().GetHeight()/2);
	loadingText.SetPosition(ConfigOptions::getScreenCenter());

	std::vector<Screen*> screens;

	//Screens creation
	Connect4GameScreen gameScreen(screens.size());
	screens.push_back(&gameScreen);

	int screen = 0;
	int oldScreen = 0;

	app.Clear();
	app.Draw(loadingText);
	app.Display();

	screens.at(screen)->initialize();
	while (screen >= 0)
	{
		screen = screens.at(screen)->run(app);
		if(oldScreen != screen && screen != EXIT)
		{
			app.Clear();
			app.Draw(loadingText);
			app.Display();
			screens.at(oldScreen)->uninitialize();
			screens.at(screen)->initialize();
		}
		oldScreen = screen;
	}
	return EXIT_SUCCESS;
}



#ifdef CONSOLE_ON
void InitializeConsoleStdIO()
{
    // si une console est rattachée au processus, alors il existe des fichiers
    // virtuel CONIN$ et CONOUT$ qui permettent respectivement de lire
    // et d'écrire depuis / dans cette console (voir la doc de CreateFile).

#if _MSC_VER >= 1400 // VC++ 8
    {
    // éviter le warning C4996: 'freopen' was declared deprecated
    // This function or variable may be unsafe. Consider using freopen_s instead.
    FILE *stream;
    freopen_s( &stream, "CONIN$", "r", stdin );
    freopen_s( &stream, "CONOUT$", "w", stdout );
    freopen_s( &stream, "CONOUT$", "w", stderr );
    }
#else
    std::freopen( "CONIN$", "r", stdin );
    std::freopen( "CONOUT$", "w", stdout );
    std::freopen( "CONOUT$", "w", stderr );
#endif

    // la ligne suivante synchronise les flux standards C++ (cin, cout, cerr...)
    std::ios_base::sync_with_stdio();
}

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow
)
{
    // créer une console
    ::AllocConsole();
    // synchroniser la CRT
    InitializeConsoleStdIO();
    // tester
    std::cout << "Hello World!\n";
    std::cout << "Appuyez sur Entree pour quitter...";
    std::cin.ignore();
}
#endif
