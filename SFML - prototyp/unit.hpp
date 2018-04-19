#ifndef UNIT_HPP_INCLUDED
#define UNIT_HPP_INCLUDED

#include <SFML/Main.hpp>

class unit
{
	sf::Uint8 x;
	sf::Uint8 y;

public:
	unit();
	unit(sf::Uint8 x, sf::Uint8 y);
	void set_position(sf::Uint8 x, sf::Uint8 y);
	sf::Uint8 get_x();
	sf::Uint8 get_y();

};

#endif // UNIT_HPP_INCLUDED
