#include "RGBLed.hh"

RGBLed::RGBLed(uint8_t redPin, uint8_t greenPin, uint8_t bluePin) {
    _red = redPin;
    _green = greenPin;
    _blue = bluePin;
}

void RGBLed::begin() {
    pinMode(_red, OUTPUT);
    pinMode(_green, OUTPUT);
    pinMode(_blue, OUTPUT);
}

void RGBLed::color(char color) {
    digitalWrite(_red, color & COLOR_RED ? HIGH : LOW);
    digitalWrite(_green, color & COLOR_GREEN ? HIGH : LOW);
    digitalWrite(_blue, color & COLOR_BLUE ? HIGH : LOW);
    _currentColor = (color & (COLOR_RED | COLOR_GREEN | COLOR_BLUE));
}

void RGBLed::none() {
    color(0);
}

void RGBLed::red() {
    color(COLOR_RED);
}

void RGBLed::green() {
    color(COLOR_GREEN);
}

void RGBLed::blue() {
    color(COLOR_BLUE);
}

void RGBLed::cyan() {
    color(COLOR_BLUE | COLOR_GREEN);
}

void RGBLed::magenta() {
    color(COLOR_RED | COLOR_BLUE);
}

void RGBLed::yellow() {
    color(COLOR_GREEN | COLOR_RED);
}

void RGBLed::white() {
    color(COLOR_RED | COLOR_GREEN | COLOR_BLUE);
}

char RGBLed::getColor() {
    return _currentColor;
}