
#include <YogiDelay.h>
#include <YogiSonic.h>


const uint8_t kPinTrigger = 9;
const uint8_t kPinEcho = 9;


YogiSonic g_tSonic( kPinTrigger, kPinEcho );
YogiDelay g_tDelay;


void
setup()
{
    Serial.begin( 115200 );
    g_tDelay.init( 1000 / 5 );  // divisor is hz value
    g_tSonic.init();
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