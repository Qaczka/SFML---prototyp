#include "unit.hpp"

unit::unit()
{

}

unit::unit(sf::Uint16 x, sf::Uint16 y)
{
	this->x = x;
	this->y = y;
}

void unit::set_position(sf::Uint16 x, sf::Uint16 y)
{
	this->x = x;
	this->y = y;
}

sf::Uint16 unit::get_x()
{
	return x;
}

sf::Uint16 unit::get_y()
{
	return y;
}