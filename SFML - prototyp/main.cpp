#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <cmath>

//#include "../common/network_opcodes.hpp"
#include "network_opcodes.hpp"

#ifdef linux
#include <getopt.h>
#endif // linux

void umiesc_obiekt(sf::Sprite& obiekt,
	sf::Vector2u wys_obiektu,
	sf::Sprite *chunki,
	int ktory,
	sf::Vector2u texture1_size,
	sf::RenderWindow& okno,
	sf::View v)
{
	sf::Vector2f odn1;
	sf::Vector2i odn2;
	odn1 = chunki[ktory].getPosition();
	odn2 = okno.mapCoordsToPixel(odn1, v);
	obiekt.setPosition((odn2.x + (texture1_size.x*sqrt(2) / 2) - (texture1_size.x / 2)),
		(odn2.y - wys_obiektu.y + (texture1_size.y*sqrt(2) / 2) - (texture1_size.y / 2)));
	//0.4142*32/2  CZYLI: biore dane przeksztalcenia
	//--> odejmuje wysokosc zeby rysowanie bylo od lewgo dolnego rogu
	//--> i teraz obnizam rysowanie tekstury do najnizszego punktu rysowania chunka
	//--> uwzgledniam pierwsiatek po prekstalceniu w wymiarach

	//sa przekatne wiec wymiary chunka razy sqrt2 i w jego polowie czyli /2
	//a potem przesuwam o polowe wymiaru chunka
	//czyli przsuniecie jest o polowe zwiekszenia wymiaru przez pierwiastek czyli to 0.41/2
}

