#include "TicTacToe.h"
#include "Mcts.h"
#define elseif else if

//using namespace std;
using namespace mcts;
/*using std::vector;
using std::list;
using std::string;
using std::cin;
using std::cout;
using std::endl;*/

int main(int argc, char const *argv[])
{
//	map<string, Bitboard> pm;
	cout << endl << "\t\t    If it compiles then it works ! " << endl;
	cout << "\tBut remember, all code is guilty until proven inocent !" << endl << endl;

	Bitboard Bb = Bitboard(3,2,1);
	Bitboard bitboard = Bitboard(3, 2, 1);
	int result = 0, moveok;
	string move;
	list<string> Listtoprint;
	list<string>::iterator iter;

	Mcts mcts = Mcts(bitboard);
	cout << mcts.GetBestMove() << endl;




	while (result == 0)
	{
		TicTacToe::diplayBoard(Bb);

		Listtoprint = TicTacToe::listPossibleMoves(Bb);
		cout << "possible moves : ";
		for (iter = Listtoprint.begin(); iter != Listtoprint.end(); iter++){
			cout << *iter << " ";
		}
		cout << endl;

		moveok = 0;
		while (moveok == 0)
		{
			cin >> move;
			iter = find(Listtoprint.begin(), Listtoprint.end(), move);
			if (iter != Listtoprint.end())
			{
				moveok = 1;
			}
		}
		TicTacToe::play(move, Bb);
		result = TicTacToe::end(Bb);
	}
	cout << endl;
	TicTacToe::diplayBoard(Bb);

	_sleep(2000);
}