/**
 * \file Mcts.h
 * \brief Define class Mcts
 * \author Benoit Viguier
 * \version 1.0
 * \date December 26th 2014
 *
 */
#pragma once
#include "../tools/typedef.h"
namespace mcts {

	/**
	 * \brief MctsArgs class
	 * \details 
	 *  options for the MCTS Algorithm
	 *	u_int _depth = depth to explore before running random simulations.
	 *	u_int _timeLimitsimulationPerRoot = time limit in ms
	 *	u_long _simulationPerRoot = number of simulations to run per Root.
	 *	u_int _simulationPerLeaves = number of simulations to run per Leaves.
	 *	u_int _numberOfVisitBeforeExploration = number of visits on a node before expanding its children.
	 *	u_long _maxNumberOfLeaves = maximum number of nodes in the tree, defined by available memory * _percentRAM / 2
	 *	double _percentRAM = percentage of RAM to use by the trees
	 */
	class MctsArgs
	{

		u_int _depth;
		u_int _timeLimitsimulationPerRoot;
		u_long _simulationPerRoot;
		u_int _simulationPerLeaves;
		u_int _numberOfVisitBeforeExploration;
		u_long _maxNumberOfLeaves;
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
		 */
		explicit inline MctsArgs(
			u_int depth = 44,
			u_int timelimit = 10000,
			u_long simulR = 100000000,
			u_int simulL = 2,
			u_int numVisitExplo = 4,
			double percentRAM = 0.95
			) : _depth(depth), _timeLimitsimulationPerRoot(timelimit), _simulationPerRoot(simulR), _simulationPerLeaves(simulL), _numberOfVisitBeforeExploration(2*numVisitExplo), _percentRAM(percentRAM)
		{
#if defined(DOUBLE_TREE)
	#define DIVIDE 2 * sizeof(Node)
#else
	#define DIVIDE (sizeof(Node) + 2*sizeof(void*))
#endif

#if defined(LIMIT_MEMORY)
			_maxNumberOfLeaves = static_cast<u_long>((static_cast<u_long>(1) << 31) * _percentRAM / (DIVIDE)); // maximum size is 2 Go...
#elif defined(_WIN64)
			_maxNumberOfLeaves = static_cast<u_long>(Memory::getfreememory() * _percentRAM / (DIVIDE));
#elif defined(__linux__) || defined(__linux) || defined(linux) || defined(__gnu_linux__)
			_maxNumberOfLeaves = static_cast<u_long>(Memory::getfreememory() * _percentRAM / (DIVIDE));
#else
			_maxNumberOfLeaves = static_cast<u_long>((static_cast<u_long>(1) << 31) * _percentRAM / (DIVIDE)); // maximum size is 2 Go...
#endif
			std::cout << "size of node : " << sizeof(Node) << std::endl;

			std::cout << "max num of leaves : " << _maxNumberOfLeaves << std::endl;
		};

		/**
		 * \fn getDepth() 
		 * \brief getter for _depth
		 * 
		 * \return maximum depth
		 */
		inline u_int getDepth()
		{
			return _depth;
		};

		/**
		 * \fn setTimeLimitSimulationPerRoot(u_int t = 2000) 
		 * \brief allow to change the time limit for the simulations between executions
		 */
		inline void setTimeLimitSimulationPerRoot(u_int t = 2000)
		{
			_timeLimitsimulationPerRoot = t;
		};

		/**
		 * \fn getTimeLimitSimulationPerRoot() 
		 * \brief getter for _timeLimitsimulationPerRoot
		 * 
		 * \return maximum time limite to run the simulations
		 */
		inline u_int getTimeLimitSimulationPerRoot()
		{
			return _timeLimitsimulationPerRoot;
		};

		/**
		 * \fn getSimulationPerRoot() 
		 * \brief getter for _simulationPerRoot
		 * 
		 * \return maximum number of simulations to run at the root
		 */
		inline u_long getSimulationPerRoot()
		{
			return _simulationPerRoot;
		};

		/**
		 * \fn getSimulationPerLeaves() 
		 * \brief getter for _simulationPerLeaves
		 * 
		 * \return maximum number of simulations to run at the leaves
		 */
		inline u_int getSimulationPerLeaves()
		{
			return _simulationPerLeaves;
		};

		/**
		 * \fn getNumberOfVisitBeforeExploration() 
		 * \brief getter for _numberOfVisitBeforeExploration
		 * 
		 * \return number of simulations to run before exploring a node
		 */
		inline u_int getNumberOfVisitBeforeExploration()
		{
			return _numberOfVisitBeforeExploration;
		};

		/**
		 * \fn getMaxNumberOfLeaves() 
		 * \brief getter for _maxNumberOfLeaves
		 * 
		 * \return maximum number of leaves in the tree
		 */
		inline u_long getMaxNumberOfLeaves()
		{
			return _maxNumberOfLeaves;
		}

		/**
		 * \fn ~MctsArgs() 
		 * \brief destructor...
		 */
		inline ~MctsArgs() {}
	};

}