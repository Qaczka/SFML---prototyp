#ifndef NETWORK_OPCODES_HPP_INCLUDED
#define NETWORK_OPCODES_HPP_INCLUDED

#include <SFML/Main.hpp>

// ID - oznacza, ¿e operacja dotyczy konkretnego obiektu.
// BP - oznacza, ¿e operacja dotyczy mo¿liwego okietu.
enum OPCODES : sf::Uint8
{

	UNUSED,

	MOVE_UNIT,//przesuniêcie jednostki
			  // [sf::Uint8 ID_jednostki] [sf::Uint8 pozycja_x] [sf::Uint8 pozycja_y]

			  ATTACK,//atakuj
					 // [sf::Uint8 ID_jednostki_naszej] [sf::Uint8 ID_jedostki_atakowanej]

					 CREATE_UNIT,//stwórz jednostkê
								 // [sf::Uint8 ID_budynku_z_którego_budujemy] [sf::Uint8 BP_jednostki]

								 CREATE_BUILDING,//wybudowanie budynku
												 // [sf::Uint8 BP_budynku] [sf::Uint8 pozycja_x] [sf::Uint8 pozycja_y]

												 /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

												 ADD_UNIT_TO_GAME,//dodaj jednostke do gry
																  // [sf::Uint8 BP_jednostki] [sf::Uint8 pozycja_x] [sf::Uint8 pozycja_y]

																  SET_UNIT_POSITION,//zmien pozycje jednostki
																					// [sf::Uint8 ID_jednostki] [sf::Uint8 pozycja_x] [sf::Uint8pozycja_y]

																					REMOWE_UNIT_FROM_GAME,//usuñ jednostke z gry
																										  // [sf::Uint8 ID_jednostki]

};

#endif // NETWORK_OPCODES_HPP_INCLUDED
