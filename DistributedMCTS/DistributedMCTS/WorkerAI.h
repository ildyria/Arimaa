#pragma once

#include "voteAi.h"

class WorkerAI
{
public:
	explicit WorkerAI(prog_options& options);
	~WorkerAI();

private:
	//api_v2::Game* m_game;
	api_v2::Ai m_ai;

	void run();

	bool onTimeRecv(int ttime);
	bool onMoveRecv(u_long move);
	bool onStateRecv(std::vector<u_long> state);

	void vote();
};

