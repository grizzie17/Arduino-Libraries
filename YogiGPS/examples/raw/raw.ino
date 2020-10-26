
#include <SoftwareSerial.h>


const uint8_t k_pinRX = 7;
const uint8_t k_pinTX = 8;

SoftwareSerial g_tSSerial( k_pinRX, k_pinTX );

#define GpsSerial g_tSSerial


void
serialInterrupt()
{
    if ( Serial.available() )
        GpsSerial.write( Serial.read() );
}


void
setup()
{
    Serial.begin( 115200 );
    while ( ! Serial )
        ;  // wait serial startup

    GpsSerial.begin( 9600 );
    while ( ! GpsSerial )
        ;  // wait GpsSerial startup
}


void
loop()
{
    if ( 0 < GpsSerial.available() )
        Serial.write( GpsSerial.read() );
}
