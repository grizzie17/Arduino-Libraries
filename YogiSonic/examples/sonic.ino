
#include <YogiDelay.h>
#include <YogiSonic.h>


const uint8_t kPinTrigger = 7;
const uint8_t kPinEcho = 6;


YogiSonic g_tSonic;
YogiDelay g_tDelay;

unsigned long g_uTimeCurrent = 0;

void
setup()
{
    Serial.begin( 9600 );
    g_tDelay.init( 1000 / 5 );  // divisor is hz value
    g_tSonic.init( kPinTrigger, kPinEcho );
    g_uTimeCurrent = millis();
}


void
loop()
{
    g_uTimeCurrent = millis();
    if ( g_tDelay.timesUp( g_uTimeCurrent ) )
    {
        double dist = g_tSonic.getDistanceCm();
        Serial.print( "Distance = " );
        Serial.print( dist );
        Serial.println( " cm" );
    }
}