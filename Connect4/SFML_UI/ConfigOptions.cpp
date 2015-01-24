#include "ConfigOptions.h"


sf::Vector2i ConfigOptions::m_resolution;
sf::View ConfigOptions::m_view;
bool ConfigOptions::m_inFullscreen;
InputHandler ConfigOptions::m_iHandler;
std::string ConfigOptions::m_theme = "";

void ConfigOptions::init()
{
	//Opening ini file
	CSimpleIniA ini;
	ini.SetUnicode();
	ini.SetMultiKey();
	if (ini.LoadFile("Config.ini") == SI_OK)
	{
		/////////////////////GRAPHICS///////////////////////////////
		//Resoltion
		m_resolution = sf::Vector2i(atoi(ini.GetValue("Graphics", "ScreenWidth", "1920")), atoi(ini.GetValue("Graphics", "ScreenHeight", "1080")));
		m_inFullscreen = ini.GetBoolValue("Graphics", "FullScreen", 0);
		m_view.SetFromRect(sf::FloatRect(0, 0, (float) nativeWidth(), (float) nativeHeight()));

		//////////////////////CONTROLS/////////////////////////////
		std::string item;
		std::string val;

		//Keys
		std::map<std::string, sf::Key::Code> allKeys = getNamedKeys();

		// if there are keys and values...
		const CSimpleIniA::TKeyVal* keys = ini.GetSection("KeyboardControls");
		if (keys)
		{
			// iterate over all keys and dump the key name and value
			for (CSimpleIniA::TKeyVal::const_iterator it = keys->begin() ; it != keys->end() ; ++it)
			{
				item = it->first.pItem;
				val = it->second;
				m_iHandler.map(item, allKeys[val]);
			}
		}

		//Mouse buttons
		std::map<std::string, sf::Mouse::Button> allButtons = getNamedButtons();

		// if there are keys and values...
		const CSimpleIniA::TKeyVal* buttons = ini.GetSection("MouseControls");
		if (keys)
		{
			// iterate over all keys and dump the key name and value
			for (CSimpleIniA::TKeyVal::const_iterator it = buttons->begin() ; it != buttons->end() ; ++it)
			{
				item = it->first.pItem;
				val = it->second;
				m_iHandler.map(item, allButtons[val]);
			}
		}

		//////////////////////OTHER/////////////////////////////

		m_theme = ini.GetValue("Graphics", "Theme", "");
		if (m_theme == "Default") //default theme
			m_theme = "";
	}
	else
	{
		std::cerr << "Unable to load file : Config.ini" << std::endl;
	}
}

std::map<std::string, sf::Key::Code> ConfigOptions::getNamedKeys()
{
	std::map<std::string, sf::Key::Code> res;

	#define INSERT(identifier) res[#identifier]=sf::Key::identifier;

	INSERT(A);
	INSERT(B);
	INSERT(C);
	INSERT(D);
	INSERT(E);
	INSERT(F);
	INSERT(G);
	INSERT(H);
	INSERT(I);
	INSERT(J);
	INSERT(K);
	INSERT(L);
	INSERT(M);
	INSERT(N);
	INSERT(O);
	INSERT(P);
	INSERT(Q);
	INSERT(R);
	INSERT(S);
	INSERT(T);
	INSERT(U);
	INSERT(V);
	INSERT(W);
	INSERT(X);
	INSERT(Y);
	INSERT(Z);
	INSERT(Num0);
	INSERT(Num1);
	INSERT(Num2);
	INSERT(Num3);
	INSERT(Num4);
	INSERT(Num5);
	INSERT(Num6);
	INSERT(Num7);
	INSERT(Num8);
	INSERT(Num9);
	INSERT(Escape);
	INSERT(LControl);
	INSERT(LShift);
	INSERT(LAlt);
	INSERT(LSystem);
	INSERT(RControl);
	INSERT(RShift);
	INSERT(RAlt);
	INSERT(RSystem);
	INSERT(Menu);
	INSERT(LBracket);
	INSERT(RBracket);
	INSERT(SemiColon);
	INSERT(Comma);
	INSERT(Period);
	INSERT(Quote);
	INSERT(Slash);
	INSERT(BackSlash);
	INSERT(Tilde);
	INSERT(Equal);
	INSERT(Dash);
	INSERT(Space);
	INSERT(Return);
	INSERT(Back);
	INSERT(Tab);
	INSERT(PageUp);
	INSERT(PageDown);
	INSERT(End);
	INSERT(Home);
	INSERT(Insert);
	INSERT(Delete);
	INSERT(Add);
	INSERT(Subtract);
	INSERT(Multiply);
	INSERT(Divide);
	INSERT(Left);
	INSERT(Right);
	INSERT(Up);
	INSERT(Down);
	INSERT(Numpad0);
	INSERT(Numpad1);
	INSERT(Numpad2);
	INSERT(Numpad3);
	INSERT(Numpad4);
	INSERT(Numpad5);
	INSERT(Numpad6);
	INSERT(Numpad7);
	INSERT(Numpad8);
	INSERT(Numpad9);
	INSERT(F1);
	INSERT(F2);
	INSERT(F3);
	INSERT(F4);
	INSERT(F5);
	INSERT(F6);
	INSERT(F7);
	INSERT(F8);
	INSERT(F9);
	INSERT(F10);
	INSERT(F11);
	INSERT(F12);
	INSERT(F13);
	INSERT(F14);
	INSERT(F15);
	INSERT(Pause);

	#undef INSERT

	return res;
}

std::map<std::string, sf::Mouse::Button> ConfigOptions::getNamedButtons()
{
	std::map<std::string, sf::Mouse::Button> res;

	#define INSERT(identifier) res[#identifier]=sf::Mouse::identifier;

	INSERT(Left);
	INSERT(Right);
	INSERT(Middle);
	INSERT(XButton1);
	INSERT(XButton2);

	#undef INSERT

	return res;
}
