
#include <YogiRelay.h>


const uint8_t k_pinRelayOn = 4;
const uint8_t k_pinRelayOff = 3;
const uint8_t k_pinRelayConfirm = 0;


YogiRelay g_tRelay( k_pinRelayOn, k_pinRelayOff, k_pinRelayConfirm );


unsigned long g_uTimeCurrent = 0;
unsigned long g_uTimePrevious = 0;

void
setup()
{
    Serial.begin( 115200 );
    while ( ! Serial )
        ;
    delay( 300 );
    Serial.println( "YogiRelay Latching example" );

    g_tRelay.init();

    g_uTimeCurrent = 0;
    g_uTimePrevious = 0;
}


void
loop()
{
    g_uTimeCurrent = millis();
    if ( 2000 < g_uTimeCurrent - g_uTimePrevious )
    {
        g_uTimePrevious = g_uTimeCurrent;

        if ( g_tRelay.isSet() )
            g_tRelay.reset();
        else
            g_tRelay.set();
    }
}
