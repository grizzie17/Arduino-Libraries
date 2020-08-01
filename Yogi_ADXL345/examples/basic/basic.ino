
#include "Yogi_ADXL345.h"


unsigned long g_uTimeCurrent = 0;
unsigned long g_uTimePrevious = 0;


ADXL345 adxl;

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

    adxl.endConfigure();


    g_uTimePrevious = 0;

    Serial.println( "setup complete" );
}


void
loop()
{
    g_uTimeCurrent = millis();
    if ( 500 < abs( g_uTimeCurrent - g_uTimePrevious ) )
    {
        g_uTimePrevious = g_uTimeCurrent;

        int x, y, z;
        adxl.readAccel( &x, &y, &z );

        Serial.print( x );
        Serial.print( "," );
        Serial.print( y );
        Serial.print( "," );
        Serial.println( z );
    }
}
