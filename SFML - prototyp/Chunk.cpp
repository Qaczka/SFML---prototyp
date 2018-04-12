#include <iostream>
#include "Chunk.h"

using namespace std;

Chunk::Chunk()
{
	set_width(10);
	set_height(10);
}

Chunk::Chunk(double a, double b)
{
	set_width(a);
	set_height(b);
}

void Chunk::set_width(double a)
{
	width = a;
}

void Chunk::set_height(double a)
{
	height = a;
}

double Chunk::get_height()
{
	return height;
}

double Chunk::get_width() {
	return width;
}