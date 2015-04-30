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
#include <string>

/**
 * \brief Count class : SINGLETON
 * \details 
 * class that is used to keep the counts of various numbers
 */
class Count
{
public:
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
