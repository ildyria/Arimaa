/**
 * \file Mcts.h
 * \brief Define class Mcts
 * \author Benoit Viguier
 * \version 1.0
 * \date December 26th 2014
 *
 */
#pragma once
namespace mcts {

	/**
	 * \brief MctsArgs class
	 * \details 
	 *  options for the MCTS Algorithm
	 *	int _depth = depth to explore before running random simulations.
	 *	int _timeLimitsimulationPerRoot = time limit in ms
	 *	int _simulationPerRoot = number of simulations to run per Root.
	 *	int _simulationPerLeaves = number of simulations to run per Leaves.
	 *	int _numberOfVisitBeforeExploration = number of visits on a node before expanding its children.
	 * 
	 */
	class MctsArgs
	{

		int _depth;
		int _timeLimitsimulationPerRoot;
		int _simulationPerRoot;
		int _simulationPerLeaves;
		int _numberOfVisitBeforeExploration;


	public:

		/**
		 * \fn MctsArgs(int depth = 20, int timelimit = 5000, int simulR = 10000000, int simulL = 1, int numVisitExplo = 0)
		 * \brief initialisation of the parameters.
		 * 
		 * \param depth : depth of the maximum search
		 * \param timelimit : time limit for the simulations
		 * \param t : number of root simulations
		 * \param s : number of leaf simulations
		 * \return [description]
		 */
		explicit inline MctsArgs(
			int depth = 40,
			int timelimit = 2000,
			int simulR = 10000000,
			int simulL = 1,
			int numVisitExplo = 1
			) : _depth(depth), _timeLimitsimulationPerRoot(timelimit), _simulationPerRoot(simulR), _simulationPerLeaves(simulL), _numberOfVisitBeforeExploration(numVisitExplo)
		{
		};

		inline int getDepth()
		{
			return _depth;
		};

		inline void setTimeLimitSimulationPerRoot(int t = 2000)
		{
			_timeLimitsimulationPerRoot = t;
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

		inline ~MctsArgs()
		{
		}
	};

}