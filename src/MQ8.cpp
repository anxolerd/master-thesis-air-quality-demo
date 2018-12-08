#include "MQ8.hh"

MQ8::MQ8(uint8_t aoutPin, uint8_t doutPin) {
    _aoutPin = aoutPin;
    _doutPin = doutPin;
}

void MQ8::begin() {
    pinMode(_doutPin, INPUT);
}

int MQ8::readValue() {
    return analogRead(_aoutPin);
}

bool MQ8::isLimitReached() {
    return digitalRead(_doutPin) == HIGH;
}