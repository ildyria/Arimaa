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

		unsigned int _depth;
		unsigned int _timeLimitsimulationPerRoot;
		unsigned long _simulationPerRoot;
		unsigned int _simulationPerLeaves;
		unsigned int _numberOfVisitBeforeExploration;
		unsigned long _maxNumberOfLeaves;
		double _percentRAM;

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
			unsigned int depth = 44,
			unsigned int timelimit = 10000,
			unsigned long simulR = 10000000,
			unsigned int simulL = 2,
			unsigned int numVisitExplo = 10,
			double percentRAM = 0.9
			) : _depth(depth), _timeLimitsimulationPerRoot(timelimit), _simulationPerRoot(simulR), _simulationPerLeaves(simulL), _numberOfVisitBeforeExploration(2*numVisitExplo), _percentRAM(percentRAM)
		{
#if defined(_WIN64)
			_maxNumberOfLeaves = static_cast<unsigned long>(Memory::getfreememory() * _percentRAM / (2 * sizeof(Node)));
#elif defined(__linux__) || defined(__linux) || defined(linux) || defined(__gnu_linux__)
			_maxNumberOfLeaves = static_cast<unsigned long>(Memory::getfreememory() * _percentRAM / (2 * sizeof(Node)));
#else
			_maxNumberOfLeaves = static_cast<unsigned long>((static_cast<unsigned long>(1) << 31) * _percentRAM / (2 * sizeof(Node))); // maximum size is 2 Go...
#endif
			std::cout << "size of node : " << sizeof(Node) << std::endl;

			std::cout << "max num of leaves : " << _maxNumberOfLeaves << std::endl;
		};

		inline unsigned int getDepth()
		{
			return _depth;
		};

		inline void setTimeLimitSimulationPerRoot(unsigned int t = 2000)
		{
			_timeLimitsimulationPerRoot = t;
		};

		inline unsigned int getTimeLimitSimulationPerRoot()
		{
			return _timeLimitsimulationPerRoot;
		};

		inline unsigned long getSimulationPerRoot()
		{
			return _simulationPerRoot;
		};

		inline unsigned int getSimulationPerLeaves()
		{
			return _simulationPerLeaves;
		};

		inline unsigned int getNumberOfVisitBeforeExploration()
		{
			return _numberOfVisitBeforeExploration;
		};

		inline unsigned long getMaxNumberOfLeaves()
		{
			return _maxNumberOfLeaves;
		}
		inline ~MctsArgs()
		{
		}
	};

}