#include <iostream> //cout i cin
#include <stdlib.h> //dla system PAUSE
#include <string> //dla tringow
#include <SFML/Graphics.hpp> // SFML do renderingu
#include <SFML/Window.hpp> // dla okna SFML
#include <cmath> // do przeksztalcen


//w dependendiences -d zeby wlaczyc tryb dla debugoania

// Wylaczenie konsoli |-|  Linker-> System -> subSystem -> Windows (/SUBSYSTEM:WINDOWS)
// Kopiowanie plików dll do folderu

void testowe(sf::Clock stoper)
{
	sf::RenderWindow okno(sf::VideoMode(1280, 720), "Kelajno");
	while (okno.isOpen())
	{
		sf::Event event;
		while (okno.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				okno.close();

		} //while
		okno.clear();

		sf::CircleShape ksztalt(std::sin(stoper.getElapsedTime().asSeconds()) * okno.getSize().y / 8 + okno.getSize().y / 4);
		ksztalt.setOrigin(sf::Vector2f(ksztalt.getRadius(), ksztalt.getRadius()));
		ksztalt.setPosition(okno.getSize().x / 2.0f, okno.getSize().y / 2.0f);
		ksztalt.setFillColor(sf::Color::Yellow);
		okno.draw(ksztalt);

		okno.display();
	} //while
}

using namespace std;

int main()
{
	//sf::RenderWindow okno(sf::VideoMode(1280, 720), "Kelajno");
	sf::Clock stoper;
	//testowe(stoper);
	int number_of_chunks = 1024;
	int map_width = 32, map_height = 32;

	//sf::Window oknoAplikacji(sf::VideoMode(1280, 720, 32), "Kelajno");// szer, wys, glebia bitowa
	// mozna to podac bez argumentow - zwykla inicjalizacja ale wtedy tworzysz oknoAplikacji.create(...);

	sf::RenderWindow oknoAplikacji(sf::VideoMode(1920, 1080, 32), "Kelajno", sf::Style::Fullscreen);//to opcja fullscreen

	sf::Texture tekstura;//sluzy do wczytywania tekstury bo jest texture i image
	tekstura.loadFromFile("Textures/Grunt.png");//zwraca true lub false
	sf::Texture tekstura2;
	tekstura2.loadFromFile("Textures/Grunt2.png");
	sf::Texture tekstura3;
	tekstura3.loadFromFile("Textures/Drzewko.png");

	//sf::Sprite obrazek[4];
	sf::Sprite *obrazek = new sf::Sprite[number_of_chunks];
	for (int i = 0; i < number_of_chunks; i++)
	{
		if (i % 2 == 0)
		{
			obrazek[i].setTexture(tekstura);
		}
		else
		{
			obrazek[i].setTexture(tekstura2);
		}
	}

	sf::Sprite drzewo;
	drzewo.setTexture(tekstura3);

	sf::FloatRect chunk_size = obrazek[0].getGlobalBounds();//biore info odnosnie wymiarow chunka na przykladzie pierwszego


	for (int i = 0; i < map_height; i++)
	{
		for (int g = 0; g < map_width; g++)
		{
			obrazek[g + (i*map_width)].setPosition((oknoAplikacji.getSize().x * 0.5+250)-(g*chunk_size.width), (oknoAplikacji.getSize().y * 0.5-250) + (i*chunk_size.height));
			//podwojna petla "i" dla nr wiersza i "g" dla kolumny - do szerokosci dodaje wielokrotnosci kolumnowe i wierszowe dla wysokosci
			//250 na korekte
		}
	}

	drzewo.setPosition((oknoAplikacji.getSize().x * 0.5), (oknoAplikacji.getSize().y * 0.5));
	//obrazek[1].setPosition(obrazek[1].getPosition().x, obrazek[1].getPosition().y * 2);

	/*
	void setPosition( float x, float y );
	void setPosition( const sf::Vector2f & position );

	void move( float offsetX, float offsetY );
	void move( const sf::Vector2f & offset );
	*/

	sf::View v = oknoAplikacji.getDefaultView();//widok ma byc taki jak okno tak jakby ciagnie z niego dane
	v.setSize(v.getSize().x, v.getSize().y * 2);//tak jak przy teksturze skalujemy 2 wieksza wysoksoc
	//v.setCenter(v.getSize() *.5f);//center bo bierzesz rozmiar i na pol .5 - 0.5
	v.setRotation(45);
	//v.zoom(2.0f);
	//v.move(0.f, 0.f);
	v.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));//kamera o polowe 90 stopni czyli 0.5f w dol plus przesuniecie o cwierc w dol

	while (oknoAplikacji.isOpen())
	{

		sf::Event zdarzenie;

		//oknoAplikacji.clear(sf::Color(255, 0, 0)); //pusty nawias czysci na czarno a tak to RGBA
		while (oknoAplikacji.pollEvent(zdarzenie))
		{
			//tu obs³uga zdarzeñ

			if (zdarzenie.type == sf::Event::Closed)//chec zamkniecia
				oknoAplikacji.close();

			if (zdarzenie.type == sf::Event::KeyPressed && zdarzenie.key.code == sf::Keyboard::Escape)//wciskasz (w dol) esc - zamyka
				oknoAplikacji.close();

			if (zdarzenie.type == sf::Event::MouseButtonPressed && zdarzenie.mouseButton.button == sf::Mouse::Middle)//SPM zamyka
				oknoAplikacji.close();
		}
		oknoAplikacji.clear(sf::Color(0, 0, 0));
		oknoAplikacji.setView(v);//ustawia widok

		for (int i = 0; i < number_of_chunks; i++)
		{
			oknoAplikacji.draw(obrazek[i]);//wyswietla chunki
		}
		oknoAplikacji.setView(oknoAplikacji.getDefaultView());//zeby zrzutowac drzewo prosto
		oknoAplikacji.draw(drzewo);

		oknoAplikacji.display();
	}

	delete[] obrazek;
	return 0;
}