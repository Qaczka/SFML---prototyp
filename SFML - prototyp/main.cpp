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

	//sf::Window oknoAplikacji(sf::VideoMode(1280, 720, 32), "Kelajno");// szer, wys, glebia bitowa
	// mozna to podac bez argumentow - zwykla inicjalizacja ale wtedy tworzysz oknoAplikacji.create(...);

	sf::RenderWindow oknoAplikacji(sf::VideoMode(1920, 1080, 32), "Kelajno", sf::Style::Fullscreen);//to opcja fullscreen

	sf::Texture tekstura;//sluzy do wczytywania tekstury bo jest texture i image
	tekstura.loadFromFile("C:/Users/Qaczka/Desktop/Moje/Grunt.png");//zwraca true lub false

	sf::Sprite obrazek[4];
	obrazek[0].setTexture(tekstura);
	obrazek[1].setTexture(tekstura);
	obrazek[2].setTexture(tekstura);
	obrazek[3].setTexture(tekstura);
	sf::FloatRect chunk_size = obrazek[1].getGlobalBounds();

	obrazek[1].setRotation(45.f);//obrot o 45 stopni
	obrazek[2].setRotation(45.f);

	//obrazek[1].setPosition(obrazek[1].getPosition().x, obrazek[1].getPosition().y * 2);
	obrazek[1].setPosition(oknoAplikacji.getSize().x * .5, oknoAplikacji.getSize().y);
	obrazek[2].setPosition((oknoAplikacji.getSize().x * .5)+chunk_size.width, (oknoAplikacji.getSize().y)+chunk_size.height);
	/*
	void setPosition( float x, float y );
	void setPosition( const sf::Vector2f & position );

	void move( float offsetX, float offsetY );
	void move( const sf::Vector2f & offset );
	*/

	sf::View v = oknoAplikacji.getDefaultView();//widok ma byc taki jak okno tak jakby ciagnie z niego dane
	v.setSize(v.getSize().x, v.getSize().y * 2);//tak jak przy teksturze
	v.setCenter(v.getSize() *.5f);//center bo bierzesz rozmiar i na pol .5 - 0.5

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
		oknoAplikacji.draw(obrazek[1]);//wyswietla obrazek
		oknoAplikacji.draw(obrazek[2]);
		//oknoAplikacji.setView(oknoAplikacji.getDefaultView());//nie wiem czemu
		oknoAplikacji.display();


	}


	system("PAUSE");
	return 0;
}