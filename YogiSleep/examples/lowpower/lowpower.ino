#include <LowPower.h>


const uint8_t k_pinINT = 2;


void
wakeUp()
{}


void
goSleep()
{
    attachInterrupt( digitalPinToInterrupt( k_pinINT ), wakeUp, RISING );

    LowPower.powerDown( SLEEP_FOREVER, ADC_OFF, BOD_OFF );

    detachInterrupt( digitalPinToInterrupt( k_pinINT ) );
}


void
setup()
{
    for ( uint8_t i = 2; i <= 13 + 8; ++i )
    {
        pinMode( i, OUTPUT );
        // digitalWrite( i, LOW );
    }
    pinMode( k_pinINT, INPUT );
}


void
loop()
{
    goSleep();
}
