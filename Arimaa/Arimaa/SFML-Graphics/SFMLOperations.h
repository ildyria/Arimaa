#pragma once

#include <SFML/System/Resource.hpp>
#include <SFML/Graphics.hpp>

#define PI 3.14159265f

/// \addtogroup sfmlop
///  @{

/// \brief Namespace covering various operations with SFML objects, mainly vectors.


namespace sfmlop
{
    /// \brief Returns the result of the rotation of a given Vector v by a given angle.
    /// \param v Vector to rotate (it will not be altered in any way).
    /// \param angle Rotation angle in degrees, to match with sprite rotation.
	/// \returns The vector v rotated as specified.
	inline sf::Vector2f rotate(const sf::Vector2f v, const float angle)
	{
		float sinus = sin(angle*PI/180);
		float cosinus = cos(angle*PI/180);
		return sf::Vector2f(v.x*cosinus - v.y*sinus,
							v.y*cosinus + v.x*sinus);
	}

	/// \brief Returns the product of the given vector by the given coefficient.
	/// \param v The vector to multiply (it will not be altered in any way).
	/// \param coefficient a number that will multiply v's coordinates.
	/// \returns The vector v*coefficient;
	template<typename T>
	inline sf::Vector2<T> multiply(const sf::Vector2<T> v, T coefficient) { return sf::Vector2<T>(v.x * coefficient, v.y  * coefficient); }

	/// \brief Returns the division of the given vector by the given divider.
	/// \param v The vector to divide (it will not be altered in any way).
	/// \param divider a number that will divide v's coordinates.
	/// \returns The vector v/coefficient;
	inline sf::Vector2f divide(const sf::Vector2f v, float divider) { return sf::Vector2f(v.x / divider, v.y  / divider); }
	/// \brief Returns the division of the given vector by the given divider.
	/// \param v The vector to divide (it will not be altered in any way).
	/// \param divider a number that will divide v's coordinates.
	/// \returns The vector v/coefficient;
	inline sf::Vector2f divide(const sf::Vector2i v, float divider) { return sf::Vector2f( ((float) v.x) / divider, ((float) v.y)  / divider); }

    /// \brief Returns the square of the norm of the given vector. Less expensive than getNorm.
	/// \param v The vector which norm will be procesed (it will not be altered in any way).
	/// \returns The norm of v, squared.
	inline float getSquaredNorm(const sf::Vector2f v) { return v.x*v.x + v.y*v.y; }

    /// \brief Returns the norm of the given vector. More expensive than getSquaredNorm.
	/// \param v The vector which norm will be procesed (it will not be altered in any way).
	/// \returns The norm of v.
	inline float getNorm(const sf::Vector2f v) { return sqrt(sfmlop::getSquaredNorm(v)); }

	/// \brief Returns the normalization of the given Vector.
	/// \param v The vector to normalize (it will not be altered in any way).
	/// \returns The normalization of v.
	inline sf::Vector2f normalize(const sf::Vector2f v) { return sfmlop::divide(v, getNorm(v)); }

	/// \brief Handles the conversion between std::pairs and sf::vector2s.
	/// \param v The vector to convert to std::pair.
	/// \returns The std::pair corresponding to v.
	inline std::pair<int,int> toPair(sf::Vector2i v) { return std::pair<int,int>(v.x,v.y); }
	/// \brief Handles the conversion between std::pairs and sf::vector2s.
	/// \param v The vector to convert to std::pair.
	/// \returns The std::pair corresponding to v.
	inline std::pair<float,float> toPair(sf::Vector2f v) { return std::pair<float,float>(v.x,v.y); }

	/// \brief Handles the conversion between std::pairs and sf::vector2s.
	/// \param p The pair to convert to sf::vector2.
	/// \returns sf::vector2 corresponding to p.
	inline sf::Vector2i toVect2(std::pair<int,int> p) { return sf::Vector2i(p.first, p.second); }
	/// \brief Handles the conversion between std::pairs and sf::vector2s.
	/// \param p The pair to convert to sf::vector2.
	/// \returns sf::vector2 corresponding to p.
	inline sf::Vector2f toVect2(std::pair<float,float> p) { return sf::Vector2f(p.first, p.second); }

	/// \brief Handles the conversion to sf::Vector2f.
	/// \param v The vector to convert to sf::Vector2f.
	/// \returns sf::Vector2f corresponding to v.
	inline sf::Vector2f toFloatVect(sf::Vector2i v) { return sf::Vector2f((float) v.x, (float) v.y); }

	/// \brief Handles the conversion to sf::Vector2i.
	/// \param v The vector to convert to sf::Vector2i.
	/// \returns sf::Vector2i corresponding to v.
	inline sf::Vector2i toIntVect(sf::Vector2f v) { return sf::Vector2i((int) v.x, (int) v.y); }
	
	/// \brief Applies a horizontal symmetry to a given vector.
	/// \param v The vector to use the symmetry on.
	/// \returns v's horizontal symmetry.
	template<typename T>
	inline sf::Vector2<T> xSymmetry(sf::Vector2<T> v) { return sf::Vector2<T>(-v.x, v.y); }

	/// \brief Applies a vertical symmetry to a given vector.
	/// \param v The vector to use the symmetry on.
	/// \returns v's vertical symmetry.
	template<typename T>
	inline sf::Vector2<T> ySymmetry(sf::Vector2<T> v) { return sf::Vector2<T>(v.x, -v.y); }

};
/// @} End of Doxygen Groups