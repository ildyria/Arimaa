#pragma once
namespace mcts {

	class MctsArgs
	{

		int _depth;
		int _timeLimitsimulationPerRoot;
		int _simulationPerRoot;
		int _simulationPerLeaves;
		int _numberOfVisitBeforeExploration;


	public:

		explicit MctsArgs(
			int depth = 20,				// depth of the maximum search
//			int depth = 10,				// depth of the maximum search
			int timelimit = 10000,		// time limit for the simulations
//			int timelimit = 40000,		// time limit for the simulations
			int simulR = 1000000,		// number of root simulations
			int simulL = 1,				// number of leaf simulations
//			int numVisitExplo = 0		// number visit required before explorating a node
			int numVisitExplo = 5		// number visit required before explorating a node
			);

		inline int getDepth()
		{
			return _depth;
		};

		inline int getTimeLimitSimulationPerRoot()
		{
			return _timeLimitsimulationPerRoot;
		};

		inline int getSimulationPerRoot()
		{
			return _simulationPerRoot;
		};

		inline int getSimulationPerLeaves()
		{
			return _simulationPerLeaves;
		};

		inline int getNumberOfVisitBeforeExploration()
		{
			return _numberOfVisitBeforeExploration;
		};

		~MctsArgs();
	};

}