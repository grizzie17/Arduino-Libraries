#include <Arduino.h>


//#define SONIC_DEBUG 1


#include "YogiSonic.h"

//


#if defined( SONIC_DEBUG )
#    define SONIC_DEBUG_PRINT( s )   Serial.print( s )
#    define SONIC_DEBUG_PRINTLN( s ) Serial.println( s )
#else
#    define SONIC_DEBUG_PRINT( s )   ( 0 )
#    define SONIC_DEBUG_PRINTLN( s ) ( 0 )
#endif

//#define MAX_SENSOR_DELAY    5800
//#define NO_ECHO              0L


//-----------------------------------------------
// YogiSonic code
//-----------------------------------------------
YogiSonic::YogiSonic( uint8_t nTrigger, uint8_t nEcho, unsigned long uTimeout )
        : m_nPinTrigger( nTrigger )
        , m_nPinEcho( nEcho )
        , m_uTimeout( uTimeout )
        , m_uMaxDistance( 0 )
        , m_uMaxDuration( 0 )
        , m_nLastDistance( 1 )
        , m_nZeroCount( 0 )
{
    if ( 0 == m_uTimeout )
        m_uTimeout = MAX_SENSOR_DURATION;
}


void
YogiSonic::init()
{
    this->m_nZeroCount = 0;

    pinMode( this->m_nPinTrigger, OUTPUT );
    pinMode( this->m_nPinEcho, INPUT );
}


void
YogiSonic::setMaxDistance( unsigned nCM )
{
    this->m_uMaxDistance = nCM;
    this->m_uMaxDuration = nCM * 2 * DIVISOR_CM;
    // Serial.print( "max duration = " );
    // Serial.println( m_uMaxDuration );
}


long
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
    unsigned long uMaxDuration
            = 0 < m_uMaxDuration ? m_uMaxDuration : m_uTimeout;
    long uMaxDistance
            = 0 < m_uMaxDistance ? m_uMaxDistance : MAX_SENSOR_DISTANCE;
    unsigned long uPreviousMicros = 0;

    pinMode( m_nPinTrigger, OUTPUT );
    digitalWrite( this->m_nPinTrigger, LOW );
    delayMicroseconds( 2 );
    digitalWrite( this->m_nPinTrigger, HIGH );
    delayMicroseconds( 10 );
    digitalWrite( this->m_nPinTrigger, LOW );
    pinMode( m_nPinEcho, INPUT );
#    if false
    delayMicroseconds( 2 );
    uEchoTime = pulseIn( m_nPinEcho, HIGH );
#    else
    uPreviousMicros = micros();
    while ( ! digitalRead( this->m_nPinEcho )
            && ( micros() - uPreviousMicros ) <= m_uTimeout )
        ;  // wait for echo pin HIGH or m_uTimeout
    uPreviousMicros = micros();
    while ( digitalRead( this->m_nPinEcho )
            && ( micros() - uPreviousMicros ) <= m_uTimeout )
        ;  // wait for echo pin LOW or timeout
    uEchoTime = micros() - uPreviousMicros;
    // Serial.print( "echo time = " );
    // Serial.println( uEchoTime );
#    endif

#endif
    long nDist = cmFromMicroseconds( uEchoTime );
    if ( nDist <= uMaxDistance )
    {
        this->m_nLastDistance = nDist;
        return nDist;
    }
    else
    {
        this->m_nLastDistance = 0;
        return 0;
    }
}

long
YogiSonic::cmFromMicroseconds( unsigned long duration )
{
    // const double kSpeedOfSound
    // 		= 0.03313 + 0.0000606 * 19.307;  // Cair
    // (331.3 + 0.606 * temperature) double dDist =
    // double( duration ) / 4.0 * kSpeedOfSound; return
    // dDist;
    // d = static_cast<double>( duration )* d = double( duration ) * 340.0 / 20000;
    if ( ( 0 < m_uMaxDuration && duration <= m_uMaxDuration )
            || 0 == m_uMaxDuration || duration <= MAX_SENSOR_DURATION )
        return ( duration / 2 ) / DIVISOR_CM;
    else
        return 0;
}

long
YogiSonic::lastDistanceCm()
{
    return this->m_nLastDistance;
}
