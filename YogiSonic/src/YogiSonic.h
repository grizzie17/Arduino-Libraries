//===============================================
/// @file: YogiSonic.h
//===============================================

#ifndef YogiSonic_h
#define YogiSonic_h
#pragma once


//-----------------------------------------------
// YogiSonic class
//-----------------------------------------------
class YogiSonic
{
public:
    // construct with trigger and echo
    YogiSonic( uint8_t nTrigger, uint8_t nEcho, unsigned long uTimeout = 0 );

public:
    // initialize
    void
    init();

    // distance maximum cm
    void
    setMaxDistance( unsigned nCM );

    // retrieve the current distance cm
    long
    getDistanceCm();

    // previous distance
    long
    lastDistanceCm();

protected:
    long
    cmFromMicroseconds( unsigned long duration );

    uint8_t       m_nPinTrigger;
    uint8_t       m_nPinEcho;
    unsigned long m_uTimeout;
    unsigned long m_uMaxDistance;
    unsigned long m_uMaxDuration;
    long          m_nLastDistance;
    int           m_nZeroCount;

    const int  US_ROUNDTRIP_CM = 58;
    const int  DIVISOR_CM = 29.4;
    const int  DIVISOR_IN = 71;
    const long MAX_SENSOR_DISTANCE = 400;  // cm
    const long MAX_SENSOR_DURATION = MAX_SENSOR_DISTANCE * 2 * DIVISOR_CM;
};


#endif
