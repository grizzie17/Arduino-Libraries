#include <Arduino.h>


#include "YogiSonic.h"

#define SONIC_DEBUG_PRINT( s )   Serial.print( s )
#define SONIC_DEBUG_PRINTLN( s ) Serial.println( s )
// #define SONIC_DEBUG_PRINT( s )   ( 0 )
// #define SONIC_DEBUG_PRINTLN( s ) ( 0 )


#define MAX_SENSOR_DISTANCE 500
#define MAX_SENSOR_DELAY    5800
#define NO_ECHO             0


//-----------------------------------------------
// YogiSonic code
//-----------------------------------------------
YogiSonic::YogiSonic()
        : m_nPinTrigger( 0 )
        , m_nPinEcho( 0 )
        , m_uTimeout( 20000UL )
        , m_dLastDistance( 0.0 )
        , m_nZeroCount( 0 )
{}

YogiSonic::~YogiSonic()
{}

void
YogiSonic::init( uint8_t nTrigger, uint8_t nEcho, unsigned long uTimeout )
{
    m_nPinTrigger = nTrigger;
    m_nPinEcho = nEcho;
    m_uTimeout = uTimeout;
    m_nZeroCount = 0;

    pinMode( m_nPinTrigger, OUTPUT );
    pinMode( m_nPinEcho, INPUT );
}

double
YogiSonic::getDistanceCm()
{
    unsigned long uEchoTime = 0;
#ifdef RUS04
    pinMode( m_nPinTrigger, OUTPUT );
    digitalWrite( m_nPinTrigger, LOW );
    delayMicroseconds( 2 );
    digitalWrite( m_nPinTrigger, HIGH );
    delayMicroseconds( 10 );
    digitalWrite( m_nPinTrigger, LOW );
    pinMode( m_nPinTrigger, INPUT );
    uEchoTime = pulseIn( m_nPinTrigger, HIGH );
#else
    unsigned long uPreviousMicros = 0;
    digitalWrite( m_nPinTrigger, LOW );
    delayMicroseconds( 2 );
    digitalWrite( m_nPinTrigger, HIGH );
    delayMicroseconds( 10 );
    digitalWrite( m_nPinTrigger, LOW );
    // delayMicroseconds( 2 );
    // uEchoTime = pulseIn( m_nPinEcho, HIGH );
    uPreviousMicros = micros();
    while ( ! digitalRead( m_nPinEcho )
            && ( micros() - uPreviousMicros ) <= m_uTimeout )
        ;  // wait for echo pin HIGH or m_uTimeout
    uPreviousMicros = micros();
    while ( digitalRead( m_nPinEcho )
            && ( micros() - uPreviousMicros ) <= m_uTimeout )
        ;  // wait for echo pin LOW or timeout
    uEchoTime = micros() - uPreviousMicros;

#endif
    double dDist = CmFromMicroseconds( uEchoTime );
    if ( 1.0 <= dDist && dDist < 400.0 )
    {
        m_dLastDistance = dDist;
        return dDist;
    }
    else if ( dDist < 1.0 )
    {
        ++m_nZeroCount;
        SONIC_DEBUG_PRINT( "[" );
        SONIC_DEBUG_PRINT( m_nZeroCount );
        SONIC_DEBUG_PRINT( "] Sonic dist = " );
        SONIC_DEBUG_PRINTLN( dDist );
        return 0;
    }
    else
    {
        return m_dLastDistance;
    }
}

double
YogiSonic::CmFromMicroseconds( unsigned long duration )
{
    // const double kSpeedOfSound
    // 		= 0.03313 + 0.0000606 * 19.307;  // Cair
    // (331.3 + 0.606 * temperature) double dDist =
    // double( duration ) / 4.0 * kSpeedOfSound; return
    // dDist;
    double d = double( duration ) / DIVISOR_CM / 2;
    // d = static_cast<double>( duration )* d = double( duration ) * 340.0 / 20000;
    return d;
}

double
YogiSonic::lastDistanceCm()
{
    return m_dLastDistance;
}
