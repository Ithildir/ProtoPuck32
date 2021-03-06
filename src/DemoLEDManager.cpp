#include <Arduino.h>
#include "DemoLEDManager.h"

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
    CRGB::Red,
    CRGB::Gray, // 'white' is too bright compared to red and blue
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Red,
    CRGB::Gray,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Blue,
    CRGB::Black,
    CRGB::Black
};


void CDemoLEDManager::FillLEDsFromPaletteColors(CRGB *leds, uint8_t colorIndex)
{
    for( int i = 0; i < size; i++) {
        leds[i] = ColorFromPalette( palettes[currentPaletteIndex], colorIndex, brightness * 255, currentBlending);
        //leds[i] = ColorFromPalette( PartyColors_p, colorIndex, brightness, currentBlending);
        colorIndex += 1;
    }
}


// This function fills the palette with totally random colors.
void SetupTotallyRandomPalette()
{
    for( int i = 0; i < 16; i++) {
        currentPalette[i] = CHSV( random8(), 255, random8());
    }
}

// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void SetupBlackAndWhiteStripedPalette()
{
    // 'black out' all 16 palette entries...
    fill_solid( currentPalette, 16, CRGB::Black);
    // and set every fourth one to white.
    currentPalette[0] = CRGB::White;
    currentPalette[4] = CRGB::White;
    currentPalette[8] = CRGB::White;
    currentPalette[12] = CRGB::White;
    
}

// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette()
{
    CRGB purple = CHSV( HUE_PURPLE, 255, 255);
    CRGB green  = CHSV( HUE_GREEN, 255, 255);
    CRGB black  = CRGB::Black;
    
    currentPalette = CRGBPalette16(
                                   green,  green,  black,  black,
                                   purple, purple, black,  black,
                                   green,  green,  black,  black,
                                   purple, purple, black,  black );
}

CDemoLEDManager::CDemoLEDManager(CRGB *leds, uint16_t size, float brightness)
: CLEDManager(size, brightness) {

    this->leds = leds;

    tMillis = tMillsChangePalette = millis();
    changePalette = 0;

    CRGB purple = CHSV( HUE_PURPLE, 255, 255);
    CRGB green  = CHSV( HUE_GREEN, 255, 255);
    CRGB black  = CRGB::Black;

    palettes.push_back(
        CRGBPalette16(
            green,  green,  black,  black,
            purple, purple, black,  black,
            green,  green,  black,  black,
            purple, purple, black,  black )
    );

    palettes.push_back(RainbowColors_p);
    palettes.push_back(RainbowStripeColors_p);
    palettes.push_back(RainbowStripeColors_p);
    palettes.push_back(CloudColors_p);
    palettes.push_back(PartyColors_p);
    palettes.push_back(LavaColors_p);
    palettes.push_back(OceanColors_p);
    palettes.push_back(ForestColors_p);
    palettes.push_back(HeatColors_p);

    currentPaletteIndex = 0;
}

void CDemoLEDManager::ChangePalettePeriodically() {
    uint8_t secondHand = (millis() / 2000) % 60;
    static uint8_t lastSecond = 99;
    
    if( lastSecond != secondHand) {
        lastSecond = secondHand;
        if( secondHand ==  0)  { currentPalette = RainbowColors_p;         currentBlending = LINEARBLEND; }
        if( secondHand == 10)  { currentPalette = RainbowStripeColors_p;   currentBlending = NOBLEND;  }
        if( secondHand == 15)  { currentPalette = RainbowStripeColors_p;   currentBlending = LINEARBLEND; }
        if( secondHand == 20)  { SetupPurpleAndGreenPalette();             currentBlending = LINEARBLEND; }
        if( secondHand == 25)  { SetupTotallyRandomPalette();              currentBlending = LINEARBLEND; }
        if( secondHand == 30)  { SetupBlackAndWhiteStripedPalette();       currentBlending = NOBLEND; }
        if( secondHand == 35)  { SetupBlackAndWhiteStripedPalette();       currentBlending = LINEARBLEND; }
        if( secondHand == 40)  { currentPalette = CloudColors_p;           currentBlending = LINEARBLEND; }
        if( secondHand == 45)  { currentPalette = PartyColors_p;           currentBlending = LINEARBLEND; }
        if( secondHand == 50)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = NOBLEND;  }
        if( secondHand == 55)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = LINEARBLEND; }
    }
}

void CDemoLEDManager::loop() {
    if (millis() - tMillis > 50) {
        tMillis = millis();
        //ChangePalettePeriodically();
        startIndex = startIndex + 1;
    }

    if (millis() - tMillsChangePalette > 500) {
        tMillsChangePalette = millis();
        currentPaletteIndex += changePalette;
        if (currentPaletteIndex >= palettes.size()) {
            currentPaletteIndex = 0;
        }
        changePalette = 0;
    }
    
}


uint16_t CDemoLEDManager::LED_Status(CRGB *leds) {
    FillLEDsFromPaletteColors(this->leds, startIndex);
    return 10;
}

void CDemoLEDManager::keyEvent(key_status_t key) {

    switch (key) {
        case KEY_LEFT: changePalette = 1; break;
        case KEY_RIGHT: changePalette = -1; break;
        default: ;
    }

}