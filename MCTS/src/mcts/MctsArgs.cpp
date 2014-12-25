#include "MctsArgs.h"
namespace mcts{

	MctsArgs::MctsArgs(int depth, int timelimit, int simulR, int simulL) :
		_depth(depth),
		_timeLimitsimulationPerRoot(timelimit),
		_simulationPerRoot(simulR),
		_simulationPerLeaves(simulL)
	{
	}

	int MctsArgs::getDepth()
	{
		return _depth;
	}

	int MctsArgs::getTimeLimitSimulationPerRoot()
	{
		return _timeLimitsimulationPerRoot;
	}

	int MctsArgs::getSimulationPerRoot()
	{
		return _simulationPerRoot;
	}

	int MctsArgs::getSimulationPerLeaves()
	{
		return _simulationPerLeaves;
	}

	MctsArgs::~MctsArgs()
	{
	}

}