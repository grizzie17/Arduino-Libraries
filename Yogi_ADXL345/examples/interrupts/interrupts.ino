
#include "Yogi_ADXL345.h"


const uint8_t k_pinINT = 2;
const uint8_t k_pinLED = LED_BUILTIN;

const byte k_mIntActive = INTERRUPT_ACTIVITY | INTERRUPT_DOUBLE_TAP
        | INTERRUPT_SINGLE_TAP | INTERRUPT_FREE_FALL;
const byte k_mIntAll = k_mIntActive | INTERRUPT_INACTIVITY;


unsigned long g_uTimeCurrent = 0;
unsigned long g_uTimePrevious = 0;


ADXL345 adxl;


volatile bool g_bInterrupts = false;

void
interruptHandler()
{
    g_bInterrupts = true;
}

void
setup()
{
    Serial.begin( 115200 );
    while ( ! Serial )
        ;  // wait for serial bus

    delay( 500 );

    Serial.println( "Yogi_ADXL345 setup" );

    adxl.configureI2C();

    adxl.setRange( 2 );

    adxl.setActivityXYZ( true, true, true );
    adxl.setActivityAc( true );
    adxl.setActivityThreshold( 16 );
    adxl.setInactivityXYZ( true, true, true );
    adxl.setInactivityAc( true );
    adxl.setInactivityThreshold( 16 );


    adxl.setInterrupts( k_mIntAll );
    adxl.setInterruptMap( k_mIntAll );  // use INT2

    adxl.endConfigure();

    adxl.printAllRegisters();

    g_bInterrupts = false;
    pinMode( k_pinINT, INPUT );
    attachInterrupt(
            digitalPinToInterrupt( k_pinINT ), interruptHandler, RISING );

    pinMode( k_pinLED, OUTPUT );
    digitalWrite( k_pinLED, LOW );


    g_uTimePrevious = 0;

    Serial.println( "setup complete" );
}


void
loop()
{
    if ( g_bInterrupts )
    {
        g_bInterrupts = false;
        byte m = adxl.getInterruptSource();
        digitalWrite( k_pinLED, HIGH );
        Serial.print( "Interrupt: " );
        Serial.println( m, BIN );
        if ( 0 != ( m & INTERRUPT_INACTIVITY ) )
        {
            adxl.setInterrupts( k_mIntActive );
        }
        else if ( 0 != ( m & k_mIntActive ) )
        {
            adxl.setInterrupts( k_mIntAll );
        }

        int x, y, z;
        adxl.readAccel( &x, &y, &z );

        Serial.print( x );
        Serial.print( "," );
        Serial.print( y );
        Serial.print( "," );
        Serial.println( z );
    }
    else
    {
        g_uTimeCurrent = millis();
        if ( 1000 * 3 < abs( g_uTimeCurrent - g_uTimePrevious ) )
        {
            g_uTimePrevious = g_uTimeCurrent;
            digitalWrite( k_pinLED, LOW );
        }
    }
}
