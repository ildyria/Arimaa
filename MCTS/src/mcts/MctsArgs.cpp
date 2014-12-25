#include "MctsArgs.h"
namespace mcts{

	MctsArgs::MctsArgs(int depth, int timelimit, int simulR, int simulL, int numVisitExplo) :
		_depth(depth),
		_timeLimitsimulationPerRoot(timelimit),
		_simulationPerRoot(simulR),
		_simulationPerLeaves(simulL),
		_numberOfVisitBeforeExploration(numVisitExplo)
	{
	}

	MctsArgs::~MctsArgs()
	{
	}

}