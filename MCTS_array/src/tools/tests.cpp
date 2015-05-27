#include "tests.h"

void fun_test1 ()
 {
	v_stat vect1 = 
	{
		n_stat(6,p_stat(1,1)),
		n_stat(5,p_stat(1,2)),
		n_stat(4,p_stat(1,3)),
		n_stat(3,p_stat(1,4)),
		n_stat(2,p_stat(1,5)),
		n_stat(1,p_stat(1,6))
	};

	v_stat vect2 = 
	{
		n_stat(16,p_stat(1,5)),
		n_stat(12,p_stat(4,2)),
		n_stat(8,p_stat(7,3)),
		n_stat(6,p_stat(6,4)),
		n_stat(4,p_stat(1,5)),
		n_stat(2,p_stat(2,6))
	};


	std::cout << std::endl;
	std::cout << "vect1 :" << std::endl;
	std::cout << vect1;

	std::cout << std::endl;
	std::cout << "vect1 sorted second :" << std::endl;
	sort(vect1.begin(),vect1.end(),second);
	std::cout << vect1;

	std::cout << std::endl;
	std::cout << "vect1 sorted first :" << std::endl;
	sort(vect1.begin(),vect1.end(),first);
	std::cout << vect1;

	std::cout << std::endl;
	std::cout << "vect2 sorted first :" << std::endl;
	sort(vect2.begin(),vect2.end(),first);
	std::cout << vect2;

	v_stat v = vect1 + vect2;

	std::cout << std::endl;
	std::cout << "v = vect1 + vect2 :" << std::endl;
	std::cout << v;

	vect1 += vect2;

	std::cout << std::endl;
	std::cout << "vect1 += vect2 :" << std::endl;
	std::cout << vect1 << std::endl;
}

void display_board_num()
{
	int num = SIZEY*SIZEX - 1;
	for (int i = 0; i < SIZEY; ++i)
	{
		std::cout << "   ";
		for (int j = 0; j < SIZEX; ++j)
		{
			std::cout << "+---";
		}
		std::cout << "+" << std::endl;
		std::cout << "   ";
		for (int j = 0; j < SIZEX; ++j)
		{
			std::cout << "| ";
			std::cout << num;
			if(num < 10) std::cout << " ";
			num --;
		}
		std::cout << "|" << std::endl;
	}
	std::cout << "   ";
	for (int j = 0; j < SIZEX; ++j)
	{
		std::cout << "+---";
	}
	std::cout << "+" << std::endl;
}

void fun_test_Arimaa1(prog_options options)
{
	Arimaa* game = new Arimaa();
	Bitboard* Bb = new Bitboard((NB_PIECE+1)*2,1);

	std::string t = "";
	game->diplay_board(Bb);
	Move temp;
	while (true)
	{
		std::cin >> t;
		if(t == "exit") break;
		try
		{
			Move temp = Arimaa_tools::convert_move(t);
			std::cout << Arimaa_tools::convert_move(temp) << std::endl;
		}
		catch(...)
		{
			std::cout << "mouvement non valide." << std::endl;
		}
	}
}

void fun_test_Arimaa2(prog_options options)
{
	Arimaa* game = new Arimaa();
	Bitboard* Bb = new Bitboard((NB_PIECE+1)*2,1);

	Bb->setBit(0, 0);
	game->diplay_board(Bb);
	Bb->setBit(1, 1);
	game->diplay_board(Bb);
	Bb->setBit(2, 2);
	game->diplay_board(Bb);
	Bb->setBit(3, 3);
	game->diplay_board(Bb);
	Bb->setBit(4, 4);
	game->diplay_board(Bb);
	Bb->setBit(5, 5);
	game->diplay_board(Bb);
	Bb->setBit(6, 6);
	game->diplay_board(Bb);
	Bb->setBit(7, 7);
	game->diplay_board(Bb);
	Bb->setBit(8, 8);
	// Bb->setBit(9, 9);
	// Bb->setBit(10, 10);
	// Bb->setBit(11, 11;
	// Bb->setBit(12, 12);
	// Bb->setBit(13, 13);
	// Bb->setBit(14, 14);
	game->diplay_board(Bb);
}

void fun_test_Arimaa3(prog_options options)
{
	Arimaa* game = new Arimaa();
	Bitboard* Bb = new Bitboard((NB_PIECE+1)*2,1);

	Bb->setBit(0, 7);
	Bb->setBit(0, 10);
	Bb->setBit(0, 25);
	Bb->setBit(0, 28);
	Bb->setBit(NB_PIECE, 7);
	Bb->setBit(NB_PIECE, 10);
	Bb->setBit(NB_PIECE, 25);
	Bb->setBit(NB_PIECE, 28);
	game->diplay_board(Bb);
	std::cout << "apply traps " << std::endl;
	game->apply_traps(Bb);
	game->diplay_board(Bb);


	Bb->setBit(0, 7);
	Bb->setBit(0, 1);
	Bb->setBit(0, 10);
	Bb->setBit(0, 9);
	Bb->setBit(0, 25);
	Bb->setBit(0, 26);
	Bb->setBit(0, 28);
	Bb->setBit(0, 34);

	Bb->setBit(NB_PIECE, 7);
	Bb->setBit(NB_PIECE, 1);
	Bb->setBit(NB_PIECE, 10);
	Bb->setBit(NB_PIECE, 9);
	Bb->setBit(NB_PIECE, 25);
	Bb->setBit(NB_PIECE, 26);
	Bb->setBit(NB_PIECE, 28);
	Bb->setBit(NB_PIECE, 34);

	game->diplay_board(Bb);
	std::cout << "apply traps " << std::endl;
	game->apply_traps(Bb);
	game->diplay_board(Bb);
}

void fun_test_Arimaa4(prog_options options)
{
	Arimaa* game = new Arimaa();
	Bitboard* Bb = new Bitboard((NB_PIECE+1)*2,1);

	Bb->setBit(0, 7);
	Bb->setBit(NB_PIECE, 7);
	game->diplay_board(Bb);

	game->list_moves_available(Bb);
	
}