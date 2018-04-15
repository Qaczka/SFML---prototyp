#include "player.hpp"

player::player(sf::IpAddress ip, unsigned short port)
{
	this->ip = ip;
	this->port = port;
}