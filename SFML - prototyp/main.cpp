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

	sf::Sprite obrazek;
	obrazek.setTexture(tekstura);

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
		oknoAplikacji.draw(obrazek);//wyswietla obrazek
		oknoAplikacji.display();


	}


	//system("PAUSE");
	return 0;
}