void print_help()
{
	std::cout << "-p --port       default 7000\n"
		"-l --local_port default 8000\n"
		"-i --ip         default localhost\n"
		"-h --help       this message\n";
}
//---------------------------------------------------------------------------------------------------------------------//
int main(int argc, char** argv)
{
	//zmienne które można modyfikować argumentami z konsoli
	unsigned short local_port = 8000;
	unsigned short remote_port = 7000;//port do którego się łączymy
	sf::IpAddress remote_ip = "localhost";//ip do którego się łączymy

#ifdef linux
	{
		static struct option longopts[] =
		{
			{ "port",       required_argument, NULL, 'p' },
		{ "local_port", required_argument, NULL, 'l' },
		{ "ip",         required_argument, NULL, 'i' },
		{ "help",       no_argument,       NULL, 'h' },
		};

		int c;
		while ((c = getopt_long(argc, argv, "p:l:i:h", longopts, NULL)) != -1)
		{
			switch (c)
			{
			case 'p':
				remote_port = std::stoul(optarg, NULL, 0);
				break;
			case 'l':
				local_port = std::stoul(optarg, NULL, 0);
				break;
			case 'i':
				remote_ip = optarg;
				break;
			case 'h':
				std::cout << argv[0] << "\n";
				print_help();
				return 0;
			case '?':
				return 0;
			case ':':
				return 0;
			}
		}
	}
#endif // linux
	//---------------------------------------------------------------------------------------------------------------------//
	bool quit = false;
	sf::UdpSocket socket;
	socket.setBlocking(false);
	socket.bind(local_port);


	sf::Packet send_packet;
	sf::Packet receive_packet;
	sf::IpAddress incomming_ip;
	unsigned short incomming_port;

	//POMOCNE ZMIENNE
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	int map_width = 36, map_height = 36;
	int number_of_chunks = map_width * map_height;
	int number_of_trees1 = 8;
	int speed_of_scrolling = 10;
	double object_scroll = speed_of_scrolling*sqrt(2);//predkosc po przekstalceniu

	//MODYFIKOWANIE OKNA APLIKACJI
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	sf::RenderWindow oknoAplikacji(sf::VideoMode(1920, 1080), "Kelajno", sf::Style::Fullscreen);//to opcja fullscreen
	oknoAplikacji.setFramerateLimit(60);//ustawiam limit fps na 60

	//POMOCNE ZMIENNE 2
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	sf::Vector2i mouse_possition = sf::Mouse::getPosition(oknoAplikacji);

	//WCZYTYWANIE TEKSTUR
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	sf::Texture texture1;//sluzy do wczytywania tekstury bo jest texture i image
	sf::Texture texture2;
	sf::Texture texture3;
	sf::Texture texture4;

	texture1.loadFromFile("Textures/Grunt.png");//zwraca true lub false
	texture2.loadFromFile("Textures/Grunt2.png");
	texture3.loadFromFile("Textures/Drzewko.png");
	texture4.loadFromFile("Textures/Drzewko2.png");

	sf::Vector2u texture1_size = texture1.getSize();
	sf::Vector2u texture2_size = texture2.getSize();
	sf::Vector2u texture3_size = texture3.getSize();
	sf::Vector2u texture4_size = texture4.getSize();

	//TABLICE OBIEKTÓW
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	sf::Sprite *image = new sf::Sprite[number_of_chunks];
	sf::Sprite *drzewo = new sf::Sprite[number_of_trees1];

	//PRZYPISYWANIE TEKSTUR DO CHUNKÓW
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	for (int i = 0; i < number_of_chunks; i++)
	{
		if (i % 2 == 0)
		{
			image[i].setTexture(texture1);
		}
		else
		{
			image[i].setTexture(texture2);
		}

		if (i < number_of_trees1)
		{
			drzewo[i].setTexture(texture4);
		}
	}

	//ROZMIESZCZANIE CHUNKÓW W PRZESTRZENI
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	sf::FloatRect chunk_size = image[0].getGlobalBounds();//biore info wymiarow chunka na przykladzie pierwszego
	for (int i = 0; i < map_height; i++)
	{
		for (int g = 0; g < map_width; g++)
		{
			image[g + (i*map_width)].setPosition((oknoAplikacji.getSize().x*0.5 + 250) - (g*chunk_size.width),
				(oknoAplikacji.getSize().y*0.5 - 250) + (i*chunk_size.height));
			//podwojna petla "i" dla nr wiersza i "g" dla kolumny
			//do szerokosci dodaje wielokrotnosci kolumnowe i wierszowe dla wysokosci 250 na korekte
		}
	}

	//MODYFIKOWANIE WIDOKU
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	sf::View chunk_view = oknoAplikacji.getDefaultView();//widok ma byc taki jak okno tak jakby ciagnie z niego dane
	chunk_view.setSize(chunk_view.getSize().x, chunk_view.getSize().y * 2);//tak jak przy teksturze skalujemy 2 wieksza wysoksoc
	chunk_view.setRotation(45);
	chunk_view.setCenter(oknoAplikacji.getSize().x*0.5, oknoAplikacji.getSize().y*0.5);

	sf::View object_view = oknoAplikacji.getDefaultView();

	//UMIESZCZANIE OBIEKTÓW NA KONKRETNYCH CHUNKACH
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	umiesc_obiekt(drzewo[0], texture4_size, image, 1000, texture1_size, oknoAplikacji, chunk_view);
	umiesc_obiekt(drzewo[1], texture4_size, image, 250, texture1_size, oknoAplikacji, chunk_view);
	umiesc_obiekt(drzewo[2], texture4_size, image, 453, texture1_size, oknoAplikacji, chunk_view);
	umiesc_obiekt(drzewo[3], texture4_size, image, 841, texture1_size, oknoAplikacji, chunk_view);
	umiesc_obiekt(drzewo[4], texture4_size, image, 134, texture1_size, oknoAplikacji, chunk_view);
	umiesc_obiekt(drzewo[5], texture4_size, image, 345, texture1_size, oknoAplikacji, chunk_view);
	umiesc_obiekt(drzewo[6], texture4_size, image, 612, texture1_size, oknoAplikacji, chunk_view);
	umiesc_obiekt(drzewo[7], texture4_size, image, 748, texture1_size, oknoAplikacji, chunk_view);

	//---------------------------------------------------------------------------------------------------------------------//
	sf::Time time;
	sf::Clock clock;
	//GŁÓWNA PĘTLA GRY
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	while (!quit)
	{
		time = clock.restart();//pobranie czasu
		mouse_possition = sf::Mouse::getPosition(oknoAplikacji);//pobieram wspolrzedne myszki w kazdej klatce


		//GŁÓWNA PĘTLA PAKIETÓW
		//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		while (!socket.receive(receive_packet, incomming_ip, incomming_port))
		{
			while (!receive_packet.endOfPacket())
			{
				sf::Uint8 opcode;
				receive_packet >> opcode;

				switch (opcode)
				{
				case ADD_UNIT_TO_GAME:
				{
					sf::Uint8 LISTA_jednostki;
					sf::Uint16 x;
					sf::Uint16 y;
					receive_packet >> LISTA_jednostki >> x >> y;
					break;
				}
				case SET_UNIT_POSITION:
				{
					sf::Uint8 ID_jednostki;
					sf::Uint16 x;
					sf::Uint16 y;
					receive_packet >> ID_jednostki >> x >> y;
					break;
				}
				default:
				{
					break;
				}
				}//end switch
			}
		}

		//OBSŁUGA URZĄDZEŃ WEJŚCIA
		//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		sf::Event zdarzenie;
		while (oknoAplikacji.pollEvent(zdarzenie))
		{
			switch (zdarzenie.type)
			{
			case sf::Event::Closed:
				quit = true;
				break;
			case sf::Event::KeyPressed:

				switch (zdarzenie.key.code)
				{
				case sf::Keyboard::Escape:
				quit = true;
				break;
				default:
				break;
				}
				break;
			case sf::Event::MouseButtonPressed:
				quit = true;
				break;
			default:
				break;
			}
			
			//--------------- PRZESUWANIE KAMERY --------------------------------------------//
			if (mouse_possition.x == 0)
			{
				chunk_view.move(-speed_of_scrolling, -speed_of_scrolling);//kamera w lewo
				object_view.move(-object_scroll, 0);
			}
			if (mouse_possition.y == 0)
			{
				chunk_view.move(speed_of_scrolling, -speed_of_scrolling);//kamera w gore
				object_view.move(0, -object_scroll/2);//W OSI Y TRZEBA POLOWA BO RENDER BYL W 2 WIEKSZEJ WIELKOSCI
			}
			if (mouse_possition.x == oknoAplikacji.getSize().x-1)
			{
				chunk_view.move(speed_of_scrolling, speed_of_scrolling);//kamera w prawo
				object_view.move(object_scroll, 0);
			}
			if (mouse_possition.y == oknoAplikacji.getSize().y-1)
			{
				chunk_view.move(-speed_of_scrolling, speed_of_scrolling);//kamera w dol
				object_view.move(0, object_scroll/2);//W OSI Y TRZEBA POLOWA BO RENDER BYL W 2 WIEKSZEJ WIELKOSCI
			}
			//dla objektow uzywam object_scroll tutaj sqrt(2)*speed_of_scrolling bo po przekstalceniu
			
		}

		//RENDER OBRAZU
		//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		oknoAplikacji.clear();
		oknoAplikacji.setView(chunk_view);//ustawia widok
		for (int i = 0; i < number_of_chunks; i++)
		{
			oknoAplikacji.draw(image[i]);//wyswietla chunki
		}
		//oknoAplikacji.setView(oknoAplikacji.getDefaultView());//zeby zrzutowac jednostki i obiekty prosto
		oknoAplikacji.setView(object_view);
		for (int i = 0; i < number_of_trees1; i++)
		{
			oknoAplikacji.draw(drzewo[i]);
		}
		oknoAplikacji.display();

		socket.send(send_packet, remote_ip, remote_port);//wyslanie pakietu
		send_packet.clear();//czyszczenie pakietu
	}
	//---------------------------------------------------------------------------------------------------------------------//
	delete[] image;
	delete[] drzewo;
	oknoAplikacji.close();

	return EXIT_SUCCESS;
}

//https://www.sfml-dev.org/tutorials/2.4/window-window.php
