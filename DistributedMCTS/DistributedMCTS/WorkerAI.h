#pragma once

#include "voteAi.h"

class WorkerAI
{
public:
	explicit WorkerAI(int t = 5);
	~WorkerAI();

private:
	api::Game* m_game;
	api::Ai m_ai;

	void run();

	bool onTimeRecv(int ttime);
	bool onGameRecv(api::Game* game);

	void vote();
};

