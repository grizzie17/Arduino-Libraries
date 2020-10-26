
#include <SoftwareSerial.h>

#include <YogiGPS.h>


const uint8_t k_pinRX = 7;
const uint8_t k_pinTX = 8;
const uint8_t k_pinPWR = 9;

SoftwareSerial g_tSSerial( k_pinRX, k_pinTX );


// fills with leading zeros
void
Serial_printXX( int nValue )
{
    if ( nValue < 10 )
        Serial.print( "0" );
    Serial.print( nValue );
}

void
Serial_printXXX( int nValue )
{
    if ( nValue < 10 )
        Serial.print( "00" );
    else if ( nValue < 100 )
        Serial.print( "0" );
    Serial.print( nValue );
}


//===============================================
// Collect NMEA Sentence
//===============================================

YogiGpsSentence<SoftwareSerial> g_tSentence( g_tSSerial );


//===============================================
// Parse NMEA Sentence
//===============================================

YogiGpsParse g_tParse;


//===============================================
// Main Code
//===============================================

void
setup()
{
    Serial.begin( 115200 );
    while ( ! Serial )
        ;  // startup serial port

    g_tSSerial.begin( 9600 );

    pinMode( k_pinRX, INPUT );
    pinMode( k_pinTX, OUTPUT );
    pinMode( k_pinPWR, OUTPUT );
    digitalWrite( k_pinPWR, LOW );
    delay( 1000 );
    digitalWrite( k_pinPWR, HIGH );
    delay( 1000 );

    g_tParse.initData();

    Serial.println( "setup complete" );
}


void
loop()
{
    if ( 0 < g_tSentence.read() )
    {
        // Serial.print( "Sentence: " );
        int         nLength;
        const char* sentence = g_tSentence.sentence( &nLength );
        if ( g_tParse.parse( sentence, nLength ) )
        {
            Serial.print( "[" );
            Serial.print( g_tParse.satellites );
            Serial.print( "] " );
            if ( 0 != g_tParse.status )
            {
                Serial.print( g_tParse.status );
                Serial.print( ": " );
            }
            if ( 0 < g_tParse.year )
            {
                Serial.print( g_tParse.year );
                Serial.print( "-" );
                Serial_printXX( g_tParse.month );
                Serial.print( "-" );
                Serial_printXX( g_tParse.day );
                Serial.print( " T " );
            }
            Serial_printXX( g_tParse.hour );
            Serial.print( ":" );
            Serial_printXX( g_tParse.minute );
            Serial.print( ":" );
            Serial_printXX( g_tParse.seconds );
            if ( 0 < g_tParse.milliseconds )
            {
                Serial.print( "." );
                Serial_printXXX( g_tParse.milliseconds );
            }
            // add milliseconds

            if ( 0 < g_tParse.latitude && 0 < g_tParse.longitude )
            {
                Serial.print( "; lat=" );
                Serial.print( g_tParse.latitude, 7 );
                Serial.print( " " );
                Serial.print( g_tParse.lat );
                Serial.print( " (" );
                Serial.print( g_tParse.latitude_fixed );
                Serial.print( ")" );
                Serial.print( ", lon=" );
                Serial.print( g_tParse.longitude, 7 );
                Serial.print( " " );
                Serial.print( g_tParse.lon );
                Serial.print( " (" );
                Serial.print( g_tParse.longitude_fixed );
                Serial.print( ")" );
            }
            if ( 0.0 != g_tParse.altitude )
            {
                Serial.print( "; alt=" );
                Serial.print( g_tParse.altitude );
            }

            Serial.println( "" );
        }
        // Serial.println( g_tSentence.sentence() );
        g_tSentence.clearSentence();
    }
}
