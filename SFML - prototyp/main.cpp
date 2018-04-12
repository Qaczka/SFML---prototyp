#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>

#ifdef linux
#include <getopt.h>
#endif // linux

void print_help()
{
	std::cout << "-p --port       default 7000\n"
		"-l --local_port default 8000\n"
		"-i --ip         default localhost\n"
		"-h --help       this message\n";
}

int main(int argc, char** argv)
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool quit = false;
	sf::UdpSocket socket;
	socket.setBlocking(false);
	socket.bind(local_port);


	sf::Packet send_packet;
	sf::Packet receive_packet;
	sf::IpAddress incomming_ip;
	unsigned short incomming_port;

	int map_width = 32, map_height = 32;
	int number_of_chunks = map_width * map_height;

	sf::RenderWindow oknoAplikacji(sf::VideoMode(1920, 1080), "Kelajno", sf::Style::Fullscreen);//to opcja fullscreen

	sf::Texture texture1;//sluzy do wczytywania tekstury bo jest texture i image
	texture1.loadFromFile("Textures/Grunt.png");//zwraca true lub false
	sf::Texture texture2;
	texture2.loadFromFile("Textures/Grunt2.png");
	sf::Texture texture3;
	texture3.loadFromFile("Textures/Drzewko.png");


	sf::Sprite *image = new sf::Sprite[number_of_chunks];
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
	}
	sf::Sprite drzewo;
	drzewo.setTexture(texture3);


	sf::FloatRect chunk_size = image[0].getGlobalBounds();//biore info wymiarow chunka na przykladzie pierwszego
	for (int i = 0; i < map_height; i++)
	{
		for (int g = 0; g < map_width; g++)
		{
			image[g + (i*map_width)].setPosition((oknoAplikacji.getSize().x*0.5 + 250) - (g*chunk_size.width),
				(oknoAplikacji.getSize().y*0.5 - 250) + (i*chunk_size.height));
			//podwojna petla "i" dla nr wiersza i "g" dla kolumny
			//do szerokosci dodaje wielokrotnosci kolumnowe i wierszowe dla wysokosci
			//250 na korekte
		}
	}
	drzewo.setPosition((oknoAplikacji.getSize().x * 0.5), (oknoAplikacji.getSize().y * 0.5));

	sf::View v = oknoAplikacji.getDefaultView();//widok ma byc taki jak okno tak jakby ciagnie z niego dane
	v.setSize(v.getSize().x, v.getSize().y * 2);//tak jak przy teksturze skalujemy 2 wieksza wysoksoc
	v.setRotation(45);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	sf::Time time;
	sf::Clock clock;
	while (!quit)
	{
		time = clock.restart();//pobranie czasu


							   //odbieranie pakietów
		while (!socket.receive(receive_packet, incomming_ip, incomming_port))
		{
			while (!receive_packet.endOfPacket())
			{
				sf::Uint8 command;
				receive_packet >> command;

				//dokumentacja przychodzących komend
				//id - oznacza, że operacja dotyczy obiektu w aktualnej grze
				//LISTA - oznacza, że operacja dotyczy nowego obiektu(dopiero do dodania do aktualnej gry)
				switch (command)
				{
				case 10://dodaj jednostke do gry
						//<LISTA_jednostki><pozycja_x><pozycja_y>
				{
					sf::Uint8 LIST_of_unit;
					sf::Uint16 x;
					sf::Uint16 y;
					receive_packet >> LIST_of_unit >> x >> y;
				}
				case 11://zmien pozycje jednostki
						//<id_jednostki><pozycja_x><pozycja_y>
				{
					sf::Uint8 id_unit;
					sf::Uint16 x;
					sf::Uint16 y;

					receive_packet >> id_unit >> x >> y;
				}
				default:
				{
					break;
				}
				}//end switch
			}
		}

		//obsluga urządzeń wejścia
		sf::Event zdarzenie;
		while (oknoAplikacji.pollEvent(zdarzenie))
		{
			switch (zdarzenie.type)
			{
			case sf::Event::Closed:
				quit = true;
				break;
			default:
				break;
			}

			switch (zdarzenie.key.code)
			{
			case sf::Keyboard::Escape:
				quit = true;
				break;
			default:
				break;
			}

			switch (zdarzenie.mouseButton.button)
			{
			case sf::Mouse::Middle:
				quit = true;
				break;
			default:
				break;
			}
		}


		//rysowanie
		oknoAplikacji.clear();
		oknoAplikacji.setView(v);//ustawia widok
		for (int i = 0; i < number_of_chunks; i++)
		{
			oknoAplikacji.draw(image[i]);//wyswietla chunki
		}
		oknoAplikacji.setView(oknoAplikacji.getDefaultView());//zeby zrzutowac drzewo prosto
		oknoAplikacji.draw(drzewo);
		oknoAplikacji.display();



		socket.send(send_packet, remote_ip, remote_port);//wyslanie pakietu
		send_packet.clear();//czyszczenie pakietu
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	delete[] image;
	oknoAplikacji.close();

	return EXIT_SUCCESS;
}

//https://www.sfml-dev.org/tutorials/2.4/window-window.php
