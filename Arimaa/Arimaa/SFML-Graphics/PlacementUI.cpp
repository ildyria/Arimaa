#include "PlacementUI.h"

PlacementUI::PlacementUI(void) : m_selectedPiece(RABBIT)
{
	float ypos = (float) ConfigOptions::screenHeight()/2;
	float centerPiece = (float) NB_PIECES/2;
	for(int i = 0; i < NB_PIECES; ++i)
	{
		m_piecesSprites[i].SetPosition((float) PUI_X_POS, ypos + (i-centerPiece)*PIECES_GAP);
	}
	m_endTurnButton.SetPosition((float) PUI_X_POS, ypos + (NB_PIECES-centerPiece)*PIECES_GAP);
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
	int pieceType = m_selectedPiece - 1;
	if(pieceType < 0)
		pieceType += NB_PIECES;
	select((PieceType) pieceType);
	return m_selectedPiece;
}

PieceType PlacementUI::goDown()
{
	int pieceType = m_selectedPiece + 1;
	if(pieceType >= NB_PIECES)
		pieceType -= NB_PIECES;
	select((PieceType) pieceType);
	return m_selectedPiece;
}

void PlacementUI::select(PieceType t)
{
	m_piecesSprites[m_selectedPiece].SetColor(sf::Color::White);
	m_selectedPiece = t;
	m_piecesSprites[m_selectedPiece].SetColor(sf::Color::Yellow);
}

void PlacementUI::setColor(Color c)
{
	int x;
	int y = c * SQUARE_SIZE;
	for(int i = 0; i < NB_PIECES; ++i)
	{
		x = m_piecesSprites[i].GetSubRect().Left;
		m_piecesSprites[i].SetSubRect(sf::IntRect(x , y, x + SQUARE_SIZE, y + SQUARE_SIZE ));
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

void PlacementUI::draw(sf::RenderWindow& app)
{
	for(int i = 0; i < NB_PIECES; ++i)
		app.Draw(m_piecesSprites[i]);
	app.Draw(m_endTurnButton);
}