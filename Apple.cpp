#include "Apple.h"

Apple::Apple() : coords(std::pair<int, int>(0, 0)) {};

Apple::~Apple(){};

std::pair<int, int> Apple::getCoords()
{
    return coords;
};

void Apple::setCoords(std::pair<int, int> newCoords)
{
    coords = newCoords;
};

void Apple::draw(FieldType &field)
{
    field[coords.second][coords.first] = APPLE_SYMBOL;
};