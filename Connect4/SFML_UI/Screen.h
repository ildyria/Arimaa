#pragma once
#include <SFML/Graphics.hpp>

#define EXIT -1

/// <summary>
/// A screen, that can be a menu, the game screen, the inventory screen...
/// </summary>
class Screen
{
public:
	/// <summary>
    /// Constructor for Screen.
	/// You have to call it, as there is no default constructor for Screen.
    /// </summary>
	/// <param name="screenID">The identificator for this Screen (used as a return value by run())</param>
	Screen (int screenID) : myID(screenID) {};

	/// <summary>
    /// The function called every frame while the screen is active
    /// </summary>
	/// <param name="app">The window in which the screen will be displayed</param>
	/// <return>-1(EXIT) to exit, or a positive integer to select which screen to use next frame (myID contains the return value for this Screen)</return>
	/// <example> This is how your main loop should look like in the main function :
	/// <code>
	/// int screen = 0;
	/// int oldScreen = 0;
	///
	/// while (screen >= 0)
	/// {
	/// 	screen = screens.at(screen)->Run(app);
	/// 	if(oldScreen != screen &amp;&amp; screen != EXIT)
	/// 		screens.at(screen)->initialize();
	/// 	oldScreen = screen;
	/// }
	/// </code>
	/// </example>	
	inline int run (sf::RenderWindow &app) { int i = update(app); draw(app); return i; }; 

	virtual int update (sf::RenderWindow &app) = 0; 

	virtual void draw (sf::RenderWindow &app) = 0; 

	/// <summary>
	/// The function called every time the application switches to this screen from a different screeen
	/// An example of usage of this function could be to load assets specific to this screen
    /// </summary>
	virtual void initialize () = 0;

	/// <summary>
	/// The function called every time the application switches from this screen to a different screeen
	/// An example of usage of this function could be to unload assets specific to this screen
    /// </summary>
	virtual void uninitialize () = 0;

protected:
	/// <summary>
	/// The identificator for this Screen (used as a return value by run())
    /// </summary>
	int myID;
};
