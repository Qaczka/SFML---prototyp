#pragma once
#include <SFML/Graphics.hpp>

class Chunk
{
private:
	double width;
	double height;
public:
	Chunk();
	Chunk(double a, double b);
	double get_width();
	double get_height();
	void set_width(double a);
	void set_height(double a);

};