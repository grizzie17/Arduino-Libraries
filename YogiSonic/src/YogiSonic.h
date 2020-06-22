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
    YogiSonic();
    ~YogiSonic();

public:
    void
    init( uint8_t nTrigger, uint8_t nEcho, unsigned long uTimeout = 20000UL );

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
    long          m_nLastDistance;
    int           m_nZeroCount;

    const int US_ROUNDTRIP_CM = 57;
    const int DIVISOR_CM = 28;
    const int DIVISOR_IN = 71;
    const int MAX_SENSOR_DISTANCE = 500;
};


#endif  // H_YOGISONIC
