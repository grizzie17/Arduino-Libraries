#include <Arduino.h>

#include "YogiDelay.h"


unsigned long YogiDelay::m_nCurrentTime = 0;


YogiDelay::YogiDelay()
        : m_nDelay( 250 )
        , m_nPreviousTime( 0 )
{}

YogiDelay::YogiDelay( unsigned long nMilliSeconds )
        : m_nDelay( nMilliSeconds )
        , m_nPreviousTime( 0 )
{}

void
YogiDelay::init( unsigned long nMilliSeconds )
{
    m_nDelay = nMilliSeconds;
    m_nPreviousTime = 0;
    m_nCurrentTime = millis();
}

void
YogiDelay::newDelay( unsigned long nMilliSeconds )
{
    m_nDelay = nMilliSeconds;
}


void
YogiDelay::reset()
{
    m_nPreviousTime = 0;
    m_nCurrentTime = millis();
}

bool
YogiDelay::timesUp( unsigned long nCurrentTime )
{
    if ( 0 == nCurrentTime )
    {
        nCurrentTime = millis();
    }
    m_nCurrentTime = nCurrentTime;
    if ( m_nDelay < m_nCurrentTime - m_nPreviousTime )
    {
        m_nPreviousTime = nCurrentTime;
        return true;
    }
    else
    {
        return false;
    }
}


bool
YogiDelay::peekTime( unsigned long nCurrentTime )
{
    if ( 0 == nCurrentTime )
    {
        nCurrentTime = millis();
    }
    m_nCurrentTime = nCurrentTime;
    if ( m_nDelay < m_nCurrentTime - m_nPreviousTime )
    {
        return true;
    }
    else
    {
        return false;
    }
}


// static
unsigned long
YogiDelay::getCurrentTime()
{
    return m_nCurrentTime;
}
