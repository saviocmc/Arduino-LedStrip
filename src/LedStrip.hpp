/**
* @author Sávio Carlos Martins Costa
**/

#ifndef Led_Strip_hpp
#define Led_Strip_hpp

#include <Arduino.h>

struct Color {
    uint8_t red, green, blue;
    bool operator == (Color &color) { return (this->red == color.red && this->green == color.green && this->blue == color.blue); }
    bool operator != (Color &color) { return !(*this == color); }
    Color operator + (Color &color) { return {(uint8_t)(this->red + color.red), (uint8_t)(this->green + color.green), (uint8_t)(this->blue + color.blue)}; }
    Color operator * (double scalar) { return {(uint8_t)(this->red*scalar), (uint8_t)(this->green*scalar), (uint8_t)(this->blue*scalar)}; }
};

class LedStrip {

public:
    LedStrip(uint8_t redPin, uint8_t greenPin, uint8_t bluePin, uint16_t eepromIndex = 512);
    void setColor(Color color, bool smoothChange = true);
    void setColor(char* hexColor, bool smoothChange = true);
    void setLightIntensity(double intensity, bool smoothChange = true);
    void setMaxTimePerColorChange(uint16_t milliseconds);
	Color getColor();

private:
    uint8_t redPin, greenPin, bluePin;
    uint16_t eepromIndex, timePerTrasition = 10;
    Color color;
	void setPwmValues(uint8_t redPinValue, uint8_t greenPinValue, uint8_t bluePinValue);
};

#endif
