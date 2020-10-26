


void
setup()
{
    Serial.begin( 115200 );
    while ( ! Serial )
        ;  // wait for Serial startup

    delay( 500 );

    Serial.println( "Test Time Conversion" );

    float fValue = 104512.010;

    float fFract;
    float fInt;
    fFract = modff( fValue, &fInt );
    long nValue = static_cast<long>( fInt );
    long hr = nValue / 10000;
    nValue -= hr * 10000;
    long mn = nValue / 100;
    nValue -= mn * 100;
    long sc = nValue;
    long ms = static_cast<long>( 1000.0 * fFract );

    Serial.print( "input=" );
    Serial.println( fValue, 6 );
    Serial.print( hr );
    Serial.print( ":" );
    Serial.print( mn );
    Serial.print( ":" );
    Serial.print( sc );
    Serial.print( "." );
    Serial.println( ms );
}


void
loop()
{}
