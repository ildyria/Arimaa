#include "Game.h"
#include "../connect4/BitboardConnect4.h"
#include <algorithm>

using std::max;
using std::min;

namespace api
{
	Game::Game() :_board(new BitboardConnect4()), _game(new Connect4()), _lastMove(0)
	{
	}

	int Game::at(int x, int y)
	{
		if (x < 0 || x >= _board->getSizeX() || y < 0 || y >= _board->getSizeY()) return 0;
		if (_board->getBit(0, x, y))
			return 1;
		if (_board->getBit(1, x, y))
			return 2;
		return 0;
	}

	int Game::getLastMove()
	{
		return _lastMove;
	}

	int Game::colHeight(int col)
	{
		int i = 0;
		numtyp board;
		auto sizeX = _board->getSizeX();

		if (col <= 0 || col > sizeX)
		{
			throw "unexpected size...";
		}

		board = _board->getBoard(0) | _board->getBoard(1);
		board >>= (sizeX - col); // get the column
		while ((board & 1) == 1)
		{
			board >>= _board->getSizeX(); // get the next row
			i++;
		}
		return i;
	}

	int Game::activePlayer()
	{
		return _board->getPlayer();
	}


	bool Game::canMakeMove(int col)
	{
		return colHeight(col) != _board->getSizeY();
	}

	bool Game::makeMove(int col)
	{
		if (canMakeMove(col))
		{
			auto m = Move(col);
			_game->play(m, _board);
			_lastMove = col;
			return true;
		};
		return false;
	}

	int Game::getWinner()
	{
		return  _game->end(_board);
	}

	std::list<std::pair<int, int>> Game::getWinningLine()
	{
		auto posy = colHeight(_lastMove) -1;
		auto posx = _board->getSizeX() - _lastMove;
		std::list<std::pair<int, int>> Res;
		auto winner = getWinner();

		if (winner == 1 || winner == 2)
		{

			// horizontal...
			auto xi = posx;
			auto minxi = max(xi - 3, 0);
			auto maxxi = min(xi + 3, _board->getSizeX() - 1);
			while (xi >= minxi && at(xi, posy) == winner) { xi--; };
			minxi = xi + 1;
			xi = posx;
			while (xi <= maxxi && at(xi, posy) == winner) { xi++; };
			maxxi = xi - 1;
			if (maxxi - minxi >= 3)
			{
				for (auto i = minxi; i <= maxxi; ++i)
				{
					Res.push_front(std::pair<int, int>(i, posy));
				}
			}

			// vertical
			int yi = posy;
			int l = 0;
			while (yi >= 0 && at(posx, yi) == winner)
			{
				l++;
				yi--;
			}
			if (l >= 4)
			{
				for (yi = posy; yi > posy - 4; --yi)
				{
					Res.push_front(std::pair<int, int>(posx, yi));
				}
			}

			// Diag1
//			std::cout << "diag1" << std::endl;
			xi = posx;
			yi = posy;
			minxi = max(xi - 3, 0);
			maxxi = min(xi + 3, _board->getSizeX() - 1);
			auto minyi = max(yi - 3, 0);
			auto maxyi = min(yi + 3, _board->getSizeY() - 1);
			while (xi >= minxi && yi >= minyi && at(xi, yi) == winner)
			{
/*
				std::cout << xi << ";" << yi << std::endl;
*/
				xi--; yi--;
			}
			minxi = xi + 1;
			minyi = yi + 1;

			xi = posx;
			yi = posy;
			while (xi <= maxxi && yi <= maxyi && at(xi, yi) == winner)
			{
/*
				std::cout << xi << ";" << yi << std::endl;
*/
				xi++; yi++;
			}
			maxxi = xi - 1;
			maxyi = yi - 1;

			if (maxxi - minxi >= 3)
			{
				yi = minyi;
				for (xi = minxi; xi <= maxxi; ++xi)
				{
					Res.push_front(std::pair<int, int>(xi, yi));
					++yi;
				}
			}

			// Diag2
//			std::cout << "diag2" << std::endl;
			xi = posx;
			yi = posy;
			minxi = max(xi - 3, 0);
			maxxi = min(xi + 3, _board->getSizeX() - 1);
			minyi = max(yi - 3, 0);
			maxyi = min(yi + 3, _board->getSizeY() - 1);
			while (xi >= minxi && yi <= maxyi && at(xi, yi) == winner)
			{
/*
				std::cout << xi << ";" << yi << std::endl;
*/
				xi--; yi++;
			};
			minxi = xi + 1;
			maxyi = yi - 1;
			xi = posx;
			yi = posy;
			while (xi <= maxxi && yi >= minyi && at(xi, yi) == winner)
			{
/*
				std::cout << "diag2" << std::endl;
				std::cout << xi << ";" << yi << std::endl;
*/
				xi++; yi--;
			};
			maxxi = xi - 1;
			minyi = yi + 1;
			if (maxxi - minxi >= 3)
			{
				yi = maxyi;
				for (xi = minxi; xi <= maxxi; ++xi)
				{
					Res.push_front(std::pair<int, int>(xi, yi));
					--yi;
				}
			}
			std::cout << "output" << std::endl;
			return Res;
		}

		return Res;
	}
}