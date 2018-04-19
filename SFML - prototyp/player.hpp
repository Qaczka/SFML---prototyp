#pragma once

#ifndef PLAYER_HPP_INCLUDED
#define PLAYER_HPP_INCLUDED

#include <SFML/Network.hpp>

class player
{
	sf::IpAddress ip;
	unsigned short port;

public:
	player(sf::IpAddress ip, unsigned short port);

};

#endif // PLAYER_HPP_INCLUDED