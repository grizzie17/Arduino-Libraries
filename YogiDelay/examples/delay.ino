#include <YogiDelay.h>


YogiDelay g_tDelay;


void
setup()
{
    g_tDelay.init( 500 );
}


void
loop()
{
    if ( g_tDelay.timesUp() )
    {
        // do something in a timely fashion
    }
}
