
#include <YogiGPS.h>


YogiGPS<Serial_> g_tGPS( Serial );


void
setup()
{
    Serial.begin( 115200 );
    while ( ! Serial )
        ;  // startup serial port
}


void
loop()
{}
