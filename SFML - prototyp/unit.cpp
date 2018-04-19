#include "unit.hpp"

unit::unit() {}

unit::unit(sf::Uint8 x, sf::Uint8 y)
{
	this->x = x;
	this->y = y;
}

void unit::set_position(sf::Uint8 x, sf::Uint8 y)
{
	this->x = x;
	this->y = y;
}

sf::Uint8 unit::get_x()
{
	return x;
}

sf::Uint8 unit::get_y()
{
	return y;
}
