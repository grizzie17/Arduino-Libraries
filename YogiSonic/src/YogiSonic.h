//===============================================
/// @file: YogiSonic.h
//===============================================

#ifndef H_YOGISONIC
#define H_YOGISONIC
#pragma once


//-----------------------------------------------
// YogiSonic class
//-----------------------------------------------
class YogiSonic
{
public:
    YogiSonic( uint8_t nTrigger, uint8_t nEcho,
            unsigned long uTimeout = 200000UL );

public:
    void
    init();

    void
    setMaxDistance( unsigned nCM );

    long
    getDistanceCm();

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

    const int US_ROUNDTRIP_CM = 57;
    const int DIVISOR_CM = 28;
    const int DIVISOR_IN = 71;
    const int MAX_SENSOR_DISTANCE = 500;
};


#endif  // H_YOGISONIC
