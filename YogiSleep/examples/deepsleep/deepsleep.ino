#include <LowPower.h>


#include "YogiSleep.h"

const uint8_t k_pinLED = LED_BUILTIN;
const uint8_t k_pinINT = 2;

YogiSleep g_tSleep;

unsigned long g_uTimeCurrent = 0;
unsigned long g_uTimePrevious = 0;

volatile bool    g_bInterrupt = false;
volatile uint8_t g_uIntButton = LOW;
unsigned long    g_uCountInterrupt = 0;


void
interruptHandler()
{
    g_bInterrupt = true;
}


void
setup()
{
    // Serial.begin( 115200 );
    // while ( ! Serial )
    //     ;  // wait for Serial
    // delay( 500 );
    // Serial.println( "Deep Sleep example" );
    for ( uint8_t i = 2; i <= 13 + 8; ++i )
    {
        pinMode( i, OUTPUT );
        // digitalWrite( i, LOW );
    }
    pinMode( k_pinLED, OUTPUT );
    digitalWrite( k_pinLED, HIGH );

    g_tSleep.setup( k_pinINT );


    g_uTimeCurrent = 0;
    g_uTimePrevious = 0;
    g_uCountInterrupt = 0;

    // g_tSleep.savePinModes();
    // g_tSleep.printPinModes();

    // g_tSleep.setPinModesOutput();
}


bool                g_bSleep = false;
uint8_t             g_bButtonValue = LOW;
uint8_t             g_bButtonLast = LOW;
unsigned long       g_uDebounceLast = 0;
const unsigned long g_uDebounceDelay = 500;
unsigned long       g_uTimeUp = 0;

uint8_t g_uLedState = HIGH;
uint8_t g_uButtonState;
uint8_t g_uButtonLast = LOW;


void
loop()
{
    if ( g_tSleep.getInterrupt() )
    {
        g_bInterrupt = false;
        uint8_t uButtonRead = g_tSleep.getIntValue();
        // Serial.print( "button=" );
        // Serial.println( uButtonRead );
        // if ( uButtonRead != g_uButtonLast )
        //     g_uDebounceLast = millis();
        g_uTimeCurrent = millis();
        g_uButtonState = LOW;
        if ( g_uDebounceDelay < abs( millis() - g_uDebounceLast ) )
        {
            g_uDebounceLast = g_uTimeCurrent;
            g_uButtonState = HIGH;
        }
        if ( HIGH == g_uButtonState )
        {
            digitalWrite( k_pinLED, LOW );
            // Serial.println( "Sleepy" );
            delay( 200 );
            g_tSleep.powerDown();
            // attachInterrupt( digitalPinToInterrupt( k_pinINT ),
            //         interruptHandler, RISING );
            digitalWrite( k_pinLED, HIGH );
            delay( 300 );
            // Serial.println( "Wakeup" );
            // g_uTimePrevious = millis();
            // g_bSleep = false;
        }


        // g_uTimeCurrent = millis();
        // if ( 200 < abs( g_uTimeCurrent - g_uTimePrevious ) )
        // {
        //     g_uTimePrevious = g_uTimeCurrent;
        //     Serial.print( "Interrupt " );
        //     Serial.println( ++g_uCountInterrupt );

        //     if ( g_bSleep )
        //     {
        //         g_bSleep = false;
        //         digitalWrite( k_pinLED, HIGH );
        //     }
        //     else
        //     {
        //         g_bSleep = true;
        //         digitalWrite( k_pinLED, LOW );
        //     }

        //     // digitalWrite( k_pinLED, LOW );
        //     // Serial.println( "Sleepy" );
        //     // g_tSleep.powerDown();
        //     // digitalWrite( k_pinLED, HIGH );
        //     // Serial.println( "Wakeup" );
        //     // g_uTimePrevious = millis();
        //     // g_bSleep = false;
        // }
    }
    // g_uTimeCurrent = millis();
    // if ( 2000 < g_uTimeCurrent - g_uTimePrevious )
    // {
    //     g_uTimePrevious = g_uTimeCurrent;
    // }
    // else if ( 1000 < g_uTimeCurrent - g_uTimePrevious )
    // {
    //     digitalWrite( k_pinLED, HIGH );
    // }
}
