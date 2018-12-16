#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <time.h>

//#include "../common/network_opcodes.hpp"
#include "network_opcodes.hpp"

#ifdef linux
#include <getopt.h>
#endif // linux
using namespace sf;
using namespace std;

void umiesc_obiekt(Sprite& obiekt, Vector2u wys_obiektu, Sprite *chunki, int ktory,
	Vector2u texture1_size, RenderWindow& okno, View v)
{
	Vector2f odn1;
	Vector2i odn2;
	odn1 = chunki[ktory].getPosition();
	odn2 = okno.mapCoordsToPixel(odn1, v);
	obiekt.setPosition((odn2.x + (texture1_size.x*sqrt(2) / 2) - (texture1_size.x / 2)),
		(odn2.y - wys_obiektu.y + (texture1_size.y*sqrt(2) / 2) - (texture1_size.y / 2)));

	//Subtracting height in order to draw it from left to right corner
	//, now I'm leading texture drawing to the lowest point of drawing it
	//I have to keep in mind now that there is square root relation in dimensions
	//that is: dimensions are diagonal so we have to multiply them by sqrt2,
	//second dimension is half of the length, then I'm shifting by it
	//It is shifted by half of the dimension increment by sqrt (~0.41/2)
}

void print_help()
{
	cout << "-p --port       default 7000\n"
		"-l --local_port default 8000\n"
		"-i --ip         default localhost\n"
		"-h --help       this message\n";
}
//---------------------------------------------------------------------------------------------------------------------//
int main(int argc, char** argv)
{
	//Variables for modification from the console
	unsigned short local_port = 8000;//The port from which application is opening connections
	unsigned short remote_port = 7000;//The port we connect to
	IpAddress remote_ip = "localhost";//Ip we connect to

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
				remote_port = stoul(optarg, NULL, 0);
				break;
			case 'l':
				local_port = stoul(optarg, NULL, 0);
				break;
			case 'i':
				remote_ip = optarg;
				break;
			case 'h':
				cout << argv[0] << "\n";
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
	bool quit = false;//main disable swich

	//HELPFULL VARIABLES
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	UdpSocket socket;
	socket.setBlocking(false);
	socket.bind(local_port);

	Packet send_packet;
	Packet receive_packet;
	IpAddress incomming_ip;
	unsigned short incomming_port;

	//HELPFULL VARIABLES 2
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	int map_width = 100, map_height = 100;
	int number_of_chunks = map_width * map_height;
	int number_of_trees1 = 32;
	int number_of_bushes1 = 64;
	int number_of_units1 = 2;
	int speed_of_scrolling = 10;
	float game_zoom = 0.0f; // From -1.5 to 0.5
	int zoom_step=60; //Zoom power multipier
	double object_scroll = speed_of_scrolling*sqrt(2);//Velocity after transformation
	int resolution_width = 1920, resolution_height=1080;
	srand(time(NULL));
	int random_number1 = rand();
	Vector2i mouse_position;


	//MODIFICATION OF APPLICATION WINDOW
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	RenderWindow applicationWindow(VideoMode(resolution_width, resolution_height), "Kelajno", Style::Fullscreen);//This is fullscreen option
	applicationWindow.setFramerateLimit(60);//FPS limit is set to 60

	//HELPFULL VARIABLES 3
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	mouse_position = Mouse::getPosition(applicationWindow);
	srand(time(NULL));

	//LOADING TEXTURES
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	Texture texture1;//Serves as texture loader
	Texture texture2;
	Texture texture3;
	Texture texture_tree1;
	Texture texture_bush1;
	Texture texture5;
	Texture texture_grass1;
	Texture texture_grass2;
	Texture texture_grass3;
	Texture texture_test1;

	texture1.loadFromFile("Textures/Grunt.png");//Returns bool
	texture2.loadFromFile("Textures/Grunt2.png");
	texture3.loadFromFile("Textures/Drzewko.png");
	texture_tree1.loadFromFile("Textures/Drzewko2.png");
	texture_bush1.loadFromFile("Textures/Bush1.png");
	texture5.loadFromFile("Textures/Czolg.png");
	texture_grass1.loadFromFile("Textures/Trawa1.png");
	texture_grass2.loadFromFile("Textures/Trawa2.png");
	texture_grass3.loadFromFile("Textures/Trawa3.png");
	texture_test1.loadFromFile("Textures/Test.png");

	Vector2u texture1_size = texture1.getSize();
	Vector2u texture2_size = texture2.getSize();
	Vector2u texture3_size = texture3.getSize();
	Vector2u texture_tree1_size = texture_tree1.getSize();
	Vector2u texture_bush1_size = texture_bush1.getSize();
	Vector2u texture5_size = texture5.getSize();
	Vector2u texture_grass1_size = texture_grass1.getSize();
	Vector2u texture_grass2_size = texture_grass2.getSize();
	Vector2u texture_grass3_size = texture_grass3.getSize();
	Vector2u texture_test1_size = texture_test1.getSize();

	//OBJECTS ARRAYS
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	Sprite *chunk = new Sprite[number_of_chunks];
	Sprite *drzewo = new Sprite[number_of_trees1];
	Sprite *bush1 = new Sprite[number_of_bushes1];
	Sprite *unit1 = new Sprite[number_of_units1];

	//ASSIGNING TEXTURES TO CHUNKS
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	for (int i = 0; i < number_of_chunks; i++)
	{
		random_number1= rand();
		//if (i % 2 == 0)
		if(random_number1 % 5 == 0)
		{
			chunk[i].setTexture(texture1);
		}
		else if (random_number1 % 5 == 1)
		{
			chunk[i].setTexture(texture2);
		}
		else if (random_number1 % 5 == 2)
		{
			chunk[i].setTexture(texture_grass1);
		}
		else if (random_number1 % 5 == 3)
		{
			chunk[i].setTexture(texture_grass2);
		}
		else if (random_number1 % 5 == 4)
		{
			chunk[i].setTexture(texture_grass3);
		}

		if (i < number_of_trees1)
		{
			drzewo[i].setTexture(texture_tree1);
		}

		if (i < number_of_bushes1)
		{
			bush1[i].setTexture(texture_bush1);
		}

		if (i < number_of_units1)
		{
			unit1[i].setTexture(texture5);
		}
	}

	//DEPLOYING CHUNKS IN THE SPACE
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	FloatRect chunk_size = chunk[0].getGlobalBounds();//Getting info about dimensions of chunk(first one is a template)
	for (int i = 0; i < map_height; i++)
	{
		for (int g = 0; g < map_width; g++)
		{
			chunk[g + (i*map_width)].setPosition((applicationWindow.getSize().x*0.5 + 250) - (g*chunk_size.width),
				(applicationWindow.getSize().y*0.5 - 250) + (i*chunk_size.height));
			//Nested loops ("i" stands for line number and "g" for column number)
			//Adding to the width column and line multiples for 250 height (in order to correct it)
		}
	}

	//MODIFYING THE VIEW
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	View chunk_view = applicationWindow.getDefaultView();//Window is the template for the view settings
	chunk_view.setSize(chunk_view.getSize().x, chunk_view.getSize().y * 2);//As texture we're scaling it up by 2 times
	chunk_view.setRotation(45);
	chunk_view.setCenter(applicationWindow.getSize().x*0.5, applicationWindow.getSize().y*0.5);
	//cout << chunk_view.getSize().x << " " << chunk_view.getSize().y << endl;
	View object_view = applicationWindow.getDefaultView();
	//chunk[0].setTexture(texture_test1);


	View minimap_view = chunk_view;
	minimap_view.zoom(1.3f);//Minimap zoom
	//minimap_view.setSize(chunk_view.getSize().x/6, chunk_view.getSize().y * 2/6);
	//minimap_view.setViewport(FloatRect(0.0f, 0.75f, 0.25f, 0.25f));
	minimap_view.setViewport(FloatRect(0.0f, 0.75f, 0.25f, 0.25f));
	//minimap_view.setSize(chunk_size.width*sqrt(2)*map_width, chunk_size.height*sqrt(2)*map_height);
	minimap_view.setCenter(chunk_size.width*sqrt(2)*map_width/2, chunk_size.height*sqrt(2)*map_height/2);

	if ((chunk_size.width*sqrt(2)*map_width > applicationWindow.getSize().x) || (chunk_size.height*sqrt(2)*map_height > applicationWindow.getSize().y))
	{
		if (chunk_size.width*sqrt(2)*map_width / applicationWindow.getSize().x > chunk_size.height*sqrt(2)*map_height / applicationWindow.getSize().y)
		{
			minimap_view.zoom(chunk_size.width*sqrt(2)*map_width/ applicationWindow.getSize().x);
		}
		else if(chunk_size.width*sqrt(2)*map_width / applicationWindow.getSize().x < chunk_size.height*sqrt(2)*map_height / applicationWindow.getSize().y)
		{
			minimap_view.zoom(chunk_size.height*sqrt(2)*map_height/ applicationWindow.getSize().y);
		}
	}
	//Minimap scaling
	//minimap_view.setCenter(oknoAplikacji.getSize().x , oknoAplikacji.getSize().y);
	cout << minimap_view.getSize().x << " " << minimap_view.getSize().y << endl;

	//DEPLOYING OBJECTS ON SPECIFIC CHUNKS
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	/*umiesc_obiekt(drzewo[0], texture4_size, image, 1000, texture1_size, oknoAplikacji, chunk_view);
	umiesc_obiekt(drzewo[1], texture4_size, image, 250, texture1_size, oknoAplikacji, chunk_view);
	umiesc_obiekt(drzewo[2], texture4_size, image, 453, texture1_size, oknoAplikacji, chunk_view);
	umiesc_obiekt(drzewo[3], texture4_size, image, 841, texture1_size, oknoAplikacji, chunk_view);
	umiesc_obiekt(drzewo[4], texture4_size, image, 134, texture1_size, oknoAplikacji, chunk_view);
	umiesc_obiekt(drzewo[5], texture4_size, image, 345, texture1_size, oknoAplikacji, chunk_view);
	umiesc_obiekt(drzewo[6], texture4_size, image, 612, texture1_size, oknoAplikacji, chunk_view);
	umiesc_obiekt(drzewo[7], texture4_size, image, 748, texture1_size, oknoAplikacji, chunk_view);
	umiesc_obiekt(unit1[0], texture5_size, image, 749, texture1_size, oknoAplikacji, chunk_view);*/
	umiesc_obiekt(unit1[1], texture5_size, chunk, 0, texture1_size, applicationWindow, chunk_view);

	for (int i = 0; i < number_of_trees1; i++) {
		umiesc_obiekt(drzewo[i], texture_tree1_size, chunk, rand() % (map_height*map_width), texture1_size, applicationWindow, chunk_view);
	}

	for (int i = 0; i < number_of_bushes1; i++) {
		umiesc_obiekt(bush1[i], texture_bush1_size, chunk, rand() % (map_height*map_width), texture1_size, applicationWindow, chunk_view);
	}

	//---------------------------------------------------------------------------------------------------------------------//
	Time time;
	Clock clock;

	//GAME MAIN SECTION
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	while (!quit)
	{
		time = clock.restart();//Getting time
		mouse_position = Mouse::getPosition(applicationWindow);//Getting mouse coordinates in every frame


		//PACKETS MAIN LOOP
		//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		while (!socket.receive(receive_packet, incomming_ip, incomming_port))
		{
			while (!receive_packet.endOfPacket())
			{
				Uint8 opcode;
				receive_packet >> opcode;

				switch (opcode)
				{
				case ADD_UNIT_TO_GAME:
				{
					Uint8 BP_jednostki;
					Uint8 x;
					Uint8 y;
					receive_packet >> BP_jednostki >> x >> y;
					break;
				}
				case SET_UNIT_POSITION:
				{
					Uint8 ID_jednostki;
					Uint8 x;
					Uint8 y;
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

		//SUPPORT OF INPUT DEVICES
		//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		Event zdarzenie;
		while (applicationWindow.pollEvent(zdarzenie))
		{
			switch (zdarzenie.type)
			{
			case Event::Closed:
				quit = true;
				break;
			case Event::KeyPressed:

				switch (zdarzenie.key.code)
				{
				case Keyboard::Escape:
				quit = true;
				break;
				case Keyboard::Numpad8:
					if (game_zoom < 0.5f)
					{
						game_zoom += 0.1f;
						//We're changing view size to desired one, adding we add a multiple of the resolution ratio (height * 2 because the height render would be 2 times greater for chunks)
						chunk_view.setSize(resolution_width + (16 * zoom_step)*game_zoom, (resolution_height * 2) + (9 * zoom_step*2)*game_zoom);
						object_view.setSize(resolution_width + (16 * zoom_step)*game_zoom, (resolution_height)+(9 * zoom_step)*game_zoom);
					}
				break;
				case Keyboard::Numpad2:
					if (game_zoom > -0.5f)
					{
						game_zoom -= 0.1f;
						chunk_view.setSize(resolution_width + (16 * zoom_step)*game_zoom, (resolution_height * 2) + (9 * zoom_step*2)*game_zoom);
						object_view.setSize(resolution_width + (16 * zoom_step)*game_zoom, (resolution_height) + (9 * zoom_step)*game_zoom);
					}
				break;
				default:
				break;
				}
				break;

			case Event::MouseButtonPressed:
				break;

			default:
				break;

				//CAMERA ZOOM
				//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
			case Event::MouseWheelScrolled:
					if (zdarzenie.mouseWheelScroll.delta <= -1)
					{
						if (game_zoom < 0.5f)//Dezooming
						{
							game_zoom += 0.1f;
							chunk_view.setSize(resolution_width + (16 * zoom_step)*game_zoom, (resolution_height * 2) + (9 * zoom_step * 2)*game_zoom);
							object_view.setSize(resolution_width + (16 * zoom_step)*game_zoom, (resolution_height)+(9 * zoom_step)*game_zoom);
						}
					}
					if (zdarzenie.mouseWheelScroll.delta >= 1)
					{
						if (game_zoom > -1.5f)//Zooming
						{
							game_zoom -= 0.1f;
							chunk_view.setSize(resolution_width + (16 * zoom_step)*game_zoom, (resolution_height * 2) + (9 * zoom_step * 2)*game_zoom);
							object_view.setSize(resolution_width + (16 * zoom_step)*game_zoom, (resolution_height)+(9 * zoom_step)*game_zoom);
						}
					}
					//cout << "wheel movement: " << zdarzenie.mouseWheelScroll.delta << endl;
					//cout << "mouse x: " << zdarzenie.mouseWheelScroll.x << endl;
					//cout << "mouse y: " << zdarzenie.mouseWheelScroll.y << endl;
				break;
			}
			
			//MOVING CAMERA
			//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
			if (mouse_position.x == 0)
			{
				chunk_view.move(-speed_of_scrolling, -speed_of_scrolling);//Camera move left
				object_view.move(-object_scroll, 0);
			}
			if (mouse_position.y == 0)
			{
				chunk_view.move(speed_of_scrolling, -speed_of_scrolling);//Camera move up
				object_view.move(0, -object_scroll/2);//In Y axis we have to use half of the value, because render was twice as big
			}
			if (mouse_position.x == applicationWindow.getSize().x-1)
			{
				chunk_view.move(speed_of_scrolling, speed_of_scrolling);//Camera move right
				object_view.move(object_scroll, 0);
			}
			if (mouse_position.y == applicationWindow.getSize().y-1)
			{
				chunk_view.move(-speed_of_scrolling, speed_of_scrolling);//Camera move down
				object_view.move(0, object_scroll/2);
			}
			//For objects I'm using object_scroll (sqrt(2)*speed_of_scrolling), because it is after tranformation
		}

		//IMAGE RENDER
		//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		applicationWindow.clear();
		applicationWindow.setView(chunk_view);//Sets view
		for (int i = 0; i < number_of_chunks; i++)
		{
			applicationWindow.draw(chunk[i]);//Displays chunks
		}
		//oknoAplikacji.setView(oknoAplikacji.getDefaultView());//Is to project objects straight
		applicationWindow.setView(object_view);
		for (int i = 0; i < number_of_trees1; i++)
		{
			applicationWindow.draw(drzewo[i]);
		}

		for (int i = 0; i < number_of_bushes1; i++)
		{
			applicationWindow.draw(bush1[i]);
		}

		for (int i = 0; i < number_of_units1; i++)
		{
			applicationWindow.draw(unit1[i]);
		}

		applicationWindow.setView(minimap_view);
		for (int i = 0; i < number_of_chunks; i++)
		{
			applicationWindow.draw(chunk[i]);//Displays chunks
		}
		applicationWindow.display();

		socket.send(send_packet, remote_ip, remote_port);//Sends packet
		send_packet.clear();//Clears packet
	}
	//---------------------------------------------------------------------------------------------------------------------//
	delete[] chunk;
	delete[] drzewo;
	delete[] unit1;
	applicationWindow.close();

	system("PAUSE");
	return EXIT_SUCCESS;
}