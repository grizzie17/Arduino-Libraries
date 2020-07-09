
#include "YogiSleep.h"

const uint8_t k_pinLED = LED_BUILTIN;
const uint8_t k_pinINT = 2;

YogiSleep g_tSleep;

unsigned long g_uTimeCurrent = 0;
unsigned long g_uTimePrevious = 0;

void
setup()
{
    Serial.begin( 115200 );
    while ( ! Serial )
        ;  // wait for Serial
    delay( 500 );
    pinMode( k_pinLED, OUTPUT );
    pinMode( k_pinINT, INPUT );

    g_uTimeCurrent = 0;
    g_uTimePrevious = 0;

    g_tSleep.savePinModes();
    g_tSleep.printPinModes();

    g_tSleep.setPinModesOutput();
}


void
loop()
{
    g_uTimeCurrent = millis();
    if ( 2000 < g_uTimeCurrent - g_uTimePrevious )
    {
        g_uTimePrevious = g_uTimeCurrent;
        digitalWrite( k_pinLED, LOW );

        g_tSleep.powerDown();
    }
    else if ( 1000 < g_uTimeCurrent - g_uTimePrevious )
    {
        digitalWrite( k_pinLED, HIGH );
    }
}
