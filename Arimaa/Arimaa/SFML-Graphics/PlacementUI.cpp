#include "PlacementUI.h"

#define GREYED_OUT_COLOR sf::Color::Red

PlacementUI::PlacementUI(void) : m_selectedPiece(RABBIT)
{
	float ypos = (float) ConfigOptions::nativeHeight()/2;
	float centerPiece = (float) NB_PIECES/2;
	for(int i = 0; i < NB_PIECES; ++i)
	{
		m_piecesSprites[i].SetPosition((float) PUI_X_POS, ypos + (i-centerPiece)*PIECES_GAP);
	}
	m_endTurnButton.SetPosition((float) PUI_X_POS, ypos + (NB_PIECES-centerPiece)*PIECES_GAP);
	resetAvailability();
}

PlacementUI::~PlacementUI(void)
{
}

PieceType PlacementUI::click(sf::Vector2f pos)
{
	sf::Vector2f spritePos = m_endTurnButton.GetPosition() - m_endTurnButton.GetCenter();
	sf::Vector2f spriteSize = m_endTurnButton.GetSize();
	sf::FloatRect r(spritePos.x, spritePos.y, spritePos.x + spriteSize.x, spritePos.y + spriteSize.y);
	if(r.Contains(pos.x, pos.y))
	{
		return NB_PIECES; //means end of turn
	}
	//else
	for(int i = 0; i < NB_PIECES; ++i)
	{
		spritePos = m_piecesSprites[i].GetPosition() - m_piecesSprites[i].GetCenter();
		spriteSize = m_piecesSprites[i].GetSize();
		sf::FloatRect r2(spritePos.x, spritePos.y, spritePos.x + spriteSize.x, spritePos.y + spriteSize.y);
		if(r2.Contains(pos.x, pos.y))
		{
			select((PieceType) i);
			break;
		}
	}
	return m_selectedPiece;
}

PieceType PlacementUI::goUp()
{
	if(isOneAvailable())
	{
		int pieceType = m_selectedPiece;
		do
		{
			pieceType -= 1;
			if(pieceType < 0)
				pieceType += NB_PIECES;
		}
		while(!select((PieceType) pieceType) && isOneAvailable()); //tries to select the type. If it fails and some pieces are left, keep going
	}

	return m_selectedPiece;
}

PieceType PlacementUI::goDown()
{
	if(isOneAvailable())
	{
		int pieceType = m_selectedPiece;
		do
		{
			pieceType += 1;
			if(pieceType >= NB_PIECES)
				pieceType -= NB_PIECES;
		}
		while(!select((PieceType) pieceType) && isOneAvailable()); //tries to select the type. If it fails and some pieces are left, keep going
	}
	return m_selectedPiece;
}

bool PlacementUI::select(PieceType t)
{
	if(m_availability[(int) t])
	{
		if(m_availability[(int) m_selectedPiece])
			m_piecesSprites[m_selectedPiece].SetColor(sf::Color::White);
		m_selectedPiece = t;
		m_piecesSprites[m_selectedPiece].SetColor(sf::Color::Yellow);
	}
	return m_availability[(int) t];
}

void PlacementUI::setPlayer(Color c)
{
	int x;
	int y = c * SQUARE_SIZE;
	for(int i = 0; i < NB_PIECES; ++i)
	{
		x = m_piecesSprites[i].GetSubRect().Left;
		m_piecesSprites[i].SetSubRect(sf::IntRect(x , y, x + SQUARE_SIZE, y + SQUARE_SIZE ));
	}
}

PieceType PlacementUI::setAvailability(PieceType type, bool available)
{
	if(m_availability[(int) type] != available) //if there was a change
	{
		m_availability[(int) type] = available;
		if(available)
			m_piecesSprites[(int) type].SetColor(sf::Color::White);
		else
		{
			m_piecesSprites[(int) type].SetColor(GREYED_OUT_COLOR);
			if(m_selectedPiece == type && isOneAvailable()) //if the selected piece ran out and some pieces are left
			{
				goDown();
			}
		}
	}
	return m_selectedPiece;
}

void PlacementUI::resetAvailability()
{
	for(int i = 0; i < NB_PIECES; ++i)
	{
		setAvailability((PieceType) i, true);
	}
}

void PlacementUI::loadAssets()
{
	if(m_piecesSprites[0].GetImage() == NULL)
	{
		sf::Image* im = ResourceManager::getImage("Pieces.png");
		for(int i = 0; i < NB_PIECES; ++i)
		{
			Piece* p = new Piece((PieceType) i);
			m_piecesSprites[i].setPiece(p);
			m_piecesSprites[i].SetImage(*im);
			delete p;
		}
		m_piecesSprites[m_selectedPiece].SetColor(sf::Color::Yellow);
	}
	if(m_endTurnButton.GetImage() == NULL)
	{
		m_endTurnButton.SetImage(*ResourceManager::getImage("End_Turn_Button.png"));
		m_endTurnButton.SetCenter(sfmlop::divide(m_endTurnButton.GetSize(), 2));
	}
}

void PlacementUI::unloadAssets()
{
	ResourceManager::unloadImage("Pieces.png");
	ResourceManager::unloadImage("End_Turn_Button.png");
}

void PlacementUI::draw(sf::RenderWindow& app, bool canEndPlacement)
{
	for(int i = 0; i < NB_PIECES; ++i)
		app.Draw(m_piecesSprites[i]);
	if(canEndPlacement) //if we can end the turn, activate the end turn button
		app.Draw(m_endTurnButton);
}

bool PlacementUI::isOneAvailable()
{
	for(int i = 0; i < NB_PIECES; ++i)
	{
		if(m_availability[i])
			return true;
	}
	return false;
}