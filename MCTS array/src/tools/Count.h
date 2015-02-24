/**
 * \file Count.h
 * \brief Define singleton Count
 * \author Benoit Viguier
 * \version 1.0
 * \date January 5th 2015
 *
 */
#pragma once
#include <iostream>
#include "Memory.h"
#include <string>

/**
 * \brief Count class : SINGLETON
 * \details 
 * class that is used to keep the counts of various numbers
 */
class Count
{
	long _nodes;
	long _bitboads;
	long _bitboadsC4;
	long _nbleaves;
	long _nodes_created;

	long _nodes_saved;
	long _bitboads_saved;
	long _bitboadsC4_saved;
	long _nbleaves_saved;

	long _max_depth;

	static Count* UniqueInstance;
	
	Count();
	~Count();

	static inline long ratio(long& a, long& b)
	{
		return (b != 0) ? (a * 100 / b) - 100 : 0;
	}
public:
	
	/**
	 * \fn I
	 * \brief get the instance of the singleton.
	 * \param  seed optionnal, allow us to set the seed in case singleton hasn't been initialized
	 * \return the object.
	 */
	static Count* I();
	
	inline void addNode() { _nodes++; }
	inline void rmNode() { _nodes--; }

	inline void createNode(int n = 1) { _nodes_created += n; }

	inline void addBitBoards() { _bitboads++; }
	inline void rmBitBoards() { _bitboads--; }

	inline void addBitBoardsC4() { _bitboadsC4++; }
	inline void rmBitBoardsC4() { _bitboadsC4--; }
	
	inline void saveNbLeaves(long n) { _nbleaves = n; }
	inline void saveMaxDepth(int MaxDepth) { _max_depth = MaxDepth; };

	inline void clear()
	{
		_nodes = 0;
		_bitboads = 0;
		_bitboadsC4 = 0;
		_nbleaves = 0;
		_nodes_created = 0;

		_nodes_saved = 1;
		_bitboads_saved = 1;
		_bitboadsC4_saved = 1;
		_nbleaves_saved = 1;

		_max_depth = 0;
	}

	friend std::ostream& operator<< (std::ostream& stream, Count* count) {
		stream << "nb nodes  : " << Count::format(count->_nbleaves) << " (" << Count::format(count->_nbleaves - count->_nbleaves_saved);
		if (count->ratio(count->_nbleaves, count->_nbleaves_saved) < 100)
		{
			stream << " : " << count->ratio(count->_nbleaves, count->_nbleaves_saved) << "%";
		}
		else
		{
			stream << " : 999%";
		}

		stream << ")" << std::endl;
//		stream << "nb nodes  : " << count->_nodes << " (" << (count->_nodes - count->_nodes_saved) << " " << count->ratio(count->_nodes, count->_nodes_saved) << "%)" << std::endl;
//		stream << "nb BbC4   : " << Count::format(count->_bitboadsC4) << " (" << Count::format(count->_bitboadsC4 - count->_bitboadsC4_saved) << " ; " << count->ratio(count->_bitboadsC4, count->_bitboadsC4_saved) << "%)" << std::endl;
//		stream << "max depth : " << count->_max_depth << std::endl;
//		if (count->_nodes_created > 0) stream << "add nodes : " << count->_nodes_created << std::endl;
		count->_nodes_saved = count->_nodes;
		count->_bitboads_saved = count->_bitboads;
		count->_bitboadsC4_saved = count->_bitboadsC4;
		count->_nbleaves_saved = count->_nbleaves;
		count->_nodes_created = 0;
//		Memory::WinFUllInfo();
		return stream;
	};

	inline static std::string format(long value){
		std::string numWithCommas = std::to_string(value);
		return format_string(numWithCommas);
	}

	inline static std::string format(u_long value){
		std::string numWithCommas = std::to_string(value);
		return format_string(numWithCommas);
	}

	inline static std::string format(long long value){
		std::string numWithCommas = std::to_string(value);
		return format_string(numWithCommas);
	}

	inline static std::string& format_string(std::string& numFormated){
		short insertPosition = static_cast<short>(numFormated.length()) - 3;
		while (insertPosition > 0) {
			numFormated.insert(insertPosition, " ");
			insertPosition -= 3;
		}
		return numFormated;
	}
};
