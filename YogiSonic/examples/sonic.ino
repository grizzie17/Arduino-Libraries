
#include <YogiDelay.h>
#include <YogiSonic.h>


const uint8_t kPinTrigger = 7;
const uint8_t kPinEcho = 6;


YogiSonic g_tSonic;
YogiDelay g_tDelay;


void
setup()
{
    Serial.begin( 9600 );
    g_tDelay.init( 1000 / 5 );  // divisor is hz value
    g_tSonic.init( kPinTrigger, kPinEcho );
}


void
loop()
{
    if ( g_tDelay.timesUp() )
    {
        long dist = g_tSonic.getDistanceCm();
        Serial.print( "Distance = " );
        Serial.print( dist );
        Serial.println( " cm" );
    }
}