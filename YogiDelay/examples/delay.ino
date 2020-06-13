#include <YogiDelay.h>


YogiDelay     g_tDelay;
unsigned long g_uTimeCurrent;


void
setup()
{
    g_tDelay.init( 500 );
}


void
loop()
{
    g_uTimeCurrent = millis();
    if ( m_tDelay.timesUp( g_uTimeCurrent ) )
    {
        // do something in a timely fashion
    }
}
