/**
* @author Sávio Carlos Martins Costa
**/

#include "LedStrip.hpp"
#include <EEPROM.h>

LedStrip::LedStrip(uint8_t redPin, uint8_t greenPin, uint8_t bluePin, uint16_t eepromIndex) {
    this->redPin = redPin;
    this->greenPin = greenPin;
    this->bluePin = bluePin;
    this->eepromIndex = eepromIndex;
    // Set the PWM pins to OUTPUT mode
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
    // Load the last saved color stored in the EEPROM memory and sets it.
    EEPROM.get(eepromIndex, this->color);
    this->setColor(this->color);
}

// Takes a numeric character in the hexadecimal base (0 to F)
// and returns the corresponding value as an integer in base 10 (0 to 15).
static inline uint8_t hex2dec(char hex) {
    return
    (hex>='0' && hex<='9') ? (hex-48) :
    (hex>='A' && hex<='F') ? (hex-55) :
    (hex>='a' && hex<='f') ? (hex-87) : 0;
}

// Decodes a color written in Hexadecimal form ("#RRGGBB") into a Color object.
Color decodeColor(char* hex_color) {
    return {
        (uint8_t)(hex2dec(hex_color[1]) * 16 + hex2dec(hex_color[2])),
        (uint8_t)(hex2dec(hex_color[3]) * 16 + hex2dec(hex_color[4])),
        (uint8_t)(hex2dec(hex_color[5]) * 16 + hex2dec(hex_color[6]))
    };
}

void LedStrip::setColor(Color color, bool smoothChange) {
    if(smoothChange) {
        int8_t deltaRed = (color.red > this->color.red) ? 1 : -1;
        int8_t deltaGreen = (color.green > this->color.green) ? 1 : -1;
        int8_t deltaBlue = (color.blue > this->color.blue) ? 1 : -1;
        while (this->color != color) {
            if(this->color.red != color.red) this->color.red += deltaRed;
            if(this->color.green != color.green) this->color.green += deltaGreen;
            if(this->color.blue != color.blue) this->color.blue += deltaBlue;
            this->setPwmValues(this->color.red, this->color.green, this->color.blue);
            delay(this->timePerTrasition);
        }
    } else {
        this->setPwmValues(color.red, color.green, color.blue);
        this->color = color;
    }
    EEPROM.put(eepromIndex, color);
}

void LedStrip::setColor(char *hexColor, bool smoothChange) {
    this->setColor(decodeColor(hexColor), smoothChange);
}

void LedStrip::setLightIntensity(double intensity, bool smoothChange) {
    if (intensity < 0 || intensity > 1) return;
    Color newColor = this->color * intensity;
    this->setColor(newColor, smoothChange);
}

void LedStrip::setMaxTimePerColorChange(uint16_t milliseconds) {
    this->timePerTrasition = milliseconds/255;
}

Color LedStrip::getColor() {
    return this->color;
}

void LedStrip::setPwmValues(uint8_t redPinValue, uint8_t greenPinValue, uint8_t bluePinValue) {
    analogWrite(this->redPin, redPinValue);
    analogWrite(this->greenPin, greenPinValue);
    analogWrite(this->bluePin, bluePinValue);
}
