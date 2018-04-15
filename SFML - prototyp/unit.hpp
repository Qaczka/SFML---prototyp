#pragma once

#ifndef UNIT_HPP_INCLUDED
#define UNIT_HPP_INCLUDED

#include <SFML/Main.hpp>

class unit
{
	sf::Uint16 x;
	sf::Uint16 y;

public:
	unit();
	unit(sf::Uint16 x, sf::Uint16 y);
	void set_position(sf::Uint16 x, sf::Uint16 y);
	sf::Uint16 get_x();
	sf::Uint16 get_y();

};

#endif // UNIT_HPP_INCLUDED