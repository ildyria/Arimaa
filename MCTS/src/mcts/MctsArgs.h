#pragma once
namespace mcts {

	class MctsArgs
	{

		int _depth;
		int _timeLimitsimulationPerRoot;
		int _simulationPerRoot;
		int _simulationPerLeaves;

	public:

		explicit MctsArgs(
			int depth = 4,
			int timelimit = 5000,
			int simulR = 1000,
			int simulL = 10
			);

		int getDepth();

		int getTimeLimitSimulationPerRoot();

		int getSimulationPerRoot();

		int getSimulationPerLeaves();

		~MctsArgs();
	};

}